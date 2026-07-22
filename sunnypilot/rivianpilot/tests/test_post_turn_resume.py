from types import SimpleNamespace

from opendbc.car import structs

from openpilot.common.constants import CV
from openpilot.common.realtime import DT_CTRL
from openpilot.sunnypilot.rivianpilot.post_turn_resume import PostTurnAction, PostTurnResume


def make_params(mocker, observe=True, go_live=True):
  params = mocker.MagicMock()
  params.get_bool.side_effect = lambda key: {
    "RivianPostTurnObserve": observe,
    "RivianPostTurnGoLive": go_live,
    "RivianPilotFeatureLogging": False,
    "IsMetric": False,
  }.get(key, False)
  params.get.side_effect = lambda key, **kwargs: {
    "RivianPostTurnMaxSpeed": 30,
    "RivianPostTurnMinResumeSpeed": 10,
    "RivianPostTurnStableSeconds": 1,
    "RivianPostTurnResumeDelay": 3,
  }[key]
  return params


def car_state(speed_mph=15, left=False, right=False):
  CS = structs.CarState()
  CS.vEgo = speed_mph * CV.MPH_TO_MS
  CS.gearShifter = structs.CarState.GearShifter.drive
  CS.leftBlinker = left
  CS.rightBlinker = right
  CS.steeringAngleDeg = 0
  CS.yawRate = 0
  CS.steeringPressed = False
  return CS


def model(left_prob=0.9, right_prob=0.9):
  return SimpleNamespace(laneLineProbs=[0.0, left_prob, right_prob, 0.0])


def test_disabled_never_arms(mocker):
  feature = PostTurnResume(make_params(mocker, observe=False, go_live=False))
  action, warning = feature.update(car_state(left=True), model(), True, True)
  assert action == PostTurnAction.none
  assert warning is None
  assert not feature.pending


def test_observe_only_tracks_without_control_change(mocker):
  feature = PostTurnResume(make_params(mocker, observe=True, go_live=False))
  action, warning = feature.update(car_state(left=True), model(), True, True)
  assert action == PostTurnAction.none
  assert warning is None
  assert feature.pending
  assert not feature.live_sequence


def test_go_live_without_observe_fails_safe(mocker):
  feature = PostTurnResume(make_params(mocker, observe=False, go_live=True))
  action, warning = feature.update(car_state(left=True), model(), True, True)
  assert action == PostTurnAction.none
  assert warning is None
  assert not feature.pending


def test_qualifying_turn_arms_pause(mocker):
  feature = PostTurnResume(make_params(mocker))
  action, warning = feature.update(car_state(left=True), model(), True, True)
  assert action == PostTurnAction.pause
  assert warning is None
  assert feature.pending
  assert feature.direction == "left"
  assert feature.live_sequence


def test_above_turn_speed_does_not_change_high_speed_behavior(mocker):
  feature = PostTurnResume(make_params(mocker))
  action, _ = feature.update(car_state(speed_mph=30.1, left=True), model(), True, True)
  assert action == PostTurnAction.none
  assert not feature.pending


def test_requires_active_lateral_control_to_arm(mocker):
  feature = PostTurnResume(make_params(mocker))
  action, _ = feature.update(car_state(left=True), model(), True, False)
  assert action == PostTurnAction.none


def test_waits_for_blinker_driver_yaw_and_lane(mocker):
  feature = PostTurnResume(make_params(mocker))
  feature.update(car_state(left=True), model(), True, True)

  cases = []
  cases.append((car_state(left=True), model(), True))
  steering = car_state()
  steering.steeringPressed = True
  cases.append((steering, model(), True))
  yawing = car_state()
  yawing.yawRate = 0.09
  cases.append((yawing, model(), True))
  cases.append((car_state(), model(left_prob=0.49), True))
  cases.append((car_state(), model(), False))

  for CS, lane_model, valid in cases:
    action, warning = feature.update(CS, lane_model, valid, False)
    assert action == PostTurnAction.waiting
    assert warning is None
    assert feature.stable_ticks == 0


def test_stable_lane_then_countdown_resumes(mocker):
  feature = PostTurnResume(make_params(mocker))
  feature.update(car_state(right=True), model(), True, True)
  CS = car_state(speed_mph=15)

  for _ in range(round(feature.stable_seconds / DT_CTRL)):
    action, warning = feature.update(CS, model(), True, False)
    assert action == PostTurnAction.waiting
    assert warning is None

  seen = []
  for _ in range(round(feature.resume_delay / DT_CTRL) + 2):
    action, warning = feature.update(CS, model(), True, False)
    if warning is not None:
      seen.append(warning)
    if action == PostTurnAction.resume:
      break
  else:
    raise AssertionError("post-turn MADS did not resume")

  assert seen == [3, 2, 1]


def test_countdown_resets_when_driver_turns_again(mocker):
  feature = PostTurnResume(make_params(mocker))
  feature.stable_seconds = 1
  feature.update(car_state(left=True), model(), True, True)
  CS = car_state()
  for _ in range(round(1 / DT_CTRL) + 2):
    feature.update(CS, model(), True, False)
  assert feature.countdown > 0

  turning = car_state(left=True)
  action, warning = feature.update(turning, model(), True, False)
  assert action == PostTurnAction.waiting
  assert warning is None
  assert feature.countdown == 0
  assert feature.stable_ticks == 0


def test_reverse_cancels_and_yields_to_reverse_feature(mocker):
  feature = PostTurnResume(make_params(mocker))
  feature.update(car_state(left=True), model(), True, True)
  reverse = car_state()
  reverse.gearShifter = structs.CarState.GearShifter.reverse

  action, warning = feature.update(reverse, model(), True, False)
  assert action == PostTurnAction.none
  assert warning is None
  assert not feature.pending


def test_metric_thresholds(mocker):
  feature = PostTurnResume(make_params(mocker))
  feature.is_metric = True
  feature.max_turn_speed = 30

  below = car_state(speed_mph=0, left=True)
  below.vEgo = 29.9 * CV.KPH_TO_MS
  assert feature.update(below, model(), True, True)[0] == PostTurnAction.pause

  feature.reset()
  above = car_state(speed_mph=0, left=True)
  above.vEgo = 30.1 * CV.KPH_TO_MS
  assert feature.update(above, model(), True, True)[0] == PostTurnAction.none
