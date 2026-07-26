import math
import time

from opendbc.car import structs

from openpilot.common.params import Params
from openpilot.common.swaglog import cloudlog


INCH_TO_M = 0.0254
# Rivian lists the R1T at 88.4 inches wide with mirrors open. This controller is
# intentionally R1T-specific.
R1T_WIDTH_WITH_MIRRORS_M = 88.4 * INCH_TO_M
R1T_HALF_WIDTH_M = R1T_WIDTH_WITH_MIRRORS_M / 2.0
BOUNDARY_MARGIN_M = 5.0 * INCH_TO_M
MAX_CUSTOM_OFFSET_M = 10.0 * INCH_TO_M
WIDE_LANE_MIN_M = 3.2
WIDE_LANE_BONUS_M = 2.0 * INCH_TO_M
HUGGING_DIFFERENCE_M = 2.0 * INCH_TO_M
SAMPLE_DISTANCE_M = 20.0
MIN_LANE_PROBABILITY = 0.65
MIN_LANE_WIDTH_M = 2.7
MAX_LANE_WIDTH_M = 4.8
MAX_CURVE_LAT_ACCEL = 3.0
NUDGE_TORQUE_THRESHOLD = 1.0
PARAM_REFRESH_SECONDS = 1.0
UPDATE_PERIOD_SECONDS = 0.1
SAMPLE_LOG_PERIOD_SECONDS = 0.5
CURVE_RELEASE_RATIO = 0.8


