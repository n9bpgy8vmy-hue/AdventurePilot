from cereal import log

from openpilot.sunnypilot.rivianpilot.vision_bsm import (
  VBSM_BLOCK_TIMEOUT_SECONDS,
  VBSM_STATE_TIMEOUT_SECONDS,
  VisionBSMState,
  VisionBSMLaneChangeGuard,
  get_fresh_vision_bsm_state,
  get_matching_vision_bsm_side,
  get_vision_bsm_block,
)
from openpilot.sunnypilot.rivianpilot.vision_bsmd import VisionBSMDaemon
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


class StrictRuntimeParams(FakeParams):
  TYPES = {
    "RivianPilotVisionBSMLastUpdateMonoTime": float,
    "RivianPilotVisionBSMLeftActive": bool,
    "RivianPilotVisionBSMRightActive": bool,
    "RivianPilotVisionBSMLeftConfidence": float,
    "RivianPilotVisionBSMRightConfidence": float,
    "RivianPilotVisionBSMBlockedAt": float,
  }

  def put(self, key, value):
    expected = self.TYPES.get(key)
    if expected is not None:
      assert isinstance(value, expected), (key, value, expected)
    super().put(key, value)


def test_daemon_reads_typed_float_params_without_get_float_api():
  params = FakeParams({
    "RivianPilotVisionBSMEnabled": True,
    "RivianPilotFeatureLogging": True,
    "RivianPilotVisionBSMConfidenceThreshold": 0.75,
    "RivianPilotVisionBSMSmoothSeconds": 0.3,
  })
  params.get_bool = lambda key: bool(params.values[key])
  params.get = lambda key, **_kwargs: params.values[key]
  daemon = VisionBSMDaemon.__new__(VisionBSMDaemon)
  daemon.params = params
  daemon._cache_params()
  assert daemon._confidence_threshold == 0.75
  assert daemon._smooth_seconds == 0.3


def test_daemon_and_guard_publish_typed_runtime_params():
  memory = StrictRuntimeParams()
  daemon = VisionBSMDaemon.__new__(VisionBSMDaemon)
  daemon.params_memory = memory
  daemon._last_update_at = 0.0
  daemon._last_published = None
  daemon._last_active = (False, False)
  daemon._publish(False, False, 0.0, 0.0, updated_at=0.0, force=True)

  guard = make_guard(memory)
  guard._publish_block(LaneChangeDirection.left, "test", VisionBSMState(left=True, fresh=True), now=100.0)
  guard.clear_block("test")


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
