from __future__ import annotations

from dataclasses import dataclass
import platform
import time

from cereal import log

from openpilot.common.params import Params
from openpilot.common.swaglog import cloudlog
from openpilot.sunnypilot.selfdrive.controls.lib.auto_lane_change import AutoLaneChangeMode


def memory_params() -> Params:
  """Return the in-memory Params store supported by this AdventurePilot base."""
  return Params("/dev/shm/params") if platform.system() != "Darwin" else Params()


VBSM_STATE_TIMEOUT_SECONDS = 3.0
VBSM_BLOCK_TIMEOUT_SECONDS = 10.0

LaneChangeDirection = log.LaneChangeDirection


@dataclass(frozen=True)
class VisionBSMState:
  left: bool = False
  right: bool = False
  left_confidence: float = 0.0
  right_confidence: float = 0.0
  fresh: bool = False
  age: float = 0.0

  def detected(self, direction) -> bool:
    return ((direction == LaneChangeDirection.left and self.left) or
            (direction == LaneChangeDirection.right and self.right))

  def confidence(self, direction) -> float:
    if direction == LaneChangeDirection.left:
      return self.left_confidence
    if direction == LaneChangeDirection.right:
      return self.right_confidence
    return 0.0


def get_matching_vision_bsm_side(left_blinker: bool, right_blinker: bool,
                                 state: VisionBSMState) -> str:
  """Return a detected vehicle side only for one unambiguous active blinker."""
  if left_blinker == right_blinker:
    return ""
  if left_blinker and state.left:
    return "left"
  if right_blinker and state.right:
    return "right"
  return ""


def _float_param(params, key: str, default: float = 0.0) -> float:
  try:
    return float(params.get(key) or default)
  except (TypeError, ValueError):
    return default


def get_fresh_vision_bsm_state(params_memory, now: float | None = None) -> VisionBSMState:
  """Return detector state only while the isolated vision daemon is updating it."""
  updated_at = _float_param(params_memory, "RivianPilotVisionBSMLastUpdateMonoTime")
  current_time = time.monotonic() if now is None else now
  age = current_time - updated_at
  if updated_at <= 0.0 or age < 0.0 or age > VBSM_STATE_TIMEOUT_SECONDS:
    return VisionBSMState(age=max(age, 0.0))

  active_values = ("1", b"1", True)
  return VisionBSMState(
    left=params_memory.get("RivianPilotVisionBSMLeftActive") in active_values,
    right=params_memory.get("RivianPilotVisionBSMRightActive") in active_values,
    left_confidence=_float_param(params_memory, "RivianPilotVisionBSMLeftConfidence"),
    right_confidence=_float_param(params_memory, "RivianPilotVisionBSMRightConfidence"),
    fresh=True,
    age=age,
  )


def get_vision_bsm_block(params_memory, now: float | None = None) -> tuple[str, float]:
  blocked_at = _float_param(params_memory, "RivianPilotVisionBSMBlockedAt")
  current_time = time.monotonic() if now is None else now
  age = current_time - blocked_at
  if blocked_at <= 0.0 or age < 0.0 or age > VBSM_BLOCK_TIMEOUT_SECONDS:
    return "", max(age, 0.0)
  side = params_memory.get("RivianPilotVisionBSMBlockedSide")
  if isinstance(side, bytes):
    side = side.decode("utf-8", errors="ignore")
  return side if side in ("left", "right") else "", age


