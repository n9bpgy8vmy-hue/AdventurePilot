from __future__ import annotations

import time

from cereal import log

from openpilot.common.swaglog import cloudlog


LaneChangeState = log.LaneChangeState
LaneChangeDirection = log.LaneChangeDirection


class BlindSpotObserver:
  """Observe Rivian blind-spot and native lane-change state without actuation."""

  PARAM_READ_INTERVAL = 50
  MIN_EVENT_INTERVAL_SECONDS = 0.2

  def __init__(self, params):
    self.params = params
    self.enabled = False
    self.feature_logging = False
    self.faulted = False
    self.error_logged = False
    self.param_read_counter = 0
    self.session_active = False
    self.last_signature = None
    self.last_emit_time = 0.0
    self.read_params()

  def read_params(self) -> None:
    if self.faulted:
      return
    try:
      self.enabled = self.params.get_bool("RivianPilotBlindSpotObserver")
      self.feature_logging = self.params.get_bool("RivianPilotFeatureLogging")
    except Exception as exc:
      self.suppress_after_error(exc, "parameter_read_failure")

  def update_params(self) -> None:
    if self.faulted:
      return
    self.param_read_counter += 1
    if self.param_read_counter >= self.PARAM_READ_INTERVAL:
      self.param_read_counter = 0
      self.read_params()

  def _emit(self, action: str, CS, lane_change_state, lane_change_direction,
            native_blocked: bool, torque_applied: bool) -> None:
    if not self.feature_logging:
      return
    try:
      cloudlog.event(
        "rivianpilot blind spot observer",
        action=action,
        speed_ms=round(float(CS.vEgo), 3),
        left_blinker=bool(CS.leftBlinker),
        right_blinker=bool(CS.rightBlinker),
        left_blindspot=bool(CS.leftBlindspot),
        right_blindspot=bool(CS.rightBlindspot),
        steering_torque=round(float(CS.steeringTorque), 3),
        steering_pressed=bool(CS.steeringPressed),
        torque_applied=bool(torque_applied),
        lane_change_state=int(lane_change_state),
        lane_change_direction=int(lane_change_direction),
        native_blocked=bool(native_blocked),
      )
    except Exception as exc:
      # Logging is optional. Suppress this observer only; never affect controls.
      self.suppress_after_error(exc, "logging_failure")

  def suppress_after_error(self, exception: Exception, error: str = "runtime_failure_suppressed") -> None:
    self.faulted = True
    self.enabled = False
    self.feature_logging = False
    self.session_active = False
    self.last_signature = None
    self.last_emit_time = 0.0
    if self.error_logged:
      return
    self.error_logged = True
    try:
      cloudlog.event(
        "rivianpilot feature error",
        feature="blind_spot_observer",
        errors=[error],
        error_type=type(exception).__name__,
      )
    except Exception:
      pass

  def update(self, CS, lane_change_state, lane_change_direction, now: float | None = None) -> None:
    """Log only session starts/ends and meaningful signal/state transitions."""
    if self.faulted:
      return
    try:
      self.update_params()
      now = time.monotonic() if now is None else now
      active = bool(self.enabled and self.feature_logging)
      one_blinker = bool(CS.leftBlinker) != bool(CS.rightBlinker)

      if not active:
        self.session_active = False
        self.last_signature = None
        self.last_emit_time = 0.0
        return

      direction = LaneChangeDirection.left if CS.leftBlinker else LaneChangeDirection.right
      blindspot_detected = bool((CS.leftBlindspot and direction == LaneChangeDirection.left) or
                                (CS.rightBlindspot and direction == LaneChangeDirection.right))
      torque_applied = bool(CS.steeringPressed and
                            ((CS.steeringTorque > 0 and direction == LaneChangeDirection.left) or
                             (CS.steeringTorque < 0 and direction == LaneChangeDirection.right)))
      native_blocked = bool(one_blinker and lane_change_state == LaneChangeState.preLaneChange and blindspot_detected)

      if not one_blinker:
        if self.session_active:
          self._emit("session_end", CS, lane_change_state, lane_change_direction, False, False)
        self.session_active = False
        self.last_signature = None
        self.last_emit_time = 0.0
        return

      signature = (
        bool(CS.leftBlindspot), bool(CS.rightBlindspot), int(lane_change_state),
        int(lane_change_direction), bool(CS.steeringPressed), torque_applied, native_blocked,
      )
      action = "session_start" if not self.session_active else "state_changed"
      should_emit = not self.session_active or (signature != self.last_signature and
                                                now - self.last_emit_time >= self.MIN_EVENT_INTERVAL_SECONDS)
      if should_emit:
        self._emit(action, CS, lane_change_state, lane_change_direction, native_blocked, torque_applied)
        if self.faulted:
          return
        self.last_signature = signature
        self.last_emit_time = now
      self.session_active = True
    except Exception as exc:
      self.suppress_after_error(exc)
