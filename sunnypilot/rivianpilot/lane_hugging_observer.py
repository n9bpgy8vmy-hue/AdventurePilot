import json
import math
import time

from opendbc.car import structs

from openpilot.common.constants import CV
from openpilot.common.params import Params
from openpilot.common.realtime import DT_CTRL
from openpilot.common.swaglog import cloudlog


MAX_LOCATIONS = 200
MERGE_DISTANCE_METERS = 30.0
CORRECTION_DURATION_SECONDS = 0.5
APPROACH_CHECK_PERIOD_SECONDS = 0.5
NEW_LOCATION_ALERT_SUPPRESSION_SECONDS = 60.0
POINT_ALERT_COOLDOWN_SECONDS = 60.0
CURVE_SAMPLE_PERIOD_SECONDS = 2.0
CURVE_MIN_LATERAL_ACCEL_MS2 = 0.5
LANE_SAMPLE_DISTANCE_METERS = 20.0


def distance_meters(lat1: float, lon1: float, lat2: float, lon2: float) -> float:
  radius = 6_371_000.0
  phi1, phi2 = math.radians(lat1), math.radians(lat2)
  d_phi = math.radians(lat2 - lat1)
  d_lambda = math.radians(lon2 - lon1)
  a = math.sin(d_phi / 2) ** 2 + math.cos(phi1) * math.cos(phi2) * math.sin(d_lambda / 2) ** 2
  return radius * 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))