class VisionBSMLaneChangeGuard:
  """Optional V-BSM veto. It never requests steering or initiates a lane change."""

  def __init__(self, params: Params | None = None, params_memory: Params | None = None):
    self.params = params or Params()
    self.params_memory = params_memory or memory_params()
    self.enabled = False
    self.logging_enabled = False
    self._param_counter = 0
    self._nudge_consumed = False
    self._waiting_for_torque_release = False
    self._blocked_side = ""
    self._last_error_log = 0.0
    self.read_params()

  def read_params(self) -> None:
    self.enabled = self.params.get_bool("RivianPilotVisionBSMEnabled")
    self.logging_enabled = self.params.get_bool("RivianPilotFeatureLogging")

  def update_params(self) -> None:
    if self._param_counter % 50 == 0:
      try:
        self.read_params()
      except Exception as exc:
        self._log_error("param_read_failure", exc)
        self.enabled = False
    self._param_counter += 1

  @staticmethod
  def _side(direction) -> str:
    if direction == LaneChangeDirection.left:
      return "left"
    if direction == LaneChangeDirection.right:
      return "right"
    return ""

  def state(self, now: float | None = None) -> VisionBSMState:
    if not self.enabled:
      return VisionBSMState()
    try:
      return get_fresh_vision_bsm_state(self.params_memory, now)
    except Exception as exc:
      self._log_error("state_read_failure", exc)
      return VisionBSMState()

  def _log(self, action: str, **kwargs) -> None:
    if not self.logging_enabled:
      return
    try:
      cloudlog.event("rivianpilot vision bsm", action=action, **kwargs)
    except Exception:
      pass

  def _log_error(self, reason: str, exc: Exception) -> None:
    now = time.monotonic()
    if now - self._last_error_log < 30.0:
      return
    self._last_error_log = now
    try:
      cloudlog.event("rivianpilot feature error", feature="vision_bsm_guard",
                     errors=[reason], error_type=type(exc).__name__)
    except Exception:
      pass

  def _publish_block(self, direction, request_source: str, state: VisionBSMState, now: float) -> None:
    side = self._side(direction)
    if not side:
      return
    self._blocked_side = side
    try:
      self.params_memory.put("RivianPilotVisionBSMBlockedSide", side)
      self.params_memory.put("RivianPilotVisionBSMBlockedAt", float(now))
    except Exception as exc:
      self._log_error("block_publish_failure", exc)
    self._log("lane_change_cancelled", side=side, request_source=request_source,
              confidence=state.confidence(direction), detector_age=state.age)

  def clear_block(self, reason: str) -> None:
    if not self._blocked_side:
      return
    self._log("request_rearmed", side=self._blocked_side, reason=reason)
    self._blocked_side = ""
    try:
      self.params_memory.put("RivianPilotVisionBSMBlockedSide", "")
      self.params_memory.put("RivianPilotVisionBSMBlockedAt", 0.0)
    except Exception as exc:
      self._log_error("block_clear_failure", exc)

  def reset_for_blinker_off(self) -> None:
    if not (self._nudge_consumed or self._waiting_for_torque_release or self._blocked_side):
      return
    self._nudge_consumed = False
    self._waiting_for_torque_release = False
    self.clear_block("blinker_cycled")

  def filter_nudge(self, torque_applied: bool, direction, state: VisionBSMState,
                   now: float | None = None) -> bool:
    """Consume a nudge made while occupied; require release before accepting another."""
    current_time = time.monotonic() if now is None else now
    if not self.enabled:
      self._nudge_consumed = False
      self._waiting_for_torque_release = False
      return torque_applied

    if state.detected(direction) and torque_applied:
      if not self._nudge_consumed:
        self._publish_block(direction, "nudge", state, current_time)
      self._nudge_consumed = True
      self._waiting_for_torque_release = True
      return False

    if self._waiting_for_torque_release:
      if not torque_applied:
        self._waiting_for_torque_release = False
        self._nudge_consumed = False
        self.clear_block("fresh_nudge_required")
      return False

    return torque_applied

  def should_cancel_blinker_request(self, lane_change_mode: int, direction,
                                    state: VisionBSMState, now: float | None = None) -> bool:
    """Cancel a blinker-only request once; DesireHelper then requires an off/on cycle."""
    if not self.enabled or lane_change_mode in (AutoLaneChangeMode.OFF, AutoLaneChangeMode.NUDGE):
      return False
    if not state.detected(direction):
      return False
    current_time = time.monotonic() if now is None else now
    self._publish_block(direction, "blinker", state, current_time)
    return True
