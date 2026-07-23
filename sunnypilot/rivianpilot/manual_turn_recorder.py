import json
import math
import os
import tempfile
import time
import uuid
from collections import deque
from datetime import datetime, timezone

from opendbc.car import structs

from openpilot.common.constants import CV
from openpilot.common.params import Params
from openpilot.common.swaglog import cloudlog


TURN_PACKAGE_SCHEMA_VERSION = 1
DEFAULT_STORAGE_DIR = "/data/rivianpilot/turn_packages"
SAMPLE_PERIOD_SECONDS = 0.2
PRE_ROLL_SECONDS = 3.0
POST_TURN_SECONDS = 3.0
MAX_TURN_SECONDS = 40.0
MIN_HEADING_CHANGE_DEG = 15.0
MAX_SAMPLES = round((PRE_ROLL_SECONDS + MAX_TURN_SECONDS) / SAMPLE_PERIOD_SECONDS)
PRE_ROLL_SAMPLES = round(PRE_ROLL_SECONDS / SAMPLE_PERIOD_SECONDS)


def angle_delta_deg(start: float, end: float) -> float:
  return (end - start + 180.0) % 360.0 - 180.0


def _measurement_value(measurement, index: int, default: float = 0.0) -> float:
  try:
    if not measurement.valid:
      return default
    return float(measurement.value[index])
  except Exception:
    return default