class LaneHuggingObserver:
  """Observe driver lateral corrections and alert near previously observed locations.

  This class is deliberately non-actuating: it never changes steering, lateral-active, or MADS state.
  """

  def __init__(self, params: Params | None = None):
    self.params = params or Params()
    self.enabled = False
    self.feature_logging = False
    self.is_metric = False
    self.min_speed = 25
    self.alert_distance = 150
    self.faulted = False
    self.error_logged = False
    self.storage_faulted = False
    self.storage_error_logged = False
    self.diagnostic_error_logged = False
    self.locations = self._load_locations()
    self.correction_ticks = 0
    self.correction_latched = False
    self.last_params_read = 0.0
    self.last_approach_check = 0.0
    self.last_curve_sample = 0.0
    self.alerted_points: dict[str, float] = {}
    self.get_params()

  def _load_locations(self) -> list[dict]:
    try:
      raw = self.params.get("RivianPilotLaneHuggingLocations")
      if isinstance(raw, (str, bytes)):
        locations = json.loads(raw)
      else:
        locations = raw if raw else []
      return locations[-MAX_LOCATIONS:] if isinstance(locations, list) else []
    except Exception as e:
      self._record_error_once("invalid_location_database", e)
      return []

  def _record_error_once(self, error: str, exception: Exception) -> None:
    if getattr(self, "storage_error_logged", False):
      return
    self.storage_error_logged = True
    try:
      cloudlog.event(
        "rivianpilot feature error",
        feature="lane_hugging_observer",
        errors=[error],
        error_type=type(exception).__name__,
      )
    except Exception:
      # Logging must never be able to terminate selfdrived.
      pass

  def get_params(self) -> None:
    self.enabled = self.params.get_bool("RivianPilotLaneHuggingObserver")
    self.feature_logging = self.params.get_bool("RivianPilotFeatureLogging")
    self.is_metric = self.params.get_bool("IsMetric")
    self.min_speed = self.params.get("RivianPilotLaneHuggingMinSpeed", return_default=True)
    self.alert_distance = self.params.get("RivianPilotLaneHuggingAlertDistance", return_default=True)

  def _log(self, action: str, **kwargs) -> None:
    if self.feature_logging:
      try:
        cloudlog.event("rivianpilot lane hugging observer", action=action, **kwargs)
      except Exception:
        # Optional diagnostics must never affect selfdrived.
        self.feature_logging = False

  def _suppress_diagnostics_after_error(self, exception: Exception) -> None:
    """Disable optional rich logging without disabling location observation or alerts."""
    self.feature_logging = False
    if self.diagnostic_error_logged:
      return
    self.diagnostic_error_logged = True
    try:
      cloudlog.event(
        "rivianpilot feature error",
        feature="lane_hugging_observer_diagnostics",
        errors=["diagnostic_snapshot_failure_suppressed"],
        error_type=type(exception).__name__,
      )
    except Exception:
      pass

  @staticmethod
  def _model_snapshot(model, controls_state, speed_ms: float, CS=None,
                      car_control=None, car_output=None) -> dict:
    """Return a compact, defensive lane/curve snapshot for offline analysis."""
    snapshot = {
      "actual_curvature": round(float(getattr(controls_state, "curvature", 0.0)), 6),
      "desired_curvature": round(float(getattr(controls_state, "desiredCurvature", 0.0)), 6),
    }
    actual_lateral_accel = snapshot["actual_curvature"] * speed_ms ** 2
    desired_lateral_accel = snapshot["desired_curvature"] * speed_ms ** 2
    snapshot.update({
      "actual_lateral_accel_ms2": round(actual_lateral_accel, 3),
      "desired_lateral_accel_ms2": round(desired_lateral_accel, 3),
      "curvature_tracking_error": round(snapshot["desired_curvature"] - snapshot["actual_curvature"], 6),
      "curve_direction": "left" if desired_lateral_accel > 0 else "right" if desired_lateral_accel < 0 else "straight",
    })

    if CS is not None:
      snapshot.update({
        "driver_steering_torque": round(float(getattr(CS, "steeringTorque", 0.0)), 3),
        "eps_steering_torque": round(float(getattr(CS, "steeringTorqueEps", 0.0)), 3),
        "steering_angle_deg": round(float(getattr(CS, "steeringAngleDeg", 0.0)), 3),
        "steering_rate_deg_s": round(float(getattr(CS, "steeringRateDeg", 0.0)), 3),
        "steering_pressed": bool(getattr(CS, "steeringPressed", False)),
      })

    requested_torque = float(getattr(getattr(car_control, "actuators", None), "torque", 0.0))
    applied_torque = float(getattr(getattr(car_output, "actuatorsOutput", None), "torque", 0.0))
    snapshot.update({
      "requested_steering_torque": round(requested_torque, 4),
      "applied_steering_torque": round(applied_torque, 4),
      "steering_torque_tracking_error": round(requested_torque - applied_torque, 4),
    })
    try:
      lateral_state = controls_state.lateralControlState
      lateral_log = getattr(lateral_state, lateral_state.which())
      snapshot["lateral_controller_saturated"] = bool(getattr(lateral_log, "saturated", False))
      snapshot["lateral_controller_output"] = round(float(getattr(lateral_log, "output", requested_torque)), 4)
    except Exception:
      # Controller unions differ by lateral-control type; missing diagnostics are non-fatal.
      snapshot["lateral_controller_saturated"] = False

    if model is None:
      return snapshot

    lane_lines = getattr(model, "laneLines", [])
    lane_probs = getattr(model, "laneLineProbs", [])
    path_x = list(getattr(getattr(model, "position", None), "x", []))
    path_y = list(getattr(getattr(model, "position", None), "y", []))
    if len(lane_probs) >= 3:
      snapshot["left_lane_probability"] = round(float(lane_probs[1]), 3)
      snapshot["right_lane_probability"] = round(float(lane_probs[2]), 3)

    if len(lane_lines) < 3:
      return snapshot
    left_x = list(getattr(lane_lines[1], "x", []))
    left_y = list(getattr(lane_lines[1], "y", []))
    right_y = list(getattr(lane_lines[2], "y", []))
    usable = min(len(left_x), len(left_y), len(right_y))
    if usable == 0:
      return snapshot

    sample_index = min(range(usable), key=lambda i: abs(float(left_x[i]) - LANE_SAMPLE_DISTANCE_METERS))
    left_lane_y = float(left_y[sample_index])
    right_lane_y = float(right_y[sample_index])
    lane_center_y = (left_lane_y + right_lane_y) / 2.0
    snapshot.update({
      "sample_distance_m": round(float(left_x[sample_index]), 1),
      "left_lane_y_m": round(left_lane_y, 3),
      "right_lane_y_m": round(right_lane_y, 3),
      "lane_width_m": round(abs(right_lane_y - left_lane_y), 3),
      "lane_center_y_m": round(lane_center_y, 3),
    })

    path_usable = min(len(path_x), len(path_y))
    if path_usable:
      path_index = min(range(path_usable), key=lambda i: abs(float(path_x[i]) - float(left_x[sample_index])))
      predicted_path_y = float(path_y[path_index])
      snapshot["predicted_path_y_m"] = round(predicted_path_y, 3)
      snapshot["predicted_path_from_lane_center_m"] = round(predicted_path_y - lane_center_y, 3)
    return snapshot

  def _log_curve_sample(self, CS: structs.CarState, gps, model, controls_state, car_control, car_output,
                        now: float) -> None:
    if not self.feature_logging or now - self.last_curve_sample < CURVE_SAMPLE_PERIOD_SECONDS:
      return
    try:
      snapshot = self._model_snapshot(model, controls_state, CS.vEgo, CS, car_control, car_output)
      if abs(snapshot["desired_lateral_accel_ms2"]) < CURVE_MIN_LATERAL_ACCEL_MS2:
        return
      self.last_curve_sample = now
      self._log("curve_sample", latitude=round(float(gps.latitude), 7), longitude=round(float(gps.longitude), 7),
                speed_ms=round(float(CS.vEgo), 3), **snapshot)
    except Exception as e:
      self._suppress_diagnostics_after_error(e)

  def suppress_after_error(self, exception: Exception) -> None:
    """Disable the observer for this process after an unexpected runtime failure."""
    self.faulted = True
    self.enabled = False
    self.correction_ticks = 0
    self.correction_latched = False
    if self.error_logged:
      return
    self.error_logged = True
    try:
      cloudlog.event(
        "rivianpilot feature error",
        feature="lane_hugging_observer",
        errors=["runtime_failure_suppressed"],
        error_type=type(exception).__name__,
      )
    except Exception:
      pass

  def _save_locations(self) -> None:
    if self.storage_faulted:
      return
    self.locations = sorted(self.locations, key=lambda point: point.get("last_seen", 0))[-MAX_LOCATIONS:]
    try:
      # JSON Params accept native Python JSON values; serializing first causes a type mismatch.
      self.params.put("RivianPilotLaneHuggingLocations", self.locations, block=False)
    except Exception as e:
      self.storage_faulted = True
      self._record_error_once("location_save_failed", e)

  def _record_correction(self, latitude: float, longitude: float, direction: str, now: float, CS: structs.CarState,
                         model, controls_state, car_control, car_output) -> None:
    nearest = None
    nearest_distance = MERGE_DISTANCE_METERS
    for point in self.locations:
      if point.get("direction") != direction:
        continue
      distance = distance_meters(latitude, longitude, point["latitude"], point["longitude"])
      if distance <= nearest_distance:
        nearest = point
        nearest_distance = distance

    if nearest is None:
      nearest = {
        "id": f"{latitude:.6f},{longitude:.6f},{direction}",
        "latitude": round(latitude, 7),
        "longitude": round(longitude, 7),
        "direction": direction,
        "count": 0,
        "first_seen": now,
      }
      self.locations.append(nearest)

    nearest["count"] = int(nearest.get("count", 0)) + 1
    nearest["last_seen"] = now
    self._save_locations()
    if not self.storage_faulted and self.feature_logging:
      try:
        snapshot = self._model_snapshot(model, controls_state, CS.vEgo, CS, car_control, car_output)
        self._log("correction_recorded", direction=direction, latitude=latitude, longitude=longitude,
                  speed_ms=round(float(CS.vEgo), 3), observations=nearest["count"], **snapshot)
      except Exception as e:
        self._suppress_diagnostics_after_error(e)

  def _approach_alert(self, latitude: float, longitude: float, now: float) -> str | None:
    if now - self.last_approach_check < APPROACH_CHECK_PERIOD_SECONDS:
      return None
    self.last_approach_check = now

    nearest = None
    nearest_distance = float(self.alert_distance)
    for point in self.locations:
      if now - point.get("last_seen", 0) < NEW_LOCATION_ALERT_SUPPRESSION_SECONDS:
        continue
      distance = distance_meters(latitude, longitude, point["latitude"], point["longitude"])
      if distance <= nearest_distance:
        nearest = point
        nearest_distance = distance

    if nearest is None:
      return None

    point_id = nearest.get("id", f"{nearest['latitude']},{nearest['longitude']},{nearest['direction']}")
    if now - self.alerted_points.get(point_id, 0) < POINT_ALERT_COOLDOWN_SECONDS:
      return None

    self.alerted_points[point_id] = now
    self._log("approach_alert", direction=nearest["direction"], distance_m=round(nearest_distance, 1),
              observations=nearest.get("count", 1))
    return nearest["direction"]

  def update(self, CS: structs.CarState, lateral_active: bool, gps, model=None, controls_state=None,
             car_control=None, car_output=None, now: float | None = None) -> str | None:
    if self.faulted:
      return None
    now = time.time() if now is None else now
    monotonic_now = time.monotonic()
    if monotonic_now - self.last_params_read > 10.0:
      self.get_params()
      self.last_params_read = monotonic_now

    if not self.enabled or gps is None or not gps.hasFix:
      self.correction_ticks = 0
      self.correction_latched = False
      return None

    speed_factor = CV.KPH_TO_MS if self.is_metric else CV.MPH_TO_MS
    self._log_curve_sample(CS, gps, model, controls_state, car_control, car_output, now)
    correction_candidate = lateral_active and CS.steeringPressed and not (CS.leftBlinker or CS.rightBlinker) and \
                           CS.vEgo >= self.min_speed * speed_factor

    if correction_candidate:
      self.correction_ticks += 1
      if not self.correction_latched and self.correction_ticks * DT_CTRL >= CORRECTION_DURATION_SECONDS:
        direction = "left" if CS.steeringTorque > 0 else "right"
        self._record_correction(gps.latitude, gps.longitude, direction, now, CS, model, controls_state,
                                car_control, car_output)
        self.correction_latched = True
    elif not CS.steeringPressed:
      self.correction_ticks = 0
      self.correction_latched = False

    return self._approach_alert(gps.latitude, gps.longitude, now)
