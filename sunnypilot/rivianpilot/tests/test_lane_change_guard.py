from types import SimpleNamespace as NS

import pytest
from cereal import custom, log

from openpilot.selfdrive.controls.lib.desire_helper import DesireHelper
from openpilot.sunnypilot.rivianpilot.lane_change_guard import (
  LaneChangeGeometryGuard, geometry_reason, model_inputs_valid, blocked_reason_from_messages,
)


def road(now=10.):
  xs = [0., 5., 10., 20., 30., 45., 60., 90., 120.]
  def line(y):
    return NS(x=xs.copy(), y=[y] * len(xs))

  return NS(timestampEof=int(now * 1e9), frameId=100, laneLines=[line(y) for y in (-5.4, -1.8, 1.8, 5.4)],
            laneLineProbs=[.95] * 4, laneLineStds=[.1] * 4,
            roadEdges=[line(-7.), line(7.)], roadEdgeStds=[.1, .1])


@pytest.mark.parametrize("side", [-1, 1])
def test_open_lane_and_curved_lane(side):
  model = road()
  assert geometry_reason(model, side, 25.) == ""
  # A common curve/translation must not reverse vehicle-left/right or change widths.
  for line in model.laneLines + model.roadEdges:
    line.y = [y + .002 * x * x + .3 for x, y in zip(line.x, line.y, strict=True)]
  assert geometry_reason(model, side, 25.) == ""


@pytest.mark.parametrize("side", [-1, 1])
@pytest.mark.parametrize("case,expected", [
  ("wall", "edge_clearance"), ("shoulder", "lane_width"), ("missing_line", "lane_uncertain"),
  ("uncertain_edge", "edge_uncertain"), ("fork", "lane_diverging"),
  ("short_horizon", "geometry_invalid"), ("unordered", "geometry_invalid"),
  ("nan_line", "geometry_invalid"), ("infinite_probability", "lane_uncertain"),
  ("negative_std", "lane_uncertain"), ("crossed_line", "lane_width"),
])
def test_rejects_unconfirmed_space(side, case, expected):
  model = road()
  outer, edge = (0, 0) if side == -1 else (3, 1)
  if case == "wall":
    model.roadEdges[edge].y = [side * 2.2] * 9
  elif case == "shoulder":
    model.laneLines[outer].y = [side * 3.] * 9
  elif case == "missing_line":
    model.laneLineProbs[outer] = .2
  elif case == "uncertain_edge":
    model.roadEdgeStds[edge] = .9
  elif case == "fork":
    model.laneLines[outer].y = [side * (5.4 + .01 * x) for x in model.laneLines[outer].x]
  elif case == "short_horizon":
    model.roadEdges[edge].x = [0., 10.]
    model.roadEdges[edge].y = [side * 7.] * 2
  elif case == "unordered":
    model.laneLines[outer].x[2] = 5.
  elif case == "nan_line":
    model.laneLines[outer].y[3] = float("nan")
  elif case == "infinite_probability":
    model.laneLineProbs[outer] = float("inf")
  elif case == "negative_std":
    model.laneLineStds[outer] = -.1
  elif case == "crossed_line":
    model.laneLines[outer].y = [0.] * 9
  assert geometry_reason(model, side, 25.) == expected


def test_side_isolation_and_no_road_edge_fallback():
  model = road()
  model.roadEdges[0].y = [-2.2] * 9
  assert geometry_reason(model, -1, 25.) == "edge_clearance"
  assert geometry_reason(model, 1, 25.) == ""
  model = road()
  model.laneLineProbs[0] = 0.
  assert geometry_reason(model, -1, 25.) == "lane_uncertain"


def test_checks_knots_between_regular_samples():
  model = road()
  model.roadEdges[0].x.insert(4, 22.)
  model.roadEdges[0].y.insert(4, -2.)
  assert geometry_reason(model, -1, 25.) == "edge_clearance"


