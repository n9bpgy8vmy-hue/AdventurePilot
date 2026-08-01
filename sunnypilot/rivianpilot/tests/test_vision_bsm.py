from cereal import log

from openpilot.sunnypilot.rivianpilot.vision_bsm import (
  VBSM_BLOCK_TIMEOUT_SECONDS,
  VBSM_STATE_TIMEOUT_SECONDS,
  VisionBSMLaneChangeGuard,
  get_fresh_vision_bsm_state,
  get_matching_vision_bsm_side,
  get_vision_bsm_block,
)
from openpilot.sunnypilot.selfdrive.controls.lib.auto_lane_change import AutoLaneChangeMode


LaneChangeDirection = log.LaneChangeDirection


class FakeParams:
  def __init__(self, values=None):
    self.values = values or {}

  def get(self, key, *args, **kwargs):
    return self.values.get(key)

  def get_bool(self, key):
    return self.values.get(key) in (True, "1", b"1")

  def put(self, key, value):
    self.values[key] = value


def detector_state(now=100.0, left=False, right=False):
  return FakeParams({
    "RivianPilotVisionBSMLastUpdateMonoTime": str(now),
    "RivianPilotVisionBSMLeftActive": "1" if left else "0",
    "RivianPilotVisionBSMRightActive": "1" if right else "0",
    "RivianPilotVisionBSMLeftConfidence": "0.91" if left else "0.1",
    "RivianPilotVisionBSMRightConfidence": "0.92" if right else "0.1",
  })


def make_guard(memory):
  params = FakeParams({"RivianPilotVisionBSMEnabled": True, "RivianPilotFeatureLogging": False})
  return VisionBSMLaneChangeGuard(params, memory)


def test_fresh_and_stale_detector_state():
  memory = detector_state(left=True)
  fresh = get_fresh_vision_bsm_state(memory, now=101.0)
  stale = get_fresh_vision_bsm_state(memory, now=100.0 + VBSM_STATE_TIMEOUT_SECONDS + 0.01)

  assert fresh.fresh and fresh.left and not fresh.right
  assert fresh.left_confidence == 0.91
  assert not stale.fresh and not stale.left and not stale.right


def test_matching_detection_requires_exactly_one_blinker():
  state = get_fresh_vision_bsm_state(detector_state(left=True, right=True), now=100.1)

  assert get_matching_vision_bsm_side(True, False, state) == "left"
  assert get_matching_vision_bsm_side(False, True, state) == "right"
  assert get_matching_vision_bsm_side(False, False, state) == ""
  assert get_matching_vision_bsm_side(True, True, state) == ""


def test_nudge_during_detection_is_consumed_until_release():
  memory = detector_state(left=True)
  guard = make_guard(memory)
  state = get_fresh_vision_bsm_state(memory, now=100.1)

  assert not guard.filter_nudge(True, LaneChangeDirection.left, state, now=100.1)
  assert memory.values["RivianPilotVisionBSMBlockedSide"] == "left"

  clear = get_fresh_vision_bsm_state(detector_state(now=100.2), now=100.3)
  assert not guard.filter_nudge(True, LaneChangeDirection.left, clear, now=100.3)
  assert not guard.filter_nudge(False, LaneChangeDirection.left, clear, now=100.4)
  assert guard.filter_nudge(True, LaneChangeDirection.left, clear, now=100.5)


def test_blinker_only_request_is_cancelled_but_nudge_mode_is_not():
  memory = detector_state(right=True)
  guard = make_guard(memory)
  state = get_fresh_vision_bsm_state(memory, now=100.1)

  assert not guard.should_cancel_blinker_request(AutoLaneChangeMode.NUDGE, LaneChangeDirection.right, state, now=100.1)
  assert guard.should_cancel_blinker_request(AutoLaneChangeMode.ONE_SECOND, LaneChangeDirection.right, state, now=100.2)
  assert memory.values["RivianPilotVisionBSMBlockedSide"] == "right"


def test_block_state_expires_and_invalid_values_fail_open():
  memory = FakeParams({"RivianPilotVisionBSMBlockedSide": "left", "RivianPilotVisionBSMBlockedAt": "100.0"})
  assert get_vision_bsm_block(memory, now=101.0)[0] == "left"
  assert get_vision_bsm_block(memory, now=100.0 + VBSM_BLOCK_TIMEOUT_SECONDS + 0.01)[0] == ""

  memory.values["RivianPilotVisionBSMBlockedAt"] = "invalid"
  assert get_vision_bsm_block(memory, now=101.0)[0] == ""
