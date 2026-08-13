#!/usr/bin/env python3
from __future__ import annotations

import json
import math
import os

os.environ["OMP_NUM_THREADS"] = "1"
os.environ["OPENBLAS_NUM_THREADS"] = "1"
os.environ["MKL_NUM_THREADS"] = "1"
os.environ["VECLIB_MAXIMUM_THREADS"] = "1"
os.environ["NUMEXPR_NUM_THREADS"] = "1"

from pathlib import Path
import time

# This import installs the tracked OpenCV vendor directory before cv2 is loaded.
from openpilot.sunnypilot.rivianpilot.vision_bsm_inference import VisionBSMInference

import cv2
import numpy as np

import cereal.messaging as messaging

from openpilot.common.params import Params
from openpilot.common.realtime import Ratekeeper, set_core_affinity
from openpilot.common.swaglog import cloudlog
from openpilot.system.hardware import PC


# The observer is completely idle without a single active blinker. While a
# blinker is active, sample at 1 Hz and briefly confirm candidates at 2 Hz.
# Promote to 2/5 Hz only after clean on-road resource data.
BASE_INTERVAL = 1.000
FOLLOWUP_INTERVAL = 0.500
FOLLOWUP_WINDOW = 1.5
FOLLOWUP_COOLDOWN = 2.0
CANDIDATE_CONFIDENCE_RATIO = 0.70
ONROAD_STARTUP_DELAY = 20.0
DRIVING_STACK_STABLE_SECONDS = 15.0
PARAM_REFRESH_INTERVAL = 2.0
STATUS_LOG_INTERVAL = 30.0
ERROR_LOG_INTERVAL = 30.0
RESOURCE_SKIP_LOG_INTERVAL = 10.0
DEFAULT_ANNOTATION_PATH = Path(__file__).resolve().parent / "assets" / "rivian_r1_driver_camera_polygons.json"

LATENCY_WARN_MS = 250.0
LATENCY_TRIP_MS = 500.0
MAX_SLOW_INFERENCES = 3
CPU_TRIP_AVG_PERCENT = 92.0
CPU_TRIP_HOT_CORE_COUNT = 6
CPU_TRIP_SECONDS = 2.0

BUSY_MAX_CPU_PERCENT = 89.0
BUSY_AVG_CPU_PERCENT = 74.0
BUSY_HOT_CORE_COUNT = 4
CRITICAL_SERVICES = ("modelV2", "liveCalibration", "driverMonitoringState", "longitudinalPlan", "livePose")
ESSENTIAL_LOG_ACTIONS = {
  "started", "model_ready", "model_load_failed", "ready", "not_ready",
  "available", "unavailable", "onroad_started", "offroad_started",
  "tripped_for_drive", "resource_skip",
}


def _online_cpu_count() -> int | None:
  # TICI changes the online CPU set with its power state. Never cache this:
  # a value sampled during boot can be wrong for the rest of the drive.
  try:
    spec = Path("/sys/devices/system/cpu/online").read_text(encoding="utf-8").strip()
    count = 0
    for group in spec.split(","):
      bounds = group.split("-", maxsplit=1)
      first, last = int(bounds[0]), int(bounds[-1])
      if last < first:
        return None
      count += last - first + 1
    return count or None
  except (OSError, ValueError):
    return None


def _cpu_topology() -> dict[str, object]:
  topology: dict[str, object] = {"online": "unknown", "frequencies_khz": {}}
  try:
    topology["online"] = Path("/sys/devices/system/cpu/online").read_text(encoding="utf-8").strip()
  except OSError:
    pass
  frequencies = {}
  for cpu_path in sorted(Path("/sys/devices/system/cpu").glob("cpu[0-9]*")):
    freq_path = cpu_path / "cpufreq" / "scaling_cur_freq"
    try:
      frequencies[cpu_path.name] = int(freq_path.read_text(encoding="utf-8").strip())
    except (OSError, ValueError):
      continue
  topology["frequencies_khz"] = frequencies
  return topology


