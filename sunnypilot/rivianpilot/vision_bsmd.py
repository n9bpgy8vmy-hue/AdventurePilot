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

from functools import lru_cache
from pathlib import Path
import time

# This import installs the tracked OpenCV vendor directory before cv2 is loaded.
from openpilot.sunnypilot.rivianpilot.vision_bsm_inference import VisionBSMInference

import cv2
import numpy as np

import cereal.messaging as messaging
from cereal import car

from openpilot.common.params import Params
from openpilot.common.realtime import Ratekeeper, set_core_affinity
from openpilot.common.swaglog import cloudlog
from openpilot.system.hardware import PC
from openpilot.sunnypilot.rivianpilot.vision_bsm import memory_params


BASE_INTERVAL = 0.500
FOLLOWUP_INTERVAL = 0.200
FOLLOWUP_WINDOW = 1.5
PARAM_REFRESH_INTERVAL = 2.0
STATUS_LOG_INTERVAL = 30.0
ERROR_LOG_INTERVAL = 30.0
AFFINITY_CORES = [0, 1, 2]

BUSY_MAX_CPU_PERCENT = 89.0
BUSY_AVG_CPU_PERCENT = 74.0
BUSY_HOT_CORE_COUNT = 4


@lru_cache(maxsize=1)
def _online_cpu_count() -> int | None:
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


class VisionBSMDaemon:
  """Low-rate PR #75 vision detector. Outputs state only; never sends vehicle controls."""

  def __init__(self):
    from msgq.visionipc import VisionIpcClient, VisionStreamType

    self.params = Params()
    self.params_memory = memory_params()
    self.sm = messaging.SubMaster(["deviceState", "carState"])
    self.VisionIpcClient = VisionIpcClient
    self.stream_type = VisionStreamType.VISION_STREAM_DRIVER
    self.client = None
    self.inference = VisionBSMInference()
    self.inference.load()

    self.last_inference_at = 0.0
    self.last_inference_by_side = {"left": 0.0, "right": 0.0}
    self.current_side = "left"
    self.followup_until = 0.0
    self._last_param_refresh = 0.0
    self._last_status_log = 0.0
    self._last_error_log = 0.0
    self._inference_count = 0
    self._last_latency_ms = 0.0
    self._throttle_factor = 1.0
    self._throttle_filtered = 1.0
    self._throttle_last_time = time.monotonic()
    self._annotation_config = object()
    self._annotation_loaded = False
    self._last_published = (None, None, None, None)
    self._last_update_at = None
    self._last_active = (False, False)
    self._affinity_set = False

    self._cache_params()
    self._load_annotation_config()
    self._publish(False, False, 0.0, 0.0, updated_at=0.0, force=True)
    self._log("started", model_valid=self.inference.valid, model_error=self.inference.last_error,
              annotation_loaded=self._annotation_loaded)

  def _cache_params(self) -> None:
    self._enabled = self.params.get_bool("RivianPilotVisionBSMEnabled")
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
    self._cache_params()
    if self._load_annotation_config():
      self._set_inactive(reset=True)

  def _load_annotation_config(self) -> bool:
    config = {}
    try:
      raw = self.params.get("RivianPilotVisionBSMAnnotationConfig")
      if isinstance(raw, bytes):
        raw = raw.decode("utf-8")
      config = json.loads(raw) if isinstance(raw, str) and raw else (raw or {})
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

  def _set_inactive(self, reset: bool = False) -> None:
    if reset:
      self.inference.reset_state()
      self.followup_until = 0.0
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
    if not getattr(self, "_logging_enabled", False) and action != "started":
      return
    try:
      cloudlog.event("rivianpilot vision bsm", action=action, **kwargs)
    except Exception:
      pass

  def _log_error(self, reason: str, exc: Exception) -> None:
    now = time.monotonic()
    if now - self._last_error_log < ERROR_LOG_INTERVAL:
      return
    self._last_error_log = now
    try:
      cloudlog.event("rivianpilot feature error", feature="vision_bsmd",
                     errors=[reason], error_type=type(exc).__name__)
    except Exception:
      pass

  def run(self) -> None:
    rk = Ratekeeper(10, None)
    while True:
      try:
        now = time.monotonic()
        self._maybe_refresh_params(now)
        self.sm.update(0)
        if not PC and not self._affinity_set:
          set_core_affinity(AFFINITY_CORES)
          self._affinity_set = True

        onroad = self.sm["deviceState"].started if self.sm.valid.get("deviceState", False) else False
        parked = (self.sm.valid.get("carState", False) and
                  self.sm["carState"].gearShifter == car.CarState.GearShifter.park)
        if not onroad or parked or not self._enabled or not self.inference.valid or not self._annotation_loaded:
          self._set_inactive(reset=True)
          rk.keep_time()
          continue
        if not self._connect_camera():
          self._set_inactive(reset=True)
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
        if not sides:
          self._set_inactive(reset=True)
          rk.keep_time()
          continue
        if self.current_side not in sides:
          self.current_side = sides[0]
        last_side_at = self.last_inference_by_side[self.current_side]
        dt = now - last_side_at if last_side_at else interval
        self.last_inference_at = now
        self.last_inference_by_side[self.current_side] = now

        image = np.frombuffer(buffer.data, dtype=np.uint8).reshape((len(buffer.data) // self.client.stride, self.client.stride))
        if self.client.stride != self.client.width:
          image = image[:, :self.client.width]
        inference_started = time.monotonic()
        left, right = self.inference.update(image, self.client.width, self.client.height, dt,
                                            self._confidence_threshold, self._smooth_seconds, self.current_side)
        self._last_latency_ms = (time.monotonic() - inference_started) * 1000.0
        self._inference_count += 1
        self._publish(left, right, self.inference.confidence["left"], self.inference.confidence["right"], now)
        if left or right:
          self.followup_until = now + FOLLOWUP_WINDOW
        side_index = sides.index(self.current_side)
        self.current_side = sides[(side_index + 1) % len(sides)]

        if now - self._last_status_log >= STATUS_LOG_INTERVAL:
          cpu = self._cpu_usage()
          self._log("status", inference_count=self._inference_count, latency_ms=self._last_latency_ms,
                    throttle_factor=self._throttle_factor,
                    cpu_average=(sum(cpu) / len(cpu) if cpu else 0.0),
                    left_confidence=self.inference.confidence["left"],
                    right_confidence=self.inference.confidence["right"])
          self._inference_count = 0
          self._last_status_log = now
        rk.keep_time()
      except Exception as exc:
        self._log_error("loop_failure_suppressed", exc)
        self._set_inactive(reset=True)
        time.sleep(1.0)


def main() -> None:
  if not PC:
    try:
      os.nice(10)
    except OSError:
      pass
  cv2.setNumThreads(1)
  VisionBSMDaemon().run()


if __name__ == "__main__":
  main()
