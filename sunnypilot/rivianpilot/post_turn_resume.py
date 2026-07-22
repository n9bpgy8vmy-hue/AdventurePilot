from __future__ import annotations

import math
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
    self.read_params()

  def read_params(self) -> None:
    self.observe_enabled = self.params.get_bool("RivianPostTurnObserve")
    self.go_live = self.params.get_bool("RivianPostTurnGoLive")
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
    if self.go_live and not self.observe_enabled:
      errors.append("go_live_requires_observe")

    signature = tuple(errors) or None
    if signature is not None and signature != self.last_config_error:
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
    self.last_config_error = signature

  def _log(self, action: str, CS: structs.CarState, **kwargs) -> None:
    if not self.logging_enabled:
      return
    cloudlog.event(
      "rivian mads post turn resume",
      action=action,
      direction=self.direction,
      speed_ms=round(CS.vEgo, 3),
      steering_angle_deg=round(CS.steeringAngleDeg, 2),
      yaw_rate=round(CS.yawRate, 3),
      **kwargs,
    )

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

  def _speed_factor(self) -> float:
    return CV.KPH_TO_MS if self.is_metric else CV.MPH_TO_MS

  @staticmethod
  def _one_blinker(CS: structs.CarState) -> bool:
    return CS.leftBlinker != CS.rightBlinker

  def _arm_condition(self, CS: structs.CarState, lateral_active: bool) -> bool:
    return bool(
      self.observe_enabled and lateral_active and self._one_blinker(CS) and
      CS.gearShifter == structs.CarState.GearShifter.drive and
      CS.vEgo <= self.max_turn_speed * self._speed_factor()
    )

  def _stable_reason(self, CS: structs.CarState, model: log.ModelDataV2, model_valid: bool) -> str | None:
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
    if not model_valid or len(model.laneLineProbs) < 3:
      return "model_unavailable"
    if model.laneLineProbs[1] < self.LANE_PROB_MIN or model.laneLineProbs[2] < self.LANE_PROB_MIN:
      return "lane_not_stable"
    return None

  def update(self, CS: structs.CarState, model: log.ModelDataV2, model_valid: bool,
             lateral_active: bool) -> tuple[PostTurnAction, int | None]:
    """Return state action plus an optional countdown warning second."""
    if not self.observe_enabled or (self.go_live and self.last_config_error is not None):
      if self.pending:
        self.reset(CS, "disabled")
      return PostTurnAction.none, None

    if not self.pending:
      if not self._arm_condition(CS, lateral_active):
        return PostTurnAction.none, None
      self.pending = True
      self.live_sequence = self.go_live
      self.direction = "left" if CS.leftBlinker else "right"
      self._log("armed", CS, max_turn_speed=self.max_turn_speed, mode="live" if self.live_sequence else "observe")
      return (PostTurnAction.pause if self.live_sequence else PostTurnAction.none), None

    # Gear-driven pauses (especially Reverse) belong to Feature 1. Cancel this
    # sequence so it can never bypass the Reverse-specific resume gate.
    if CS.gearShifter != structs.CarState.GearShifter.drive:
      self.reset(CS, "left_drive")
      return PostTurnAction.none, None

    reason = self._stable_reason(CS, model, model_valid)
    if reason is not None:
      if reason != self.last_wait_reason:
        self._log("waiting", CS, reason=reason)
      self.last_wait_reason = reason
      self.stable_ticks = 0
      self.countdown = 0.0
      self.last_warning_second = 0
      return PostTurnAction.waiting, None

    self.last_wait_reason = None
    required_stable_ticks = max(1, round(self.stable_seconds / DT_CTRL))
    if self.stable_ticks < required_stable_ticks:
      self.stable_ticks += 1
      if self.stable_ticks == required_stable_ticks:
        self._log("lane_stable", CS, stable_seconds=self.stable_seconds)
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

    self._log("resumed", CS, stable_seconds=self.stable_seconds, configured_delay=self.resume_delay)
    return PostTurnAction.resume, warning