def _memory_snapshot() -> dict[str, float | int]:
  snapshot: dict[str, float | int] = {"rss_mb": 0.0, "threads": 0, "memory_available_mb": 0.0}
  try:
    for line in Path("/proc/self/status").read_text(encoding="utf-8").splitlines():
      if line.startswith("VmRSS:"):
        snapshot["rss_mb"] = round(float(line.split()[1]) / 1024.0, 1)
      elif line.startswith("Threads:"):
        snapshot["threads"] = int(line.split()[1])
  except (OSError, IndexError, TypeError, ValueError):
    pass
  try:
    for line in Path("/proc/meminfo").read_text(encoding="utf-8").splitlines():
      if line.startswith("MemAvailable:"):
        snapshot["memory_available_mb"] = round(float(line.split()[1]) / 1024.0, 1)
        break
  except (OSError, IndexError, TypeError, ValueError):
    pass
  return snapshot


class VisionBSMDaemon:
  """Low-rate PR #75 vision detector. Outputs state only; never sends vehicle controls."""

  def __init__(self):
    from msgq.visionipc import VisionIpcClient, VisionStreamType

    self.params = Params()
    self.params_memory = Params("/dev/shm/params")
    self.sm = messaging.SubMaster(["deviceState", "carState", *CRITICAL_SERVICES])
    self.VisionIpcClient = VisionIpcClient
    self.stream_type = VisionStreamType.VISION_STREAM_DRIVER
    self.client = None
    self.inference = VisionBSMInference()
    self._model_ready = False
    self._model_load_attempted = False
    self._ready = None
    self._available = False
    self._last_ready_heartbeat = 0.0
    self._last_resource_skip_log = 0.0
    self._onroad_since = 0.0
    self._stack_healthy_since = 0.0
    self._last_onroad = False
    self._tripped_for_drive = False
    self._trip_reason = ""

    self.last_inference_at = 0.0
    self.last_inference_by_side = {"left": 0.0, "right": 0.0}
    self.current_side = "left"
    self.followup_until = 0.0
    self.followup_cooldown_until = 0.0
    self._last_param_refresh = 0.0
    self._last_status_log = 0.0
    self._last_error_log = 0.0
    self._inference_count = 0
    self._status_window_started = time.monotonic()
    self._last_latency_ms = 0.0
    self._throttle_factor = 1.0
    self._throttle_filtered = 1.0
    self._throttle_last_time = time.monotonic()
    self._annotation_config = object()
    self._annotation_loaded = False
    self._last_published = (None, None, None, None)
    self._last_update_at = None
    self._last_active = (False, False)
    self._requested_side = ""
    self._slow_inferences = 0
    self._cooldown_count = 0
    self._cpu_overload_since = 0.0

    self._cache_params()
    self._load_annotation_config()
    self._update_ready_state()
    self._publish(False, False, 0.0, 0.0, updated_at=0.0, force=True)
    self._log("started", model_valid=False, annotation_loaded=self._annotation_loaded, ready=False)

  def _cache_params(self) -> None:
    self._enabled = self.params.get_bool("RivianPilotVisionBSMEnabled")
    self._bench_mode = self.params.get_bool("RivianPilotVisionBSMBenchMode")
    self._logging_enabled = self.params.get_bool("RivianPilotFeatureLogging")
    # This AdventurePilot Params binding returns typed values from get(); it
    # intentionally has no get_float() convenience method.
    threshold = float(self.params.get("RivianPilotVisionBSMConfidenceThreshold", return_default=True) or 0.85)
    smoothing = float(self.params.get("RivianPilotVisionBSMSmoothSeconds", return_default=True) or 0.2)
    self._confidence_threshold = min(max(threshold, 0.25), 1.0)
    self._smooth_seconds = min(max(smoothing, 0.1), 0.5)

  def _maybe_refresh_params(self, now: float) -> None:
    if now - self._last_param_refresh < PARAM_REFRESH_INTERVAL:
      return
    self._last_param_refresh = now
    previous_enabled = self._enabled
    self._cache_params()
    config_changed = self._load_annotation_config()
    if config_changed or previous_enabled != self._enabled:
      self._update_ready_state()
    if config_changed:
      self._set_inactive(reset=True)

  def _update_ready_state(self) -> None:
    ready = bool(self._enabled and self._model_ready and self._annotation_loaded)
    if ready == self._ready:
      return
    self._ready = ready
    self.params_memory.put_bool("RivianPilotVisionBSMModelLoaded", ready)
    if not ready:
      self._set_available(False)
    self._log("ready" if ready else "not_ready", annotation_loaded=self._annotation_loaded,
              model_valid=self.inference.valid, model_error=self.inference.last_error)

  def _publish_ready_heartbeat(self, now: float) -> None:
    if not self._available or now - self._last_ready_heartbeat < 1.0:
      return
    self.params_memory.put("RivianPilotVisionBSMReadyHeartbeat", float(now))
    self._last_ready_heartbeat = now

  def _set_available(self, available: bool) -> None:
    available = bool(available and self._ready and not self._tripped_for_drive)
    if available == self._available:
      return
    self._available = available
    now = time.monotonic()
    self.params_memory.put_bool("RivianPilotVisionBSMReady", available)
    self.params_memory.put("RivianPilotVisionBSMReadyAt", float(now) if available else 0.0)
    if not available:
      self.params_memory.put("RivianPilotVisionBSMReadyHeartbeat", 0.0)
    self._log("available" if available else "unavailable")

  def _load_and_warm_model(self, context: str) -> bool:
    if self._model_load_attempted:
      return self._model_ready
    self._model_load_attempted = True
    resources_before = _memory_snapshot()
    load_started = time.monotonic()
    model_loaded = self.inference.load()
    load_ms = (time.monotonic() - load_started) * 1000.0
    warmup_ok, warmup_ms = self.inference.warmup() if model_loaded else (False, 0.0)
    self._model_ready = bool(model_loaded and warmup_ok)
    self._update_ready_state()
    resources_after = _memory_snapshot()
    self._log("model_ready" if self._model_ready else "model_load_failed", context=context,
              load_ms=load_ms, warmup_ms=warmup_ms, model_error=self.inference.last_error,
              backend=self.inference.backend,
              rss_before_mb=resources_before["rss_mb"], rss_after_mb=resources_after["rss_mb"],
              rss_delta_mb=round(float(resources_after["rss_mb"]) - float(resources_before["rss_mb"]), 1),
              threads_before=resources_before["threads"], threads_after=resources_after["threads"],
              memory_available_mb=resources_after["memory_available_mb"],
              cpu_topology=_cpu_topology())
    return self._model_ready

  def _resource_snapshot(self) -> tuple[float, int]:
    usage = self._cpu_usage()
    if not usage:
      return 100.0, len(CRITICAL_SERVICES)
    average = sum(usage) / len(usage)
    hot_cores = sum(value >= BUSY_MAX_CPU_PERCENT for value in usage)
    return average, hot_cores

  def _driving_stack_healthy(self) -> bool:
    for service in CRITICAL_SERVICES:
      if not self.sm.valid.get(service, False):
        return False
      if not self.sm.alive.get(service, False):
        return False
      if not self.sm.freq_ok.get(service, False):
        return False
    return True

  def _resources_allow_inference(self, now: float, require_driving_stack: bool = True) -> bool:
    average, hot_cores = self._resource_snapshot()
    healthy = self._driving_stack_healthy() if require_driving_stack else True
    allowed = healthy and average < BUSY_AVG_CPU_PERCENT and hot_cores < BUSY_HOT_CORE_COUNT
    if not allowed and now - self._last_resource_skip_log >= RESOURCE_SKIP_LOG_INTERVAL:
      self._last_resource_skip_log = now
      self._log("resource_skip", driving_stack_healthy=healthy, cpu_average=average, hot_cores=hot_cores)
    return allowed

  def _trip_for_drive(self, reason: str, **context) -> None:
    if self._tripped_for_drive:
      return
    self._tripped_for_drive = True
    self._trip_reason = reason
    self._set_available(False)
    self._disconnect_camera()
    self._set_inactive(reset=True)
    self._log("tripped_for_drive", reason=reason, **context)

  def _update_onroad_state(self, onroad: bool, now: float) -> None:
    if onroad and not self._last_onroad:
      self._onroad_since = now
      self._tripped_for_drive = False
      self._trip_reason = ""
      self._stack_healthy_since = 0.0
      self._set_available(False)
      self._disconnect_camera()
      # A parked developer test must never cross into a driving session.
      self._bench_mode = False
      self.params.put_bool("RivianPilotVisionBSMBenchMode", False)
      self._log("onroad_started", model_ready=self._model_ready, cpu_topology=_cpu_topology())
    elif not onroad and self._last_onroad:
      self._onroad_since = 0.0
      self._tripped_for_drive = False
      self._trip_reason = ""
      self._stack_healthy_since = 0.0
      self._set_available(False)
      self._disconnect_camera()
      self._set_inactive(reset=True)
      self._log("offroad_started")
    self._last_onroad = onroad

  def _update_stack_stability(self, onroad: bool, now: float) -> bool:
    if not onroad or not self._ready or self._tripped_for_drive:
      self._stack_healthy_since = 0.0
      self._set_available(False)
      return False
    onroad_age = now - self._onroad_since if self._onroad_since else 0.0
    healthy = onroad_age >= ONROAD_STARTUP_DELAY and self._driving_stack_healthy()
    if not healthy:
      if self._available:
        self._trip_for_drive("critical_service_regression")
      else:
        self._stack_healthy_since = 0.0
        self._set_available(False)
      return False
    if self._stack_healthy_since <= 0.0:
      self._stack_healthy_since = now
    stable = now - self._stack_healthy_since >= DRIVING_STACK_STABLE_SECONDS
    self._set_available(stable)
    return stable

  def _maybe_log_status(self, now: float, onroad: bool) -> None:
    if now - self._last_status_log < STATUS_LOG_INTERVAL:
      return
    cpu = self._cpu_usage()
    memory = _memory_snapshot()
    status_elapsed = max(now - self._status_window_started, 0.001)
    timing = self.inference.last_timing_ms
    self._log("status", onroad=onroad, ready=bool(self._ready), model_ready=self._model_ready,
              tripped_for_drive=self._tripped_for_drive, trip_reason=self._trip_reason,
              available=self._available,
              requested_camera_side=self._requested_side or "none",
              inference_count=self._inference_count, latency_ms=self._last_latency_ms,
              effective_fps=self._inference_count / status_elapsed,
              window_copy_ms=timing["window_copy"], color_mask_ms=timing["color_mask"],
              resize_blob_ms=timing["resize_blob"], forward_ms=timing["forward"],
              throttle_factor=self._throttle_factor,
              cpu_average=(sum(cpu) / len(cpu) if cpu else 0.0),
              rss_mb=memory["rss_mb"], process_threads=memory["threads"],
              memory_available_mb=memory["memory_available_mb"],
              cpu_topology=_cpu_topology(),
              left_confidence=self.inference.confidence["left"],
              right_confidence=self.inference.confidence["right"])
    self._inference_count = 0
    self._status_window_started = now
    self._last_status_log = now

  def _load_annotation_config(self) -> bool:
    config = {}
    try:
      raw = self.params.get("RivianPilotVisionBSMAnnotationConfig")
      if isinstance(raw, bytes):
        raw = raw.decode("utf-8")
      config = json.loads(raw) if isinstance(raw, str) and raw else (raw or {})
      if not config and DEFAULT_ANNOTATION_PATH.is_file():
        config = json.loads(DEFAULT_ANNOTATION_PATH.read_text(encoding="utf-8"))
      if not isinstance(config, dict):
        config = {}
      if config == self._annotation_config:
        return False
      self.inference.reset_state()
      has_left = bool(config.get("poly_left"))
      has_right = bool(config.get("poly_right"))
      if has_left or has_right:
        self.inference.load_config(config)
        self._annotation_config = config
        self._annotation_loaded = True
        self._log("annotation_loaded", sides=[side for side, present in (("left", has_left), ("right", has_right)) if present])
        return True
    except (TypeError, ValueError, json.JSONDecodeError) as exc:
      self._log_error("invalid_annotation", exc)
    self._annotation_config = config
    self._annotation_loaded = False
    return True

  def _connect_camera(self) -> bool:
    if self.client is not None and self.client.is_connected():
      return True
    try:
      streams = self.VisionIpcClient.available_streams("camerad", block=False)
    except Exception:
      streams = []
    if self.stream_type not in streams:
      return False
    if self.client is None:
      self.client = self.VisionIpcClient("camerad", self.stream_type, True)
    if not self.client.is_connected():
      self.client.connect(True)
    return self.client.is_connected()

  def _disconnect_camera(self) -> None:
    # Dropping the final Python reference releases this optional VisionIPC
    # client. Reconnect only for an active, single-side blinker request.
    self.client = None

  def _cpu_usage(self) -> list[float]:
    if not self.sm.valid.get("deviceState", False):
      return []
    usage = [float(value) for value in self.sm["deviceState"].cpuUsagePercent]
    online_count = _online_cpu_count()
    return usage[:online_count] if online_count is not None and online_count < len(usage) else usage

  def _cpu_throttle_factor(self) -> float:
    usage = self._cpu_usage()
    if not usage:
      return 1.0
    now = time.monotonic()
    dt = max(0.0, now - self._throttle_last_time)
    self._throttle_last_time = now
    average = sum(usage) / len(usage)
    hot_cores = sum(value >= BUSY_MAX_CPU_PERCENT for value in usage)
    average_factor = 1.0 if average < BUSY_AVG_CPU_PERCENT else 1.0 + (average - BUSY_AVG_CPU_PERCENT) / 8.0
    hot_factor = 1.0 + max(0, hot_cores - BUSY_HOT_CORE_COUNT + 1) * 0.5
    target = min(max(average_factor, hot_factor), 4.0)
    alpha = min(1.0 - math.exp(-0.8 * dt), 1.0)
    self._throttle_filtered = min(max(target * alpha + self._throttle_filtered * (1.0 - alpha), 1.0), 4.0)
    return self._throttle_filtered

  def _inference_interval(self, now: float) -> float:
    base = FOLLOWUP_INTERVAL if now < self.followup_until else BASE_INTERVAL
    self._throttle_factor = self._cpu_throttle_factor()
    return base * self._throttle_factor

  def _candidate_detected(self, side: str) -> bool:
    """Use a sub-threshold candidate only to request brief confirmation samples."""
    confidence = float(self.inference.confidence.get(side, 0.0))
    return confidence >= self._confidence_threshold * CANDIDATE_CONFIDENCE_RATIO

  def _request_followup(self, now: float) -> None:
    """Start one bounded confirmation burst; candidates cannot extend it forever."""
    if now < self.followup_until or now < self.followup_cooldown_until:
      return
    self.followup_until = now + FOLLOWUP_WINDOW
    self.followup_cooldown_until = self.followup_until + FOLLOWUP_COOLDOWN

  def _cpu_guard_tripped(self, usage: list[float], now: float) -> bool:
    """Trip only for sustained system-wide pressure; brief spikes only throttle."""
    if not usage:
      self._cpu_overload_since = 0.0
      return False
    average = sum(usage) / len(usage)
    hot_cores = sum(value >= BUSY_MAX_CPU_PERCENT for value in usage)
    overloaded = average >= CPU_TRIP_AVG_PERCENT or hot_cores >= CPU_TRIP_HOT_CORE_COUNT
    if not overloaded:
      self._cpu_overload_since = 0.0
      return False
    if self._cpu_overload_since <= 0.0:
      self._cpu_overload_since = now
      return False
    if now - self._cpu_overload_since < CPU_TRIP_SECONDS:
      return False

    self._cooldown_count += 1
    self._cpu_overload_since = 0.0
    self._trip_for_drive("cpu_pressure", cpu_average=average, hot_cores=hot_cores,
                         overload_seconds=CPU_TRIP_SECONDS, cooldown_count=self._cooldown_count)
    return True

  def _record_latency(self, latency_ms: float, now: float) -> None:
    """Shed this optional workload after repeated or extreme latency."""
    if latency_ms >= LATENCY_TRIP_MS:
      self._slow_inferences = MAX_SLOW_INFERENCES
    elif latency_ms >= LATENCY_WARN_MS:
      self._slow_inferences += 1
    else:
      self._slow_inferences = max(0, self._slow_inferences - 1)

    if self._slow_inferences >= MAX_SLOW_INFERENCES:
      self._cooldown_count += 1
      self._slow_inferences = 0
      self._trip_for_drive("inference_latency", latency_ms=latency_ms,
                           latency_trip_ms=LATENCY_TRIP_MS,
                           cooldown_count=self._cooldown_count)

  def _set_inactive(self, reset: bool = False) -> None:
    if reset:
      self.inference.reset_state()
      self.followup_until = 0.0
      self.followup_cooldown_until = 0.0
      self.last_inference_at = 0.0
    inactive_values = (False, False, 0.0, 0.0)
    if self._last_update_at != 0.0 or self._last_published != inactive_values:
      self._publish(False, False, 0.0, 0.0, updated_at=0.0, force=True)

  def _publish(self, left_active: bool, right_active: bool, left_confidence: float,
               right_confidence: float, updated_at: float, force: bool = False) -> None:
    # PR #75 camera coordinates are mirrored relative to vehicle/UI sides.
    vehicle_left, vehicle_right = right_active, left_active
    vehicle_left_conf, vehicle_right_conf = right_confidence, left_confidence
    values = (vehicle_left, vehicle_right, round(vehicle_left_conf, 3), round(vehicle_right_conf, 3))
    self._last_update_at = updated_at
    self.params_memory.put("RivianPilotVisionBSMLastUpdateMonoTime", float(updated_at))
    if not force and values == self._last_published:
      return
    self._last_published = values
    self.params_memory.put("RivianPilotVisionBSMLeftActive", bool(vehicle_left))
    self.params_memory.put("RivianPilotVisionBSMRightActive", bool(vehicle_right))
    self.params_memory.put("RivianPilotVisionBSMLeftConfidence", float(vehicle_left_conf))
    self.params_memory.put("RivianPilotVisionBSMRightConfidence", float(vehicle_right_conf))

    active = (vehicle_left, vehicle_right)
    if active != self._last_active:
      action = "detection_started" if any(active) else "detection_cleared"
      self._log(action, left=vehicle_left, right=vehicle_right,
                left_confidence=vehicle_left_conf, right_confidence=vehicle_right_conf,
                latency_ms=self._last_latency_ms, throttle_factor=self._throttle_factor)
      self._last_active = active

  def _log(self, action: str, **kwargs) -> None:
    if not getattr(self, "_logging_enabled", False) and action not in ESSENTIAL_LOG_ACTIONS:
      return
    try:
      cloudlog.event("rivianpilot vision bsm", action=action, **kwargs)
    except Exception:
      pass

  def _log_error(self, reason: str, exc: Exception, **context) -> None:
    now = time.monotonic()
    if now - self._last_error_log < ERROR_LOG_INTERVAL:
      return
    self._last_error_log = now
    try:
      cloudlog.event("rivianpilot feature error", feature="vision_bsmd",
                     errors=[reason], error_type=type(exc).__name__,
                     error_message=str(exc)[:240], **context)
    except Exception:
      pass

  @staticmethod
  def _requested_camera_side(CS) -> str:
    """Map one active vehicle blinker to PR #75's mirrored driver-camera side."""
    left_blinker = bool(CS.leftBlinker)
    right_blinker = bool(CS.rightBlinker)
    if left_blinker == right_blinker:
      return ""
    return "right" if left_blinker else "left"

  @staticmethod
  def _decode_nv12_frame(data, width: int, height: int, stride: int) -> np.ndarray:
    """Return a zero-copy padded NV12 view or reject malformed camera data."""
    if data is None:
      raise ValueError("camera buffer data is missing")
    if width <= 0 or height <= 0 or stride <= 0:
      raise ValueError(f"invalid camera geometry width={width} height={height} stride={stride}")
    if width > stride:
      raise ValueError(f"camera width {width} exceeds stride {stride}")
    if height % 2:
      raise ValueError(f"NV12 camera height must be even: {height}")

    expected_rows = height * 3 // 2
    expected_bytes = expected_rows * stride
    byte_count = len(data)
    if byte_count < expected_bytes:
      raise ValueError(f"short camera buffer bytes={byte_count} expected={expected_bytes}")

    # Keep the VisionIPC row stride and expose a zero-copy view. Inference then
    # copies only the selected side-window rectangle into its reusable buffer.
    flat = np.frombuffer(data, dtype=np.uint8, count=expected_bytes)
    return flat.reshape((expected_rows, stride))

  def run(self) -> None:
    rk = Ratekeeper(10, None)
    while True:
      try:
        now = time.monotonic()
        self._maybe_refresh_params(now)
        self._publish_ready_heartbeat(now)
        self.sm.update(0)
        device_state_valid = self.sm.valid.get("deviceState", False)
        onroad = self.sm["deviceState"].started if device_state_valid else self._last_onroad
        if device_state_valid:
          self._update_onroad_state(onroad, now)
        stack_stable = self._update_stack_stability(onroad, now)
        self._maybe_log_status(now, onroad)

        # Loading and warm-up are intentionally off-road only. Starting the
        # OpenCV graph competes for CPU and memory, so a device that was not
        # prepared before ignition leaves BSM unavailable for the whole drive.
        if self._enabled and not self._model_load_attempted:
          if device_state_valid and not onroad:
            self._load_and_warm_model("offroad")

        active_context = onroad or self._bench_mode
        if (not active_context or not self._enabled or not self._ready or not self.inference.valid or
            not self._annotation_loaded or self._tripped_for_drive):
          self._set_inactive(reset=True)
          rk.keep_time()
          continue

        if onroad and not stack_stable:
          self._set_inactive(reset=True)
          self._disconnect_camera()
          rk.keep_time()
          continue
        requested_side = (self._requested_camera_side(self.sm["carState"])
                          if self.sm.valid.get("carState", False) else "")
        # Bench mode alternates both configured regions without requiring a carState/blinker.
        if self._bench_mode and not onroad:
          sides = self.inference.configured_sides
          requested_side = self.current_side if self.current_side in sides else (sides[0] if sides else "")
        if onroad and requested_side != self._requested_side:
          self._requested_side = requested_side
          self._set_inactive(reset=True)
          self._log("blinker_gate", active=bool(requested_side), camera_side=requested_side or "none")
        if not requested_side:
          self._set_inactive(reset=True)
          self._disconnect_camera()
          rk.keep_time()
          continue
        if not self._connect_camera():
          self._set_inactive(reset=True)
          rk.keep_time()
          continue

        # A sustained overload disables this optional observer for the current
        # drive. Offroad bench mode has no drive to protect, so it simply skips
        # samples while busy and resumes after pressure subsides.
        if onroad and self._cpu_guard_tripped(self._cpu_usage(), now):
          rk.keep_time()
          continue
        if not self._resources_allow_inference(now, require_driving_stack=onroad):
          self._set_inactive(reset=True)
          if onroad:
            self._trip_for_drive("resource_or_service_guard")
          rk.keep_time()
          continue

        interval = self._inference_interval(now)
        if self.last_inference_at and now - self.last_inference_at < interval - 0.015:
          rk.keep_time()
          continue

        buffer = None
        while True:
          latest = self.client.recv(timeout_ms=0)
          if latest is None:
            break
          buffer = latest
        if buffer is None:
          rk.keep_time()
          continue

        sides = self.inference.configured_sides
        if requested_side not in sides:
          self._set_inactive(reset=True)
          rk.keep_time()
          continue
        self.current_side = requested_side
        last_side_at = self.last_inference_by_side[requested_side]
        dt = now - last_side_at if last_side_at else interval
        self.last_inference_at = now
        self.last_inference_by_side[requested_side] = now

        try:
          image = self._decode_nv12_frame(buffer.data, self.client.width, self.client.height, self.client.stride)
        except (TypeError, ValueError) as exc:
          self._log_error("camera_frame_rejected", exc,
                          buffer_bytes=(len(buffer.data) if buffer.data is not None else 0),
                          width=self.client.width, height=self.client.height, stride=self.client.stride)
          self._set_inactive(reset=True)
          rk.keep_time()
          continue
        inference_started = time.monotonic()
        left, right = self.inference.update(image, self.client.width, self.client.height, dt,
                                            self._confidence_threshold, self._smooth_seconds, self.current_side)
        self._last_latency_ms = (time.monotonic() - inference_started) * 1000.0
        self._record_latency(self._last_latency_ms, now)
        if onroad and not self._driving_stack_healthy():
          self._trip_for_drive("post_inference_service_regression",
                               latency_ms=self._last_latency_ms)
        if self._tripped_for_drive:
          rk.keep_time()
          continue
        self._inference_count += 1
        self._publish(left, right, self.inference.confidence["left"], self.inference.confidence["right"], now)
        if self._candidate_detected(requested_side):
          self._request_followup(now)
        if self._bench_mode and not onroad:
          sides = self.inference.configured_sides
          if sides:
            side_index = sides.index(self.current_side)
            self.current_side = sides[(side_index + 1) % len(sides)]
        rk.keep_time()
      except Exception as exc:
        self._log_error("loop_failure_suppressed", exc)
        self._set_inactive(reset=True)
        if getattr(self, "_last_onroad", False):
          self._trip_for_drive("loop_exception", error_type=type(exc).__name__, error_message=str(exc)[:240])
        time.sleep(1.0)


def main() -> None:
  if not PC:
    try:
      os.nice(15)
    except OSError:
      pass
    try:
      # Comma 4 reserves cores 4-7 for controls/planning/cameras/models. Keep
      # this optional single-threaded observer on the noncritical cluster.
      set_core_affinity([0, 1, 2, 3])
    except OSError:
      cloudlog.exception("failed to isolate RivianPilot Vision BSM CPU affinity")
    # Do not use SCHED_IDLE here: camerad may starve an observer completely.
    # nice(15), CPU-cluster isolation, single-threaded OpenCV-DNN, health gates,
    # and the latency trip keep this optional observer subordinate.
  cv2.setNumThreads(1)
  VisionBSMDaemon().run()


if __name__ == "__main__":
  main()