class ManualTurnRecorder:
  """Record bounded, observation-only packages for driver-executed turns."""

  def __init__(self, params: Params | None = None, storage_dir: str = DEFAULT_STORAGE_DIR):
    self.params = params or Params()
    self.storage_dir = storage_dir
    self.enabled = False
    self.feature_logging = False
    self.is_metric = False
    self.max_speed = 35
    self.max_packages = 50
    self.faulted = False
    self.error_logged = False
    self.storage_faulted = False
    self.inventory_initialized = False
    self.last_params_read = 0.0
    self.last_sample_time = 0.0
    self.history = deque(maxlen=PRE_ROLL_SAMPLES)
    self.active = False
    self.samples: list[dict] = []
    self.turn_id = ""
    self.direction = ""
    self.started_at = 0.0
    self.blinker_off_at: float | None = None
    self.manual_steering_seen = False
    self.get_params()

  def _log(self, action: str, **kwargs) -> None:
    if not self.feature_logging:
      return
    try:
      cloudlog.event("rivianpilot manual turn recorder", action=action, **kwargs)
    except Exception:
      self.feature_logging = False

  def _record_error_once(self, error: str, exception: Exception) -> None:
    if self.error_logged:
      return
    self.error_logged = True
    try:
      cloudlog.event("rivianpilot feature error", feature="manual_turn_recorder",
                     errors=[error], error_type=type(exception).__name__)
    except Exception:
      pass

  def suppress_after_error(self, exception: Exception) -> None:
    self.faulted = True
    self.enabled = False
    self._reset()
    self._record_error_once("runtime_failure_suppressed", exception)

  def get_params(self) -> None:
    self.enabled = self.params.get_bool("RivianPilotManualTurnRecorder")
    self.feature_logging = self.params.get_bool("RivianPilotFeatureLogging")
    self.is_metric = self.params.get_bool("IsMetric")
    self.max_speed = self.params.get("RivianPilotManualTurnMaxSpeed", return_default=True)
    self.max_packages = self.params.get("RivianPilotManualTurnMaxPackages", return_default=True)

  def _package_paths(self) -> list[str]:
    try:
      if not os.path.isdir(self.storage_dir):
        return []
      return sorted(os.path.join(self.storage_dir, name) for name in os.listdir(self.storage_dir)
                    if name.endswith(".json") and os.path.isfile(os.path.join(self.storage_dir, name)))
    except Exception as e:
      self.storage_faulted = True
      self._record_error_once("package_list_failure", e)
      return []

  def _delete_path(self, path: str, action: str) -> None:
    try:
      os.unlink(path)
      self._log(action, package_id=os.path.basename(path).removesuffix(".json"))
    except FileNotFoundError:
      pass
    except Exception as e:
      self._record_error_once("package_delete_failure", e)

  def _update_inventory(self, paths: list[str] | None = None) -> None:
    try:
      paths = self._package_paths() if paths is None else paths
      manifest = []
      for index, path in enumerate(paths, start=1):
        with open(path) as f:
          package = json.load(f)
        manifest.append({
          "index": index,
          "package_id": package.get("package_id", os.path.basename(path).removesuffix(".json")),
          "created_utc": package.get("created_utc"),
          "direction": package.get("direction"),
          "confidence_score": package.get("confidence_score"),
          "heading_change_deg": package.get("summary", {}).get("heading_change_deg"),
        })
      self.params.put("RivianPilotManualTurnPackageCount", len(paths), block=False)
      self.params.put("RivianPilotManualTurnPackageManifest", manifest, block=False)
    except Exception as e:
      self._record_error_once("package_inventory_failure", e)

  def _clear_delete_requests(self) -> None:
    for key in ("RivianPilotManualTurnDeleteAll", "RivianPilotManualTurnDeleteOldest",
                "RivianPilotManualTurnDeleteSelected"):
      self.params.put_bool(key, False, block=False)

  def _process_delete_requests(self) -> None:
    paths = self._package_paths()
    delete_all = self.params.get_bool("RivianPilotManualTurnDeleteAll")
    delete_oldest = self.params.get_bool("RivianPilotManualTurnDeleteOldest")
    delete_selected = self.params.get_bool("RivianPilotManualTurnDeleteSelected")
    if not (delete_all or delete_oldest or delete_selected):
      if not self.inventory_initialized:
        self._update_inventory(paths)
        self.inventory_initialized = True
      return

    if delete_all:
      for path in paths:
        self._delete_path(path, "package_deleted")
      self._clear_delete_requests()
      self._update_inventory([])
      self.inventory_initialized = True
      return

    if delete_oldest:
      if paths:
        self._delete_path(paths[0], "package_deleted")

    elif delete_selected:
      package_index = int(self.params.get("RivianPilotManualTurnDeleteIndex", return_default=True))
      if 1 <= package_index <= len(paths):
        self._delete_path(paths[package_index - 1], "package_deleted")
      else:
        self._log("delete_index_not_found", requested_index=package_index, package_count=len(paths))
    self._clear_delete_requests()
    self._update_inventory()
    self.inventory_initialized = True

  @staticmethod
  def _model_geometry(model) -> dict:
    try:
      path_x = list(model.position.x)
      path_y = list(model.position.y)
      lane_lines = model.laneLines
      road_edges = model.roadEdges
      geometry = {}
      for distance in (5.0, 10.0, 20.0):
        path_index = min(range(min(len(path_x), len(path_y))), key=lambda i: abs(float(path_x[i]) - distance))
        key = int(distance)
        geometry[f"path_y_{key}m"] = round(float(path_y[path_index]), 3)
        if len(lane_lines) >= 3:
          left_x, left_y = list(lane_lines[1].x), list(lane_lines[1].y)
          right_x, right_y = list(lane_lines[2].x), list(lane_lines[2].y)
          lane_count = min(len(left_x), len(left_y), len(right_x), len(right_y))
          if lane_count:
            lane_index = min(range(lane_count), key=lambda i: abs(float(left_x[i]) - distance))
            left_lane_y = float(left_y[lane_index])
            right_lane_y = float(right_y[lane_index])
            geometry[f"left_lane_y_{key}m"] = round(left_lane_y, 3)
            geometry[f"right_lane_y_{key}m"] = round(right_lane_y, 3)
            geometry[f"lane_center_y_{key}m"] = round((left_lane_y + right_lane_y) / 2.0, 3)
        if len(road_edges) >= 2:
          edge_x, left_edge_y = list(road_edges[0].x), list(road_edges[0].y)
          right_edge_y = list(road_edges[1].y)
          edge_count = min(len(edge_x), len(left_edge_y), len(right_edge_y))
          if edge_count:
            edge_index = min(range(edge_count), key=lambda i: abs(float(edge_x[i]) - distance))
            geometry[f"left_road_edge_y_{key}m"] = round(float(left_edge_y[edge_index]), 3)
            geometry[f"right_road_edge_y_{key}m"] = round(float(right_edge_y[edge_index]), 3)
      return geometry
    except Exception:
      return {}

  @staticmethod
  def _snapshot(CS: structs.CarState, gps, model, controls_state, car_control, car_output, live_pose,
                now: float) -> dict:
    gps_valid = bool(gps is not None and getattr(gps, "hasFix", False))
    lane_probs = list(getattr(model, "laneLineProbs", [])) if model is not None else []
    actual_curvature = float(getattr(controls_state, "curvature", 0.0))
    desired_curvature = float(getattr(controls_state, "desiredCurvature", 0.0))
    requested_torque = float(getattr(getattr(car_control, "actuators", None), "torque", 0.0))
    applied_torque = float(getattr(getattr(car_output, "actuatorsOutput", None), "torque", 0.0))
    pose_valid = bool(live_pose is not None and getattr(live_pose, "inputsOK", False) and
                      getattr(getattr(live_pose, "orientationNED", None), "valid", False))
    yaw_rad = _measurement_value(getattr(live_pose, "orientationNED", None), 2)
    yaw_rate = _measurement_value(getattr(live_pose, "angularVelocityDevice", None), 2)
    gps_bearing = float(getattr(gps, "bearingDeg", 0.0)) if gps_valid else 0.0
    heading_deg = math.degrees(yaw_rad) % 360.0 if pose_valid else gps_bearing
    try:
      lateral_state = controls_state.lateralControlState
      lateral_log = getattr(lateral_state, lateral_state.which())
      controller_saturated = bool(getattr(lateral_log, "saturated", False))
    except Exception:
      controller_saturated = False

    snapshot = {
      "t": round(now, 3),
      "speed_ms": round(float(CS.vEgo), 3),
      "accel_ms2": round(float(CS.aEgo), 3),
      "gas_pressed": bool(CS.gasPressed),
      "brake_pressed": bool(CS.brakePressed),
      "standstill": bool(CS.standstill),
      "left_blinker": bool(CS.leftBlinker),
      "right_blinker": bool(CS.rightBlinker),
      "left_blindspot": bool(CS.leftBlindspot),
      "right_blindspot": bool(CS.rightBlindspot),
      "steering_pressed": bool(CS.steeringPressed),
      "driver_torque": round(float(CS.steeringTorque), 3),
      "eps_torque": round(float(getattr(CS, "steeringTorqueEps", 0.0)), 3),
      "steering_angle_deg": round(float(CS.steeringAngleDeg), 3),
      "steering_rate_deg_s": round(float(CS.steeringRateDeg), 3),
      "actual_curvature": round(actual_curvature, 6),
      "desired_curvature": round(desired_curvature, 6),
      "actual_lateral_accel_ms2": round(actual_curvature * float(CS.vEgo) ** 2, 3),
      "desired_lateral_accel_ms2": round(desired_curvature * float(CS.vEgo) ** 2, 3),
      "requested_torque": round(requested_torque, 4),
      "applied_torque": round(applied_torque, 4),
      "lateral_active": bool(getattr(car_control, "latActive", False)),
      "controller_saturated": controller_saturated,
      "gps_valid": gps_valid,
      "latitude": round(float(getattr(gps, "latitude", 0.0)), 7) if gps_valid else None,
      "longitude": round(float(getattr(gps, "longitude", 0.0)), 7) if gps_valid else None,
      "gps_bearing_deg": round(gps_bearing, 2) if gps_valid else None,
      "heading_deg": round(heading_deg, 2),
      "heading_source": "live_pose" if pose_valid else "gps",
      "pose_valid": pose_valid,
      "yaw_rad": round(yaw_rad, 5),
      "yaw_rate_rad_s": round(yaw_rate, 5),
      "left_lane_probability": round(float(lane_probs[1]), 3) if len(lane_probs) >= 3 else None,
      "right_lane_probability": round(float(lane_probs[2]), 3) if len(lane_probs) >= 3 else None,
      "left_road_edge_std": round(float(model.roadEdgeStds[0]), 3) if model is not None and len(model.roadEdgeStds) >= 2 else None,
      "right_road_edge_std": round(float(model.roadEdgeStds[1]), 3) if model is not None and len(model.roadEdgeStds) >= 2 else None,
    }
    snapshot.update(ManualTurnRecorder._model_geometry(model))
    return snapshot

  @staticmethod
  def _confidence(samples: list[dict], manual_steering_seen: bool, settled: bool) -> tuple[int, dict]:
    count = max(len(samples), 1)
    gps_ratio = sum(bool(s["gps_valid"]) for s in samples) / count
    pose_ratio = sum(bool(s["pose_valid"]) for s in samples) / count
    model_samples = [s for s in samples if s["left_lane_probability"] is not None and s["right_lane_probability"] is not None]
    model_ratio = len(model_samples) / count
    start_heading = next((s["heading_deg"] for s in samples if s["gps_valid"] or s["pose_valid"]), 0.0)
    end_heading = next((s["heading_deg"] for s in reversed(samples) if s["gps_valid"] or s["pose_valid"]), start_heading)
    heading_change = abs(angle_delta_deg(start_heading, end_heading))
    duration = max(samples[-1]["t"] - samples[0]["t"], 0.0) if samples else 0.0
    components = {
      "gps_coverage": round(20.0 * gps_ratio, 1),
      "pose_coverage": round(20.0 * pose_ratio, 1),
      "manual_steering": 15.0 if manual_steering_seen else 0.0,
      "heading_change": round(15.0 * min(heading_change / 60.0, 1.0), 1),
      "duration_coverage": round(10.0 * min(duration / 8.0, 1.0), 1),
      "model_coverage": round(10.0 * model_ratio, 1),
      "settled_completion": 10.0 if settled else 0.0,
    }
    return int(round(min(sum(components.values()), 100.0))), components

  def _write_package(self, package: dict) -> None:
    if self.storage_faulted:
      return
    try:
      os.makedirs(self.storage_dir, mode=0o700, exist_ok=True)
      with tempfile.NamedTemporaryFile("w", dir=self.storage_dir, prefix=".turn-", suffix=".tmp", delete=False) as f:
        json.dump(package, f, separators=(",", ":"), allow_nan=False)
        temp_path = f.name
      final_path = os.path.join(self.storage_dir, f"{package['package_id']}.json")
      os.replace(temp_path, final_path)
      paths = self._package_paths()
      for path in paths[:-max(int(self.max_packages), 1)]:
        self._delete_path(path, "package_pruned")
      self._update_inventory(self._package_paths())
    except Exception as e:
      self.storage_faulted = True
      self._record_error_once("package_write_failure", e)

  def _reset(self) -> None:
    self.active = False
    self.samples = []
    self.turn_id = ""
    self.direction = ""
    self.started_at = 0.0
    self.blinker_off_at = None
    self.manual_steering_seen = False

  def _finalize(self, now: float, settled: bool, reason: str) -> None:
    if not self.samples:
      self._reset()
      return
    valid_headings = [s["heading_deg"] for s in self.samples if s["gps_valid"] or s["pose_valid"]]
    heading_change = angle_delta_deg(valid_headings[0], valid_headings[-1]) if len(valid_headings) >= 2 else 0.0
    qualifies = self.manual_steering_seen and abs(heading_change) >= MIN_HEADING_CHANGE_DEG
    if not qualifies:
      self._log("package_discarded", package_id=self.turn_id, reason="insufficient_manual_turn",
                heading_change_deg=round(heading_change, 1), samples=len(self.samples))
      self._reset()
      return

    confidence, components = self._confidence(self.samples, self.manual_steering_seen, settled)
    package = {
      "schema_version": TURN_PACKAGE_SCHEMA_VERSION,
      "package_id": self.turn_id,
      "created_utc": datetime.fromtimestamp(self.started_at, timezone.utc).isoformat(),
      "direction": self.direction,
      "confidence_score": confidence,
      "confidence_type": "recording_quality_not_automation_safety",
      "confidence_components": components,
      "summary": {
        "completion_reason": reason,
        "settled_completion": settled,
        "sample_count": len(self.samples),
        "duration_s": round(self.samples[-1]["t"] - self.samples[0]["t"], 2),
        "heading_change_deg": round(heading_change, 2),
        "max_speed_ms": max(s["speed_ms"] for s in self.samples),
        "max_abs_steering_angle_deg": max(abs(s["steering_angle_deg"]) for s in self.samples),
        "max_abs_driver_torque": max(abs(s["driver_torque"]) for s in self.samples),
      },
      "samples": self.samples,
    }
    self._write_package(package)
    if not self.storage_faulted:
      self._log("package_recorded", package_id=self.turn_id, direction=self.direction,
                confidence_score=confidence, heading_change_deg=round(heading_change, 1),
                samples=len(self.samples))
    self._reset()

  def update(self, CS: structs.CarState, gps, model=None, controls_state=None, car_control=None, car_output=None,
             live_pose=None, now: float | None = None) -> None:
    if self.faulted:
      return
    now = time.time() if now is None else now
    monotonic_now = time.monotonic()
    if monotonic_now - self.last_params_read > 10.0:
      self.get_params()
      self._process_delete_requests()
      self.last_params_read = monotonic_now

    if now - self.last_sample_time < SAMPLE_PERIOD_SECONDS:
      return
    self.last_sample_time = now
    sample = self._snapshot(CS, gps, model, controls_state, car_control, car_output, live_pose, now)
    self.history.append(sample)

    if not self.enabled:
      if self.active:
        self._finalize(now, False, "disabled")
      return

    exactly_one_blinker = bool(CS.leftBlinker) != bool(CS.rightBlinker)
    speed_factor = CV.KPH_TO_MS if self.is_metric else CV.MPH_TO_MS
    in_drive = CS.gearShifter == structs.CarState.GearShifter.drive
    under_speed = CS.vEgo <= float(self.max_speed) * speed_factor

    if not self.active:
      if exactly_one_blinker and in_drive and under_speed:
        self.active = True
        self.started_at = now
        self.direction = "left" if CS.leftBlinker else "right"
        self.turn_id = f"{int(now * 1000):013d}-{self.direction}-{uuid.uuid4().hex[:8]}"
        self.samples = list(self.history)
        self.manual_steering_seen = bool(CS.steeringPressed)
        self._log("recording_started", package_id=self.turn_id, direction=self.direction)
      return

    if len(self.samples) < MAX_SAMPLES:
      self.samples.append(sample)
    self.manual_steering_seen |= bool(CS.steeringPressed)

    if not in_drive:
      self._finalize(now, False, "left_drive")
    elif now - self.started_at >= MAX_TURN_SECONDS or len(self.samples) >= MAX_SAMPLES:
      self._finalize(now, False, "duration_limit")
    elif exactly_one_blinker:
      self.blinker_off_at = None
    else:
      if self.blinker_off_at is None:
        self.blinker_off_at = now
      elif now - self.blinker_off_at >= POST_TURN_SECONDS:
        self._finalize(now, True, "post_turn_complete")