class LanePositionController:
  """Compute a bounded temporary model camera offset.

  The controller cannot send steering commands. Go Live only publishes a temporary
  offset consumed by modeld; every invalid or uncertain state publishes zero.
  """

  def __init__(self, params: Params | None = None):
    self.params = params or Params()
    self.observe = False
    self.go_live = False
    self.feature_logging = False
    self.curve_enabled = True
    self.nudge_enabled = True
    self.curve_offset_inches = 3
    self.curve_threshold_pct = 35
    self.nudge_offset_inches = 3
    self.nudge_hold_seconds = 10
    self.last_param_read = 0.0
    self.last_update = 0.0
    self.last_heartbeat = 0.0
    self.last_sample_log = 0.0
    self.last_output = None
    self.nudge_direction = 0
    self.nudge_until = 0.0
    self.pending_nudge_direction = 0
    self.curve_active = False
    self.faulted = False
    self.error_logged = False
    self.diagnostic_faulted = False
    self.diagnostic_error_logged = False
    self.get_params()
    self._publish(0.0)
    self._log("initialized", observe=self.observe, go_live=self.go_live)

  @staticmethod
  def _finite(value) -> float:
    value = float(value)
    if not math.isfinite(value):
      raise ValueError("non-finite lane-position input")
    return value

  def get_params(self) -> None:
    self.observe = self.params.get_bool("RivianPilotLanePositionObserve")
    self.go_live = self.params.get_bool("RivianPilotLanePositionGoLive")
    self.feature_logging = self.params.get_bool("RivianPilotFeatureLogging")
    self.curve_enabled = self.params.get_bool("RivianPilotCurveOffset")
    self.nudge_enabled = self.params.get_bool("RivianPilotNudgeOffset")
    self.curve_offset_inches = max(1, min(10, int(self.params.get("RivianPilotCurveOffsetInches", return_default=True))))
    self.curve_threshold_pct = max(10, min(90, int(self.params.get("RivianPilotCurveThreshold", return_default=True))))
    self.nudge_offset_inches = max(2, min(10, int(self.params.get("RivianPilotNudgeOffsetInches", return_default=True))))
    self.nudge_hold_seconds = max(5, min(60, int(self.params.get("RivianPilotNudgeHoldSeconds", return_default=True))))

  def _log(self, action: str, **kwargs) -> None:
    if not self.feature_logging:
      return
    try:
      cloudlog.event("rivianpilot lane position", action=action, **kwargs)
    except Exception:
      self.feature_logging = False

  def _publish(self, offset_m: float) -> None:
    # Go Live always includes observation. Do not require both toggles or run a
    # second copy of the feature when both are enabled.
    try:
      output = self._finite(offset_m) if self.go_live and not self.faulted else 0.0
      if abs(output) > MAX_CUSTOM_OFFSET_M:
        raise ValueError("custom offset exceeds R1T limit")
      if self.last_output is None or abs(output - self.last_output) >= 0.001:
        self.params.put("RivianPilotDynamicCameraOffset", float(round(output, 4)), block=False)
        self.last_output = output
      now = time.monotonic()
      if now - self.last_heartbeat >= 0.5:
        self.params.put("RivianPilotDynamicCameraOffsetUpdated", float(now), block=False)
        self.last_heartbeat = now
    except Exception:
      # Stop refreshing the heartbeat. modeld rejects the stale custom offset
      # within one second and resumes unmodified core behavior.
      self.faulted = True
      self.last_output = 0.0
      raise

  @staticmethod
  def _sample_geometry(model) -> tuple[float, float, float, float, float, float]:
    probs = list(model.laneLineProbs)
    lines = list(model.laneLines)
    path_x = list(model.position.x)
    path_y = list(model.position.y)
    if len(probs) < 3 or len(lines) < 3 or min(probs[1], probs[2]) < MIN_LANE_PROBABILITY:
      raise ValueError("lane confidence")
    left_x, left_y = list(lines[1].x), list(lines[1].y)
    right_y = list(lines[2].y)
    usable = min(len(left_x), len(left_y), len(right_y))
    if usable == 0 or not path_x or not path_y:
      raise ValueError("lane geometry")
    lane_index = min(range(usable), key=lambda i: abs(float(left_x[i]) - SAMPLE_DISTANCE_M))
    path_index = min(range(min(len(path_x), len(path_y))),
                     key=lambda i: abs(float(path_x[i]) - float(left_x[lane_index])))
    left = float(left_y[lane_index])
    right = float(right_y[lane_index])
    path = float(path_y[path_index])
    if not all(math.isfinite(v) for v in (left, right, path)):
      raise ValueError("non-finite geometry")
    # Live model coordinates increase toward driver-right: laneLines[1] is the
    # current lane's left boundary and laneLines[2] is its right boundary.
    if left >= right:
      raise ValueError("lane orientation")
    width = right - left
    if not MIN_LANE_WIDTH_M <= width <= MAX_LANE_WIDTH_M:
      raise ValueError("implausible lane width")
    driver_left_clearance = path - left - R1T_HALF_WIDTH_M - BOUNDARY_MARGIN_M
    driver_right_clearance = right - path - R1T_HALF_WIDTH_M - BOUNDARY_MARGIN_M
    return (width, path, max(0.0, driver_left_clearance),
            max(0.0, driver_right_clearance), float(probs[1]), float(probs[2]))

  def _sample_geometry_if_authoritative(self, model):
    try:
      return self._sample_geometry(model), "authoritative"
    except ValueError as e:
      # Lane geometry is an optional guard for automatic movement. Weak or
      # missing lane lines never cancel an authoritative manual nudge.
      return None, str(e)

  @staticmethod
  def _diagnostics(CS, model, controls, car_control=None, car_output=None) -> dict:
    """Best-effort diagnostic fields; unavailable inputs never affect control."""
    diagnostics = {}
    try:
      diagnostics["driver_torque"] = round(float(CS.steeringTorque), 3)
      diagnostics["eps_torque"] = round(float(getattr(CS, "steeringTorqueEps", 0.0)), 3)
      diagnostics["steering_angle_deg"] = round(float(getattr(CS, "steeringAngleDeg", 0.0)), 3)
      diagnostics["steering_rate_deg_s"] = round(float(getattr(CS, "steeringRateDeg", 0.0)), 3)
      diagnostics["yaw_rate_rad_s"] = round(float(getattr(CS, "yawRate", 0.0)), 4)
      diagnostics["accel_ms2"] = round(float(getattr(CS, "aEgo", 0.0)), 3)
    except (TypeError, ValueError, OverflowError):
      pass
    try:
      actual_curvature = float(controls.curvature)
      desired_curvature = float(controls.desiredCurvature)
      if math.isfinite(actual_curvature) and math.isfinite(desired_curvature):
        diagnostics["actual_curvature"] = round(actual_curvature, 7)
        diagnostics["desired_curvature"] = round(desired_curvature, 7)
        diagnostics["actual_lateral_accel_ms2"] = round(actual_curvature * float(CS.vEgo) ** 2, 3)
        diagnostics["desired_lateral_accel_ms2"] = round(desired_curvature * float(CS.vEgo) ** 2, 3)
      lateral_state = getattr(controls, "lateralControlState", None)
      controller_type = lateral_state.which() if lateral_state is not None else "unavailable"
      diagnostics["controller_type"] = controller_type
      lateral_log = getattr(lateral_state, controller_type) if lateral_state is not None else None
      diagnostics["controller_saturated"] = bool(getattr(lateral_log, "saturated", False))
      diagnostics["controller_output"] = round(float(getattr(lateral_log, "output", 0.0)), 4)
    except (AttributeError, TypeError, ValueError, OverflowError):
      pass
    try:
      diagnostics["requested_torque"] = round(float(car_control.actuators.torque), 4)
    except (AttributeError, TypeError, ValueError, OverflowError):
      pass
    try:
      diagnostics["applied_torque"] = round(float(car_output.actuatorsOutput.torque), 4)
    except (AttributeError, TypeError, ValueError, OverflowError):
      pass
    try:
      edge_stds = list(getattr(model, "roadEdgeStds", []))
      diagnostics["left_road_edge_std"] = round(float(edge_stds[0]), 3) if len(edge_stds) >= 2 else None
      diagnostics["right_road_edge_std"] = round(float(edge_stds[1]), 3) if len(edge_stds) >= 2 else None
    except (TypeError, ValueError, OverflowError):
      pass
    return diagnostics

  def _safe_diagnostics(self, CS, model, controls, car_control=None, car_output=None) -> dict:
    if self.diagnostic_faulted:
      return {}
    try:
      return self._diagnostics(CS, model, controls, car_control, car_output)
    except Exception as e:
      self.diagnostic_faulted = True
      if not self.diagnostic_error_logged:
        self.diagnostic_error_logged = True
        try:
          cloudlog.event("rivianpilot feature error", feature="lane_position_diagnostics",
                         errors=["diagnostic_failure_suppressed"], error_type=type(e).__name__)
        except Exception:
          pass
      return {}

  def _reset(self, reason: str) -> None:
    had_offset = self.last_output not in (None, 0.0) or self.nudge_direction != 0 or self.curve_active
    self.nudge_direction = 0
    self.nudge_until = 0.0
    self.pending_nudge_direction = 0
    self.curve_active = False
    self._publish(0.0)
    if had_offset:
      self._log("reset", reason=reason)

  def update(self, CS: structs.CarState, lat_active: bool, model, controls, car_control=None,
             car_output=None, now: float | None = None) -> None:
    now = time.monotonic() if now is None else now
    if now - self.last_param_read >= PARAM_REFRESH_SECONDS:
      self.get_params()
      self.last_param_read = now
    if self.faulted or now - self.last_update < UPDATE_PERIOD_SECONDS:
      return
    self.last_update = now

    if not (self.observe or self.go_live):
      self._reset("disabled")
      return
    if CS.gearShifter != structs.CarState.GearShifter.drive or not lat_active:
      self._reset("inactive")
      return
    if CS.leftBlinker or CS.rightBlinker:
      self._reset("blinker")
      return

    torque = self._finite(CS.steeringTorque)
    if CS.steeringPressed:
      self.curve_active = False
      self.nudge_direction = 0
      self.nudge_until = 0.0
      self.pending_nudge_direction = 1 if torque > NUDGE_TORQUE_THRESHOLD else -1 if torque < -NUDGE_TORQUE_THRESHOLD else 0
      self._publish(0.0)
      return
    if self.nudge_enabled and self.pending_nudge_direction:
      self.nudge_direction = self.pending_nudge_direction
      self.nudge_until = now + self.nudge_hold_seconds
      self.pending_nudge_direction = 0
      self._log("nudge_latched", direction=self.nudge_direction, hold_seconds=self.nudge_hold_seconds)

    desired_curvature = self._finite(controls.desiredCurvature)
    lat_accel = desired_curvature * max(self._finite(CS.vEgo), 0.0) ** 2
    curve_strength = min(100.0, abs(lat_accel) / MAX_CURVE_LAT_ACCEL * 100.0)

    requested = 0.0
    source = "none"
    geometry = None
    geometry_status = "not_needed"
    base_offset = 0.0
    wide_lane_bonus = 0.0
    movement_clearance = None
    safety_capped = False
    if self.nudge_direction and now < self.nudge_until:
      # Manual nudge is an explicit driver request. It is never canceled,
      # delayed, or capped by model lane confidence or geometry.
      requested = self.nudge_direction * self.nudge_offset_inches * INCH_TO_M
      applied = requested
      source = "manual_authoritative"
      self.curve_active = False
    else:
      self.nudge_direction = 0
      release_threshold = self.curve_threshold_pct * CURVE_RELEASE_RATIO
      self.curve_active = self.curve_enabled and (
        curve_strength >= (release_threshold if self.curve_active else self.curve_threshold_pct)
      )
      if self.curve_active and lat_accel != 0.0:
        # Positive CameraOffset moves driver-left. Move opposite the curve:
        # positive/left curvature requests negative/driver-right, and vice versa.
        base_offset = self.curve_offset_inches * INCH_TO_M
        requested = -math.copysign(base_offset, lat_accel)
        source = "automatic_curve_guarded"
        geometry, geometry_status = self._sample_geometry_if_authoritative(model)
        if geometry is not None:
          width, path, driver_left_clearance, driver_right_clearance, left_prob, right_prob = geometry
          movement_clearance = driver_left_clearance if requested > 0.0 else driver_right_clearance
          inside_clearance = driver_left_clearance if lat_accel > 0.0 else driver_right_clearance
          outside_clearance = movement_clearance
          hugging_inside = inside_clearance + HUGGING_DIFFERENCE_M < outside_clearance
          if (width >= WIDE_LANE_MIN_M and hugging_inside and
              outside_clearance >= base_offset + WIDE_LANE_BONUS_M):
            wide_lane_bonus = min(WIDE_LANE_BONUS_M, MAX_CUSTOM_OFFSET_M - base_offset)
            requested = math.copysign(base_offset + wide_lane_bonus, requested)
          applied = math.copysign(min(abs(requested), movement_clearance), requested)
          safety_capped = abs(applied) + 1e-6 < abs(requested)
        else:
          # The fixed, bounded curve offset remains available without lane
          # lines; an authoritative target-side boundary can only reduce it.
          applied = requested
      else:
        applied = 0.0

    self._publish(applied)
    diagnostics = self._safe_diagnostics(CS, model, controls, car_control, car_output) if self.feature_logging else {}
    if now - self.last_sample_log >= SAMPLE_LOG_PERIOD_SECONDS:
      lane_fields = {}
      if geometry is not None:
        width, path, driver_left_clearance, driver_right_clearance, left_prob, right_prob = geometry
        lane_fields = {
          "lane_width_m": round(width, 3),
          "path_y_m": round(path, 3),
          "driver_left_clearance_m": round(driver_left_clearance, 3),
          "driver_right_clearance_m": round(driver_right_clearance, 3),
          "left_lane_probability": round(left_prob, 3),
          "right_lane_probability": round(right_prob, 3),
        }
      self._log("sample", source=source, speed_ms=round(float(CS.vEgo), 3),
                turn_direction="left" if lat_accel > 0.0 else "right" if lat_accel < 0.0 else "straight",
                movement_direction="left" if applied > 0.0 else "right" if applied < 0.0 else "none",
                curve_strength_pct=round(curve_strength, 1),
                base_offset_m=round(base_offset, 4), wide_lane_bonus_m=round(wide_lane_bonus, 4),
                requested_offset_m=round(requested, 4),
                movement_clearance_m=round(movement_clearance, 4) if movement_clearance is not None else None,
                safety_capped_offset_m=round(applied, 4), safety_capped=safety_capped,
                published_offset_m=round(self.last_output or 0.0, 4),
                geometry_status=geometry_status, r1t_width_with_mirrors_m=round(R1T_WIDTH_WITH_MIRRORS_M, 4),
                boundary_margin_m=BOUNDARY_MARGIN_M, go_live=self.go_live, observe=self.observe,
                **lane_fields, **diagnostics)
      self.last_sample_log = now

  def suppress_after_error(self, exception: Exception) -> None:
    self.faulted = True
    try:
      self.params.put("RivianPilotDynamicCameraOffset", 0.0, block=False)
      self.params.put("RivianPilotDynamicCameraOffsetUpdated", 0.0, block=False)
      self.last_output = 0.0
    except Exception:
      # A stale heartbeat independently forces modeld back to the core offset.
      pass
    if self.error_logged:
      return
    self.error_logged = True
    try:
      cloudlog.event("rivianpilot feature error", feature="lane_position",
                     errors=["runtime_failure_suppressed"], error_type=type(exception).__name__)
    except Exception:
      pass