def warm_guard(guard):
  for i in range(26):
    now = 10. + i * .05
    result = guard.update(road(now), 25., now, True)
  assert result == {-1: "", 1: ""}
  return now


@pytest.mark.parametrize("case", ["stale", "future", "duplicate", "backward", "invalid", "missing", "nan_speed"])
def test_bad_input_revokes_confirmation(case):
  guard = LaneChangeGeometryGuard()
  last = warm_guard(guard)
  now, model, valid, speed = last + .05, road(last + .05), True, 25.
  if case == "stale":
    model.timestampEof = int((now - 1.) * 1e9)
  elif case == "future":
    model.timestampEof = int((now + 1.) * 1e9)
  elif case == "duplicate":
    model.timestampEof = int(last * 1e9)
  elif case == "backward":
    model.timestampEof = int((last - .1) * 1e9)
  elif case == "invalid":
    valid = False
  elif case == "missing":
    model = None
  elif case == "nan_speed":
    speed = float("nan")
  assert guard.update(model, speed, now, valid) == {-1: "camera_unavailable", 1: "camera_unavailable"}
  now += .05
  assert guard.update(road(now), 25., now, True)[-1] == "confirming"


def test_gap_requires_new_confirmation():
  guard = LaneChangeGeometryGuard()
  now = warm_guard(guard) + 1.
  assert guard.update(road(now), 25., now, True)[1] == "confirming"


class FakeParams:
  def get(self, key, **kwargs):
    return {"AutoLaneChangeTimer": 1, "LaneTurnValue": 0}[key]

  def get_bool(self, key):
    return False


@pytest.fixture
def helper(monkeypatch):
  monkeypatch.setattr("openpilot.sunnypilot.selfdrive.controls.lib.auto_lane_change.Params", FakeParams)
  monkeypatch.setattr("openpilot.sunnypilot.selfdrive.controls.lib.lane_turn_desire.Params", FakeParams)
  return DesireHelper(require_lane_change_geometry=True, lane_change_guard_mode=2)


def car(side=0, torque=0.):
  return NS(vEgo=25., leftBlinker=side == -1, rightBlinker=side == 1,
            leftBlindspot=False, rightBlindspot=False, brakePressed=False,
            steeringPressed=torque != 0, steeringTorque=torque)


def warm_helper(helper):
  for i in range(26):
    now = 10. + i * .05
    helper.update(car(), True, 0., road(now), True, now)
  return now


def step(helper, cs, now, model=None, active=True):
  helper.update(cs, active, 1., road(now) if model is None else model, True, now)


@pytest.mark.parametrize("side", [-1, 1])
def test_confirmed_lane_can_start(helper, side):
  now = warm_helper(helper)
  for i in range(1, 6):
    step(helper, car(side), now + i * .05)
  assert helper.lane_change_state == log.LaneChangeState.laneChangeStarting


@pytest.mark.parametrize("side", [-1, 1])
def test_wall_blocks_timer_and_nudge_and_latches_until_new_signal(helper, side):
  now = warm_helper(helper)
  blocked = road(now + .05)
  blocked.roadEdges[0 if side == -1 else 1].y = [side * 2.2] * 9
  step(helper, car(side), now + .05, blocked)
  # Geometry recovers, timer expires, driver nudges: none may bypass the latch.
  for i in range(2, 70):
    step(helper, car(side, torque=-side), now + i * .05)
    assert helper.lane_change_state == log.LaneChangeState.preLaneChange
    assert helper.lane_change_guard_reason == "edge_clearance"
    assert helper.desire not in (log.Desire.laneChangeLeft, log.Desire.laneChangeRight)
  now += 3.5
  step(helper, car(), now)
  for i in range(1, 6):
    step(helper, car(side), now + i * .05)
  assert helper.lane_change_state == log.LaneChangeState.laneChangeStarting


