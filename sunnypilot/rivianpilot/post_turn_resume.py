from __future__ import annotations

import math
import time
from enum import IntEnum

from cereal import log
from opendbc.car import structs

from openpilot.common.constants import CV
from openpilot.common.realtime import DT_CTRL
from openpilot.common.swaglog import cloudlog


class PostTurnAction(IntEnum):
  none = 0
  pause = 1
  waiting = 2
  resume = 3


class PostTurnResume:
  """Conservative Rivian MADS pause/resume gate for signaled low-speed turns."""

  LANE_PROB_MIN = 0.5
  STEERING_ANGLE_MAX_DEG = 15.0
  YAW_RATE_MAX_RADS = 0.08
  PAUSE_CONFIRMATION_SECONDS = 0.5
  HIGH_SPEED_STABLE_SECONDS = 0.5
  ROAD_EDGE_STD_MAX = 0.6
  ROAD_WIDTH_MIN_M = 2.5
  ROAD_WIDTH_MAX_M = 6.0
  ROAD_EDGE_MARGIN_M = 0.5
  PATH_CENTER_TOLERANCE_M = 1.5
  ROAD_EDGE_SAMPLE_INDEX = 5
  DIAGNOSTIC_SAMPLE_SECONDS = 0.5
  DIAGNOSTIC_POST_BLINKER_SECONDS = 15.0
  DIAGNOSTIC_MAX_SEQUENCE_SECONDS = 30.0

  def __init__(self, params):
    self.params = params
    self.pending = False
    self.stable_ticks = 0
    self.countdown = 0.0
    self.last_warning_second = 0
    self.direction = "none"
    self.live_sequence = False
    self.last_wait_reason = None
    self.last_config_error = None
    self.faulted = False
    self.error_logged = False
    self.pause_confirmation_ticks = 0
    self.high_speed_lane_confirmed = False
    self.active_blinker_direction = "none"
    self.last_diagnostic_sample = 0.0
    self.diagnostic_observe_until = 0.0
    self.diagnostic_sequence_started = 0.0
    self.diagnostic_sequence_active = False
    self.diagnostic_faulted = False
    self.diagnostic_error_logged = False
    self.read_params()

  def read_params(self) -> None:
    if self.faulted:
      return
    self.observe_enabled = self.params.get_bool("RivianPostTurnObserve")
    self.go_live = self.params.get_bool("RivianPostTurnGoLive")
    self.relaxed_road_edges = self.params.get_bool("RivianPostTurnRelaxedRoadEdges")
    self.is_metric = self.params.get_bool("IsMetric")
    self.logging_enabled = self.params.get_bool("RivianPilotFeatureLogging")
    self.max_turn_speed = self.params.get("RivianPostTurnMaxSpeed", return_default=True)
    self.min_resume_speed = self.params.get("RivianPostTurnMinResumeSpeed", return_default=True)
    self.stable_seconds = self.params.get("RivianPostTurnStableSeconds", return_default=True)
    self.resume_delay = self.params.get("RivianPostTurnResumeDelay", return_default=True)
    self._validate_config()

  def _validate_config(self) -> None:
    errors = []
    if not 10 <= self.max_turn_speed <= 40:
      errors.append("max_turn_speed_out_of_range")
    if not 5 <= self.min_resume_speed <= 30:
      errors.append("min_resume_speed_out_of_range")
    if self.min_resume_speed > self.max_turn_speed:
      errors.append("resume_speed_above_turn_speed")
    if not 1 <= self.stable_seconds <= 3:
      errors.append("stable_seconds_out_of_range")
    if not 1 <= self.resume_delay <= 5:
      errors.append("resume_delay_out_of_range")
    signature = tuple(errors) or None
    if signature is not None and signature != self.last_config_error:
      try:
        cloudlog.event(
          "rivianpilot feature error",
          feature="post_turn_resume",
          errors=errors,
          max_turn_speed=self.max_turn_speed,
          min_resume_speed=self.min_resume_speed,
          stable_seconds=self.stable_seconds,
          resume_delay=self.resume_delay,
          observe_enabled=self.observe_enabled,
          go_live=self.go_live,
        )
      except Exception:
        pass
    self.last_config_error = signature

  def _log(self, action: str, CS: structs.CarState, **kwargs) -> None:
    if not self.logging_enabled:
      return
    try:
      cloudlog.event(
        "rivian mads post turn resume",
        action=action,
        direction=self.direction,
        speed_ms=round(CS.vEgo, 3),
        steering_angle_deg=round(CS.steeringAngleDeg, 2),
        yaw_rate=round(CS.yawRate, 3),
        **kwargs,
      )
    except Exception:
      # Diagnostics are optional and must never affect lateral control.
      self.logging_enabled = False

  @staticmethod
  def _diagnostic_fields(CS: structs.CarState, model, controls_state=None,
                         car_control=None, car_output=None) -> dict:
    """Collect lane-change-to-turn evidence without influencing state decisions."""
    fields = {}
    try:
      fields.update({
        "accel_ms2": round(float(getattr(CS, "aEgo", 0.0)), 3),
        "gas_pressed": bool(getattr(CS, "gasPressed", False)),
        "brake_pressed": bool(getattr(CS, "brakePressed", False)),
        "steering_pressed": bool(getattr(CS, "steeringPressed", False)),
        "driver_torque": round(float(getattr(CS, "steeringTorque", 0.0)), 3),
        "eps_torque": round(float(getattr(CS, "steeringTorqueEps", 0.0)), 3),
        "steering_rate_deg_s": round(float(getattr(CS, "steeringRateDeg", 0.0)), 3),
        "left_blinker": bool(CS.leftBlinker),
        "right_blinker": bool(CS.rightBlinker),
      })
    except (TypeError, ValueError, OverflowError):
      pass
    try:
      meta = model.meta
      fields["lane_change_state"] = str(meta.laneChangeState)
      fields["lane_change_direction"] = str(meta.laneChangeDirection)
      fields["hard_brake_predicted"] = bool(getattr(meta, "hardBrakePredicted", False))
    except (AttributeError, TypeError, ValueError):
      pass
    try:
      lane_probs = list(model.laneLineProbs)
      fields["left_lane_probability"] = round(float(lane_probs[1]), 3) if len(lane_probs) >= 3 else None
      fields["right_lane_probability"] = round(float(lane_probs[2]), 3) if len(lane_probs) >= 3 else None
      edge_stds = list(model.roadEdgeStds)
      fields["left_road_edge_std"] = round(float(edge_stds[0]), 3) if len(edge_stds) >= 2 else None
      fields["right_road_edge_std"] = round(float(edge_stds[1]), 3) if len(edge_stds) >= 2 else None
    except (AttributeError, TypeError, ValueError, OverflowError):
      pass
    try:
      actual_curvature = float(controls_state.curvature)
      desired_curvature = float(controls_state.desiredCurvature)
      fields.update({
        "actual_curvature": round(actual_curvature, 7),
        "desired_curvature": round(desired_curvature, 7),
        "actual_lateral_accel_ms2": round(actual_curvature * float(CS.vEgo) ** 2, 3),
        "desired_lateral_accel_ms2": round(desired_curvature * float(CS.vEgo) ** 2, 3),
      })
      lateral_state = controls_state.lateralControlState
      controller_type = lateral_state.which()
      lateral_log = getattr(lateral_state, controller_type)
      fields["controller_type"] = controller_type
      fields["controller_saturated"] = bool(getattr(lateral_log, "saturated", False))
      fields["controller_output"] = round(float(getattr(lateral_log, "output", 0.0)), 4)
    except (AttributeError, TypeError, ValueError, OverflowError):
      pass
    try:
      fields["requested_torque"] = round(float(car_control.actuators.torque), 4)
    except (AttributeError, TypeError, ValueError, OverflowError):
      pass
    try:
      fields["applied_torque"] = round(float(car_output.actuatorsOutput.torque), 4)
    except (AttributeError, TypeError, ValueError, OverflowError):
      pass
    return fields

  def _sample_diagnostics(self, CS: structs.CarState, model, controls_state=None,
                          car_control=None, car_output=None, force: bool = False) -> None:
    if not self.logging_enabled or self.diagnostic_faulted:
      return
    now = time.monotonic()
    if not force and now - self.last_diagnostic_sample < self.DIAGNOSTIC_SAMPLE_SECONDS:
      return
    self.last_diagnostic_sample = now
    self._log("handover_observation", CS, pending=self.pending, live_sequence=self.live_sequence,
              wait_reason=self.last_wait_reason,
              **self._safe_diagnostic_fields(CS, model, controls_state, car_control, car_output))

  def _suppress_diagnostics(self, exception: Exception) -> None:
    """Disable observation only; never disable Classic Post-Turn Resume."""
    self.diagnostic_faulted = True
    if self.diagnostic_error_logged:
      return
    self.diagnostic_error_logged = True
    try:
      cloudlog.event("rivianpilot feature error", feature="post_turn_handover_observer",
                     errors=["diagnostic_failure_suppressed"], error_type=type(exception).__name__)
    except Exception:
      pass

  def _safe_diagnostic_fields(self, CS: structs.CarState, model, controls_state=None,
                              car_control=None, car_output=None) -> dict:
    if self.diagnostic_faulted:
      return {}
    try:
      return self._diagnostic_fields(CS, model, controls_state, car_control, car_output)
    except Exception as e:
      self._suppress_diagnostics(e)
      return {}

  def _observe_handover_sequence(self, CS: structs.CarState, model, controls_state=None,
                                 car_control=None, car_output=None) -> None:
    """Observe high-speed Nudge through the following turn without taking action."""
    now = time.monotonic()
    lane_change_active = False
    try:
      lane_change_active = model.meta.laneChangeState != log.LaneChangeState.off
    except (AttributeError, TypeError, ValueError):
      pass
    trigger_active = self._one_blinker(CS) or lane_change_active
    if trigger_active and not self.diagnostic_sequence_active:
      self.diagnostic_sequence_active = True
      self.diagnostic_sequence_started = now
    if trigger_active:
      sequence_limit = self.diagnostic_sequence_started + self.DIAGNOSTIC_MAX_SEQUENCE_SECONDS
      self.diagnostic_observe_until = min(sequence_limit, now + self.DIAGNOSTIC_POST_BLINKER_SECONDS)
    if now <= self.diagnostic_observe_until:
      self._sample_diagnostics(CS, model, controls_state, car_control, car_output)
    elif not trigger_active:
      self.diagnostic_sequence_active = False
      self.diagnostic_sequence_started = 0.0

  def suppress_after_error(self, exception: Exception, error: str = "runtime_failure_suppressed") -> None:
    """Disable this feature for the current process after an unexpected failure."""
    self.faulted = True
    self.pending = False
    self.live_sequence = False
    self.stable_ticks = 0
    self.countdown = 0.0
    if self.error_logged:
      return
    self.error_logged = True
    try:
      cloudlog.event(
        "rivianpilot feature error",
        feature="post_turn_resume",
        errors=[error],
        error_type=type(exception).__name__,
      )
    except Exception:
      pass

  def reset(self, CS: structs.CarState | None = None, reason: str | None = None) -> None:
    if self.pending and CS is not None and reason is not None:
      self._log("reset", CS, reason=reason)
    self.pending = False
    self.stable_ticks = 0
    self.countdown = 0.0
    self.last_warning_second = 0
    self.direction = "none"
    self.live_sequence = False
    self.last_wait_reason = None
    self.pause_confirmation_ticks = 0
    self.high_speed_lane_confirmed = False
    self.active_blinker_direction = "none"
    self.last_diagnostic_sample = 0.0

  def _speed_factor(self) -> float:
    return CV.KPH_TO_MS if self.is_metric else CV.MPH_TO_MS

  @staticmethod
  def _one_blinker(CS: structs.CarState) -> bool:
    return CS.leftBlinker != CS.rightBlinker

  def _arm_condition(self, CS: structs.CarState, lateral_active: bool) -> bool:
    return bool(
      (self.observe_enabled or self.go_live) and lateral_active and self._one_blinker(CS) and
      CS.gearShifter == structs.CarState.GearShifter.drive and
      CS.vEgo <= self.max_turn_speed * self._speed_factor()
    )

  @staticmethod
  def _blinker_direction(CS: structs.CarState) -> str:
    if CS.leftBlinker == CS.rightBlinker:
      return "none"
    return "left" if CS.leftBlinker else "right"

  def _extend_turn_sequence(self, CS: structs.CarState, current_direction: str) -> None:
    """Keep MADS paused while restarting only final-turn stability evaluation."""
    previous_direction = self.direction
    self.direction = current_direction
    self.stable_ticks = 0
    self.countdown = 0.0
    self.last_warning_second = 0
    self.last_wait_reason = None
    self.high_speed_lane_confirmed = False
    self._log("turn_sequence_extended", CS, previous_direction=previous_direction,
              new_direction=current_direction)

  def _above_turn_speed(self, CS: structs.CarState) -> bool:
    return CS.vEgo > self.max_turn_speed * self._speed_factor()

  def _lane_confident(self, model: log.ModelDataV2, model_valid: bool) -> bool:
    return bool(model_valid and len(model.laneLineProbs) >= 3 and
                model.laneLineProbs[1] >= self.LANE_PROB_MIN and
                model.laneLineProbs[2] >= self.LANE_PROB_MIN)

  def _road_edges_confident(self, model: log.ModelDataV2, model_valid: bool) -> bool:
    """Require two reliable road edges and a plausible model path between them."""
    if not model_valid or len(model.roadEdges) < 2 or len(model.roadEdgeStds) < 2:
      return False
    if model.roadEdgeStds[0] > self.ROAD_EDGE_STD_MAX or model.roadEdgeStds[1] > self.ROAD_EDGE_STD_MAX:
      return False

    left_y = model.roadEdges[0].y
    right_y = model.roadEdges[1].y
    path_y = model.position.y
    sample_index = self.ROAD_EDGE_SAMPLE_INDEX
    if min(len(left_y), len(right_y), len(path_y)) <= sample_index:
      return False

    left = float(left_y[sample_index])
    right = float(right_y[sample_index])
    path = float(path_y[sample_index])
    if not all(math.isfinite(value) for value in (left, right, path)):
      return False

    low_edge, high_edge = sorted((left, right))
    road_width = high_edge - low_edge
    road_center = (low_edge + high_edge) / 2.0
    return bool(
      self.ROAD_WIDTH_MIN_M <= road_width <= self.ROAD_WIDTH_MAX_M and
      low_edge + self.ROAD_EDGE_MARGIN_M <= path <= high_edge - self.ROAD_EDGE_MARGIN_M and
      abs(path - road_center) <= self.PATH_CENTER_TOLERANCE_M
    )

  def _recovery_path(self, model: log.ModelDataV2, model_valid: bool, high_speed_escape: bool) -> str | None:
    if self._lane_confident(model, model_valid):
      return "lane_lines"
    if self.relaxed_road_edges and not high_speed_escape and self._road_edges_confident(model, model_valid):
      return "road_edges"
    return None

  def _settled_reason(self, CS: structs.CarState) -> str | None:
    if self._one_blinker(CS):
      return "blinker_active"
    if CS.gearShifter != structs.CarState.GearShifter.drive:
      return "not_in_drive"
    if CS.vEgo < self.min_resume_speed * self._speed_factor():
      return "below_resume_speed"
    if CS.steeringPressed:
      return "driver_steering"
    if abs(CS.steeringAngleDeg) > self.STEERING_ANGLE_MAX_DEG:
      return "steering_not_straight"
    if abs(CS.yawRate) > self.YAW_RATE_MAX_RADS:
      return "yaw_not_settled"
    return None

  def _stable_reason(self, CS: structs.CarState, model: log.ModelDataV2, model_valid: bool,
                     high_speed_escape: bool = False) -> str | None:
    settled_reason = self._settled_reason(CS)
    if settled_reason is not None:
      return settled_reason
    if high_speed_escape and self.high_speed_lane_confirmed:
      return None
    if not model_valid:
      return "model_unavailable"
    if self._recovery_path(model, model_valid, high_speed_escape) is None:
      return "lane_not_stable"
    return None

  def update(self, CS: structs.CarState, model: log.ModelDataV2, model_valid: bool,
             lateral_active: bool, controls_state=None, car_control=None,
             car_output=None) -> tuple[PostTurnAction, int | None]:
    """Return state action plus an optional countdown warning second."""
    if self.faulted or (not self.observe_enabled and not self.go_live) or (self.go_live and self.last_config_error is not None):
      if self.pending:
        self.reset(CS, "disabled")
      return PostTurnAction.none, None

    if not self.diagnostic_faulted:
      try:
        self._observe_handover_sequence(CS, model, controls_state, car_control, car_output)
      except Exception as e:
        self._suppress_diagnostics(e)

    if self.pending and self.live_sequence and not self.go_live:
      self.reset(CS, "go_live_disabled")
      return PostTurnAction.none, None

    if not self.pending:
      if not self._arm_condition(CS, lateral_active):
        return PostTurnAction.none, None
      self.pending = True
      self.live_sequence = self.go_live
      self.direction = self._blinker_direction(CS)
      self.active_blinker_direction = self.direction
      self.pause_confirmation_ticks = 0
      diagnostics = self._safe_diagnostic_fields(
        CS, model, controls_state, car_control, car_output,
      ) if self.logging_enabled else {}
      self._log("armed", CS, max_turn_speed=self.max_turn_speed, mode="live" if self.live_sequence else "observe",
                **diagnostics)
      self._sample_diagnostics(CS, model, controls_state, car_control, car_output, force=True)
      return (PostTurnAction.pause if self.live_sequence else PostTurnAction.none), None

    # Gear-driven pauses (especially Reverse) belong to Feature 1. Cancel this
    # sequence so it can never bypass the Reverse-specific resume gate.
    if CS.gearShifter != structs.CarState.GearShifter.drive:
      self.reset(CS, "left_drive")
      return PostTurnAction.none, None

    # Treat every new blinker edge while paused as another turn in the same
    # manual maneuver. Keep MADS paused, but make the final turn authoritative
    # for direction, lane confidence, stability, and warning countdown.
    current_blinker_direction = self._blinker_direction(CS)
    if current_blinker_direction != "none" and current_blinker_direction != self.active_blinker_direction:
      self._extend_turn_sequence(CS, current_blinker_direction)
    self.active_blinker_direction = current_blinker_direction
    self._sample_diagnostics(CS, model, controls_state, car_control, car_output)

    if self.live_sequence and lateral_active:
      self.pause_confirmation_ticks += 1
      if self.pause_confirmation_ticks * DT_CTRL >= self.PAUSE_CONFIRMATION_SECONDS:
        self.suppress_after_error(RuntimeError("MADS pause was not confirmed"), "pause_not_confirmed")
        return PostTurnAction.none, None
      return PostTurnAction.waiting, None
    self.pause_confirmation_ticks = 0

    high_speed_escape = self._above_turn_speed(CS)
    if high_speed_escape and not self.high_speed_lane_confirmed and self._settled_reason(CS) is None and \
       self._lane_confident(model, model_valid):
      self.high_speed_lane_confirmed = True
      self._log("lane_confidence_latched", CS, lane_probability_left=round(model.laneLineProbs[1], 3),
                lane_probability_right=round(model.laneLineProbs[2], 3))
    reason = self._stable_reason(CS, model, model_valid, high_speed_escape)
    if reason is not None:
      if reason != self.last_wait_reason:
        self._log("waiting", CS, reason=reason)
      self.last_wait_reason = reason
      self.stable_ticks = 0
      self.countdown = 0.0
      self.last_warning_second = 0
      return PostTurnAction.waiting, None

    self.last_wait_reason = None
    recovery_path = self._recovery_path(model, model_valid, high_speed_escape)
    required_stable_seconds = self.HIGH_SPEED_STABLE_SECONDS if high_speed_escape else self.stable_seconds
    required_stable_ticks = max(1, round(required_stable_seconds / DT_CTRL))
    if self.stable_ticks < required_stable_ticks:
      self.stable_ticks += 1
      if self.stable_ticks == required_stable_ticks:
        self._log("lane_stable", CS, stable_seconds=required_stable_seconds,
                  resume_path="above_turn_speed" if high_speed_escape else recovery_path)
      return PostTurnAction.waiting, None

    if not self.live_sequence:
      self._log("would_resume", CS, stable_seconds=self.stable_seconds)
      self.reset()
      return PostTurnAction.none, None

    if self.countdown <= 0.0:
      self.countdown = float(max(1, min(5, int(self.resume_delay))))
      self.last_warning_second = math.ceil(self.countdown)
      self._log("countdown_started", CS, delay_seconds=self.last_warning_second)
      warning = self.last_warning_second
    else:
      warning = None

    self.countdown = max(0.0, self.countdown - DT_CTRL)
    if self.countdown > 0.0:
      second = math.ceil(self.countdown)
      if second < self.last_warning_second:
        self.last_warning_second = second
        warning = second
      return PostTurnAction.waiting, warning

    self._log("resumed", CS, stable_seconds=required_stable_seconds, configured_delay=self.resume_delay,
              resume_path="above_turn_speed" if high_speed_escape else recovery_path)
    return PostTurnAction.resume, warning