def test_loss_during_timer_blocks_request(helper):
  now = warm_helper(helper)
  helper.alc.params.get = lambda key, **kwargs: 5
  helper.alc.lane_change_set_timer = 5  # three second timer
  step(helper, car(-1), now + .05)
  bad = road(now + .1)
  bad.laneLineProbs[0] = 0.
  step(helper, car(-1), now + .1, bad)
  for i in range(3, 100):
    step(helper, car(-1), now + i * .05)
  assert helper.lane_change_state == log.LaneChangeState.preLaneChange
  assert helper.lane_change_guard_reason == "lane_uncertain"


def test_initial_unconfirmed_request_never_launches_later(helper):
  for i in range(100):
    step(helper, car(1), 10. + i * .05)
  assert helper.lane_change_state == log.LaneChangeState.preLaneChange
  assert helper.lane_change_guard_reason == "confirming"


def test_mid_maneuver_geometry_loss_does_not_force_return(helper):
  now = warm_helper(helper)
  for i in range(1, 6):
    step(helper, car(1), now + i * .05)
  bad = road(now + .3)
  bad.roadEdges[1].y = [2.] * 9
  step(helper, car(1), now + .3, bad)
  assert helper.lane_change_state == log.LaneChangeState.laneChangeStarting
  assert helper.lane_change_guard_reason == ""


def test_blindspot_and_brake_and_disengage_still_block(helper):
  now = warm_helper(helper)
  cs = car(-1)
  cs.leftBlindspot = True
  for i in range(1, 8):
    step(helper, cs, now + i * .05)
  assert helper.lane_change_state == log.LaneChangeState.preLaneChange
  cs.leftBlindspot, cs.brakePressed = False, True
  step(helper, cs, now + .4)
  cs.brakePressed = False
  for i in range(9, 20):
    step(helper, cs, now + i * .05)
  assert helper.lane_change_state == log.LaneChangeState.preLaneChange
  step(helper, cs, now + 1., active=False)
  assert helper.lane_change_state == log.LaneChangeState.off


def test_other_brands_keep_existing_behavior(helper):
  other = DesireHelper()
  for _ in range(5):
    other.update(car(1), True, 1.)
  assert other.lane_change_state == log.LaneChangeState.laneChangeStarting


class FakeSM(dict):
  def __init__(self, **values):
    super().__init__(values)
    self.healthy = True
    self.logMonoTime = {s: int(10e9) for s in ("carState", "carControl", "modelV2", "modelDataV2SP")}
    self.valid = {s: True for s in self.logMonoTime}
    self.alive = self.valid.copy()

  def all_checks(self, services):
    return self.healthy


def test_input_health_contract():
  sm = FakeSM(liveCalibration=NS(calStatus="calibrated"))
  assert model_inputs_valid(sm, 10.1)
  assert not model_inputs_valid(sm, 11.)
  sm["liveCalibration"].calStatus = "uncalibrated"
  assert not model_inputs_valid(sm, 10.1)
  sm["liveCalibration"].calStatus = "calibrated"
  sm.healthy = False
  assert not model_inputs_valid(sm, 10.1)


def test_status_roundtrip_and_display_rejects_mismatched_frames():
  status = custom.ModelDataV2SP.new_message(laneChangeGuardEnabled=True, laneChangeGuardReason="edge_clearance",
                                         laneChangeGuardDirection=-1, laneChangeGuardFrameId=123)
  with custom.ModelDataV2SP.from_bytes(status.to_bytes()) as decoded:
    sm = FakeSM(modelDataV2SP=decoded, modelV2=NS(frameId=123, timestampEof=int(10e9), meta=NS(laneChangeDirection="left")))
    assert blocked_reason_from_messages(sm, 10.1) == "edge_clearance"
    sm["modelV2"].frameId = 124
    assert blocked_reason_from_messages(sm, 10.1) == "camera_unavailable"



def test_display_expires_status_even_if_global_checks_ignore_service():
  status = NS(laneChangeGuardEnabled=True, laneChangeGuardReason="", laneChangeGuardDirection=1, laneChangeGuardFrameId=123)
  sm = FakeSM(modelDataV2SP=status, modelV2=NS(frameId=123, timestampEof=int(10e9), meta=NS(laneChangeDirection="right")))
  assert blocked_reason_from_messages(sm, 10.1) == ""
  assert blocked_reason_from_messages(sm, 10.5) == "camera_unavailable"
  sm.alive["modelDataV2SP"] = False
  assert blocked_reason_from_messages(sm, 10.1) == "camera_unavailable"


def test_signal_direction_change_restarts_timer(helper):
  now = warm_helper(helper)
  helper.alc.params.get = lambda key, **kwargs: 3
  helper.alc.lane_change_set_timer = 3  # one second
  for i in range(1, 18):
    step(helper, car(-1), now + i * .05)
  for i in range(18, 25):
    step(helper, car(1), now + i * .05)
    assert helper.lane_change_state == log.LaneChangeState.preLaneChange
  for i in range(25, 43):
    step(helper, car(1), now + i * .05)
  assert helper.lane_change_state == log.LaneChangeState.laneChangeStarting


@pytest.mark.parametrize("mode", [-1, 0])
def test_off_and_nudge_modes_do_not_start_on_timer(helper, mode):
  now = warm_helper(helper)
  helper.alc.params.get = lambda key, **kwargs: mode
  helper.alc.lane_change_set_timer = mode
  for i in range(1, 100):
    step(helper, car(-1), now + i * .05)
    assert helper.lane_change_state != log.LaneChangeState.laneChangeStarting
  step(helper, car(-1, torque=1.), now + 5.)
  assert helper.lane_change_state == (log.LaneChangeState.off if mode == -1 else log.LaneChangeState.laneChangeStarting)


@pytest.mark.parametrize("mode", [0, 1, 2])
@pytest.mark.parametrize("trigger", ["timer", "nudge"])
def test_guard_mode_controls_blocking(helper, mode, trigger):
  # The helper fixture supplies the same parameter fakes as the live controllers.
  candidate = DesireHelper(require_lane_change_geometry=True, lane_change_guard_mode=mode)
  if trigger == "nudge":
    candidate.alc.params.get = lambda key, **kwargs: 0
    candidate.alc.lane_change_set_timer = 0
  now = warm_helper(candidate)
  for i in range(1, 8):
    stamp = now + i * .05
    blocked = road(stamp)
    blocked.roadEdges[0].y = [-2.2] * 9
    step(candidate, car(-1, torque=1. if trigger == "nudge" else 0.), stamp, blocked)
    if i == 1:
      assert candidate.lane_change_guard_reason == ("" if mode == 0 else "edge_clearance")
  assert candidate.lane_change_state == (log.LaneChangeState.preLaneChange if mode == 2 else log.LaneChangeState.laneChangeStarting)


def test_mode_is_loaded_once_and_invalid_mode_observes(helper, monkeypatch):
  value = [1]
  monkeypatch.setattr("openpilot.selfdrive.controls.lib.desire_helper.Params", lambda: NS(get=lambda *args, **kwargs: value[0]))
  candidate = DesireHelper(require_lane_change_geometry=True)
  assert candidate.lane_change_guard_mode == 1
  value[0] = 2
  warm_helper(candidate)
  assert candidate.lane_change_guard_mode == 1  # requires restart, never changes in a maneuver
  assert DesireHelper(require_lane_change_geometry=True).lane_change_guard_mode == 2
  value[0] = 9
  assert DesireHelper(require_lane_change_geometry=True).lane_change_guard_mode == 1
  assert DesireHelper().lane_change_guard_mode == 0


def test_disabled_guard_has_no_blocked_display():
  sm = FakeSM(modelDataV2SP=NS(laneChangeGuardEnabled=False))
  assert blocked_reason_from_messages(sm, 10.1) == ""
