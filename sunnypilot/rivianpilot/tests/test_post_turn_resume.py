from types import SimpleNamespace

from opendbc.car import structs

from openpilot.common.constants import CV
from openpilot.common.realtime import DT_CTRL
from openpilot.sunnypilot.rivianpilot.post_turn_resume import PostTurnAction, PostTurnResume


def make_params(mocker, observe=True, go_live=True, logging=False):
  params = mocker.MagicMock()
  params.get_bool.side_effect = lambda key: {
    "RivianPostTurnObserve": observe,
    "RivianPostTurnGoLive": go_live,
    "RivianPostTurnRelaxedRoadEdges": False,
    "RivianPilotFeatureLogging": logging,
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
  CS.steeringRateDeg = 0
  CS.steeringTorque = 0
  CS.yawRate = 0
  CS.steeringPressed = False
  return CS


def model(left_prob=0.9, right_prob=0.9, left_edge=-2.0, right_edge=2.0,
          left_edge_std=0.2, right_edge_std=0.2, path_y=0.0):
  def line(y):
    return SimpleNamespace(y=[y] * 10)
  return SimpleNamespace(
    laneLineProbs=[0.0, left_prob, right_prob, 0.0],
    roadEdges=[line(left_edge), line(right_edge)],
    roadEdgeStds=[left_edge_std, right_edge_std],
    position=line(path_y),
    meta=SimpleNamespace(
      laneChangeState=0,
      laneChangeDirection=0,
      hardBrakePredicted=False,
    ),
  )


def run_until_resume(feature, CS, lane_model, model_valid=True):
  for _ in range(round((feature.stable_seconds + feature.resume_delay) / DT_CTRL) + 5):
    action, _ = feature.update(CS, lane_model, model_valid, False)
    if action == PostTurnAction.resume:
      return True
  return False


def test_disabled_never_arms(mocker):
  feature = PostTurnResume(make_params(mocker, observe=False, go_live=False))
  action, warning = feature.update(car_state(left=True), model(), True, True)
  assert action == PostTurnAction.none
  assert warning is None
  assert not feature.pending


def test_zero_resume_delay_resumes_immediately_after_stable_confirmation(mocker):
  params = make_params(mocker)
  original_get = params.get.side_effect
  params.get.side_effect = lambda key, **kwargs: 0 if key == "RivianPostTurnResumeDelay" else original_get(key, **kwargs)
  feature = PostTurnResume(params)
  assert feature.last_config_error is None
  assert feature.update(car_state(left=True), model(), True, True)[0] == PostTurnAction.pause

  settled = car_state(speed_mph=15)
  warnings = []
  for _ in range(round(feature.stable_seconds / DT_CTRL) + 3):
    action, warning = feature.update(settled, model(), True, False)
    warnings.append(warning)
    if action == PostTurnAction.resume:
      break
  else:
    raise AssertionError("zero-delay post-turn sequence did not resume")

  assert all(warning is None for warning in warnings)
  assert feature.countdown == 0.0


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
  assert action == PostTurnAction.pause
  assert warning is None
  assert feature.pending


def test_high_speed_lane_change_is_observed_without_arming_control(mocker):
  feature = PostTurnResume(make_params(mocker, logging=True))
  event = mocker.patch("openpilot.sunnypilot.rivianpilot.post_turn_resume.cloudlog.event")
  lane_model = model()
  lane_model.meta.laneChangeState = 2
  lane_model.meta.laneChangeDirection = 1
  CS = car_state(speed_mph=55, left=True)
  CS.steeringTorque = 1.2
  CS.steeringTorqueEps = 0.4
  CS.steeringRateDeg = 2.0

  action, _ = feature.update(CS, lane_model, True, True)

  assert action == PostTurnAction.none
  assert not feature.pending
  observation = next(call for call in event.call_args_list if call.kwargs.get("action") == "handover_observation")
  assert observation.kwargs["driver_torque"] == 1.2
  assert observation.kwargs["eps_torque"] == 0.4
  assert observation.kwargs["left_blinker"]


def test_handover_diagnostic_failure_does_not_disable_classic_resume(mocker):
  feature = PostTurnResume(make_params(mocker, logging=True))
  mocker.patch.object(feature, "_diagnostic_fields", side_effect=RuntimeError("diagnostic failure"))

  action, _ = feature.update(car_state(left=True), model(), True, True)

  assert action == PostTurnAction.pause
  assert feature.diagnostic_faulted
  assert not feature.faulted
  assert feature.pending


def test_runtime_failure_can_be_suppressed_without_raising(mocker):
  feature = PostTurnResume(make_params(mocker))
  log_event = mocker.patch("openpilot.sunnypilot.rivianpilot.post_turn_resume.cloudlog.event")

  feature.suppress_after_error(RuntimeError("test failure"))
  feature.suppress_after_error(RuntimeError("repeated failure"))

  assert feature.faulted
  assert not feature.pending
  assert feature.update(car_state(left=True), model(), True, True)[0] == PostTurnAction.none
  assert log_event.call_count == 1


def test_live_mode_suppresses_itself_if_mads_pause_is_not_confirmed(mocker):
  feature = PostTurnResume(make_params(mocker))
  log_event = mocker.patch("openpilot.sunnypilot.rivianpilot.post_turn_resume.cloudlog.event")
  CS = car_state(left=True)

  assert feature.update(CS, model(), True, True)[0] == PostTurnAction.pause
  for _ in range(round(feature.PAUSE_CONFIRMATION_SECONDS / DT_CTRL)):
    feature.update(CS, model(), True, True)

  assert feature.faulted
  assert not feature.pending
  assert log_event.call_args.kwargs["errors"] == ["pause_not_confirmed"]


def test_disabling_go_live_cancels_active_sequence(mocker):
  feature = PostTurnResume(make_params(mocker))
  assert feature.update(car_state(left=True), model(), True, True)[0] == PostTurnAction.pause

  feature.go_live = False
  action, warning = feature.update(car_state(), model(), True, False)

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


def test_armed_turn_requires_lane_confidence_once_then_latches(mocker):
  feature = PostTurnResume(make_params(mocker))
  assert feature.update(car_state(speed_mph=20, left=True), model(), True, True)[0] == PostTurnAction.pause

  high_speed = car_state(speed_mph=50)
  unavailable_model = model(left_prob=0.0, right_prob=0.0)
  for _ in range(round(feature.HIGH_SPEED_STABLE_SECONDS / DT_CTRL) + 5):
    action, warning = feature.update(high_speed, unavailable_model, False, False)
    assert action == PostTurnAction.waiting
    assert warning is None
  assert not feature.high_speed_lane_confirmed
  assert feature.stable_ticks == 0

  # One valid, confident model sample latches lane confidence for this sequence.
  action, warning = feature.update(high_speed, model(), True, False)
  assert action == PostTurnAction.waiting
  assert warning is None
  assert feature.high_speed_lane_confirmed

  # Subsequent confidence flicker must not restart confirmation or the countdown.
  for _ in range(round(feature.HIGH_SPEED_STABLE_SECONDS / DT_CTRL) - 1):
    action, warning = feature.update(high_speed, unavailable_model, False, False)
    assert action == PostTurnAction.waiting
    assert warning is None

  seen = []
  for _ in range(round(feature.resume_delay / DT_CTRL) + 2):
    action, warning = feature.update(high_speed, unavailable_model, False, False)
    if warning is not None:
      seen.append(warning)
    if action == PostTurnAction.resume:
      break
  else:
    raise AssertionError("high-speed escape did not resume MADS")

  assert seen == [3, 2, 1]


def test_high_speed_escape_still_waits_for_driver_steering(mocker):
  feature = PostTurnResume(make_params(mocker))
  assert feature.update(car_state(speed_mph=20, right=True), model(), True, True)[0] == PostTurnAction.pause

  high_speed = car_state(speed_mph=50)
  feature.update(high_speed, model(), True, False)
  assert feature.high_speed_lane_confirmed
  high_speed.steeringPressed = True
  high_speed.steeringTorque = 2.0
  for _ in range(round(feature.HIGH_SPEED_STABLE_SECONDS / DT_CTRL) + 5):
    action, warning = feature.update(high_speed, model(left_prob=0.0, right_prob=0.0), False, False)
    assert action == PostTurnAction.waiting
    assert warning is None

  assert feature.stable_ticks == 0
  assert feature.countdown == 0.0


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
  steering.steeringTorque = 2.0
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


def test_light_steering_correction_on_slanted_road_allows_resume(mocker):
  feature = PostTurnResume(make_params(mocker, logging=True))
  event = mocker.patch("openpilot.sunnypilot.rivianpilot.post_turn_resume.cloudlog.event")
  assert feature.update(car_state(right=True), model(), True, True)[0] == PostTurnAction.pause

  correction = car_state(speed_mph=35)
  correction.steeringPressed = True
  correction.steeringTorque = 1.3
  correction.steeringAngleDeg = 3.0
  correction.steeringRateDeg = 2.0

  assert run_until_resume(feature, correction, model())
  assert any(call.kwargs.get("action") == "light_steering_correction_allowed"
             for call in event.call_args_list)


def test_strong_near_straight_driver_torque_blocks_resume(mocker):
  feature = PostTurnResume(make_params(mocker))
  assert feature.update(car_state(right=True), model(), True, True)[0] == PostTurnAction.pause

  takeover = car_state(speed_mph=35)
  takeover.steeringPressed = True
  takeover.steeringTorque = 1.8
  takeover.steeringAngleDeg = 2.0
  takeover.steeringRateDeg = 1.0
  for _ in range(round((feature.stable_seconds + feature.resume_delay) / DT_CTRL) + 5):
    action, warning = feature.update(takeover, model(), True, False)
    assert action == PostTurnAction.waiting
    assert warning is None

  assert feature.stable_ticks == 0
  assert feature.countdown == 0.0


def test_rapid_near_straight_steering_blocks_resume(mocker):
  feature = PostTurnResume(make_params(mocker))
  assert feature.update(car_state(left=True), model(), True, True)[0] == PostTurnAction.pause

  takeover = car_state(speed_mph=35)
  takeover.steeringPressed = True
  takeover.steeringTorque = 1.0
  takeover.steeringAngleDeg = 2.0
  takeover.steeringRateDeg = 25.0
  action, warning = feature.update(takeover, model(), True, False)

  assert action == PostTurnAction.waiting
  assert warning is None
  assert feature.stable_ticks == 0


def test_strong_driver_takeover_during_countdown_restarts_stability(mocker):
  feature = PostTurnResume(make_params(mocker))
  assert feature.update(car_state(left=True), model(), True, True)[0] == PostTurnAction.pause
  settled = car_state(speed_mph=35)
  for _ in range(round(feature.HIGH_SPEED_STABLE_SECONDS / DT_CTRL) + 2):
    feature.update(settled, model(), True, False)
  assert feature.countdown > 0

  takeover = car_state(speed_mph=35)
  takeover.steeringPressed = True
  takeover.steeringTorque = 2.0
  action, warning = feature.update(takeover, model(), True, False)

  assert action == PostTurnAction.waiting
  assert warning is None
  assert feature.stable_ticks == 0
  assert feature.countdown == 0.0


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


def test_strict_mode_does_not_resume_on_road_edges_alone(mocker):
  feature = PostTurnResume(make_params(mocker))
  feature.update(car_state(right=True), model(), True, True)
  curb_model = model(left_prob=0.1, right_prob=0.1)

  assert not run_until_resume(feature, car_state(speed_mph=15), curb_model)
  assert feature.pending
  assert feature.stable_ticks == 0


def test_relaxed_mode_resumes_with_two_confident_edges_and_safe_path(mocker):
  feature = PostTurnResume(make_params(mocker))
  feature.relaxed_road_edges = True
  feature.update(car_state(right=True), model(), True, True)
  curb_model = model(left_prob=0.1, right_prob=0.1)

  assert run_until_resume(feature, car_state(speed_mph=15), curb_model)


def test_relaxed_mode_tolerates_brief_edge_confidence_flicker_after_qualification(mocker):
  feature = PostTurnResume(make_params(mocker))
  feature.relaxed_road_edges = True
  feature.update(car_state(right=True), model(), True, True)
  CS = car_state(speed_mph=15)
  curb_model = model(left_prob=0.1, right_prob=0.1)
  flicker_model = model(left_prob=0.1, right_prob=0.1, left_edge_std=0.65)

  for _ in range(round(feature.stable_seconds / DT_CTRL) + 1):
    feature.update(CS, curb_model, True, False)
  assert feature.road_edge_recovery_qualified

  for tick in range(round(feature.resume_delay / DT_CTRL) + 5):
    # Refresh strict confidence every 0.4 seconds. The intervening dropout is
    # shorter than the bounded 0.5-second grace period.
    lane_model = curb_model if tick % round(0.4 / DT_CTRL) == 0 else flicker_model
    action, _ = feature.update(CS, lane_model, True, False)
    if action == PostTurnAction.resume:
      break
  else:
    raise AssertionError("brief curb confidence flicker blocked resume")


def test_relaxed_mode_resets_after_sustained_edge_confidence_loss(mocker):
  feature = PostTurnResume(make_params(mocker))
  feature.relaxed_road_edges = True
  feature.update(car_state(right=True), model(), True, True)
  CS = car_state(speed_mph=15)
  curb_model = model(left_prob=0.1, right_prob=0.1)

  for _ in range(round(feature.stable_seconds / DT_CTRL) + 1):
    feature.update(CS, curb_model, True, False)
  assert feature.road_edge_recovery_qualified

  uncertain_model = model(left_prob=0.1, right_prob=0.1, left_edge_std=0.7)
  for _ in range(round(feature.ROAD_EDGE_DROPOUT_GRACE_SECONDS / DT_CTRL) + 2):
    action, _ = feature.update(CS, uncertain_model, True, False)
    assert action == PostTurnAction.waiting

  assert not feature.road_edge_recovery_qualified
  assert feature.stable_ticks == 0
  assert feature.countdown == 0.0
  assert feature.last_road_edge_diagnostics["edge_reject_reason"] == "road_edge_std"


def test_relaxed_mode_rejects_single_uncertain_or_implausible_edges(mocker):
  unsafe_models = [
    model(left_prob=0.1, right_prob=0.1, left_edge_std=0.7),
    model(left_prob=0.1, right_prob=0.1, left_edge=-0.5, right_edge=0.5),
    model(left_prob=0.1, right_prob=0.1, left_edge=-4.0, right_edge=4.0),
    model(left_prob=0.1, right_prob=0.1, path_y=1.7),
  ]
  for unsafe_model in unsafe_models:
    feature = PostTurnResume(make_params(mocker))
    feature.relaxed_road_edges = True
    feature.update(car_state(right=True), model(), True, True)
    assert not run_until_resume(feature, car_state(speed_mph=15), unsafe_model)


def test_relaxed_mode_does_not_use_road_edges_above_turn_speed(mocker):
  feature = PostTurnResume(make_params(mocker))
  feature.relaxed_road_edges = True
  feature.update(car_state(speed_mph=20, right=True), model(), True, True)
  curb_model = model(left_prob=0.1, right_prob=0.1)

  assert not run_until_resume(feature, car_state(speed_mph=50), curb_model)
  assert not feature.high_speed_lane_confirmed


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


def test_opposite_consecutive_turn_replaces_direction_and_resumes_after_final_turn(mocker):
  feature = PostTurnResume(make_params(mocker))
  assert feature.update(car_state(right=True), model(), True, True)[0] == PostTurnAction.pause

  # Confirm MADS paused, then release the first blinker.
  feature.update(car_state(right=True), model(), True, False)
  feature.update(car_state(), model(), True, False)

  # Start an opposite turn before the first sequence can resume.
  feature.high_speed_lane_confirmed = True
  feature.stable_ticks = 20
  feature.countdown = 2.0
  action, warning = feature.update(car_state(left=True), model(), True, False)
  assert action == PostTurnAction.waiting
  assert warning is None
  assert feature.pending
  assert feature.direction == "left"
  assert feature.active_blinker_direction == "left"
  assert not feature.high_speed_lane_confirmed
  assert feature.stable_ticks == 0
  assert feature.countdown == 0.0

  # Only the stable lane after the final blinker is released may resume MADS.
  final_lane = car_state(speed_mph=15)
  for _ in range(round(feature.stable_seconds / DT_CTRL)):
    action, warning = feature.update(final_lane, model(), True, False)
    assert action == PostTurnAction.waiting
    assert warning is None

  for _ in range(round(feature.resume_delay / DT_CTRL) + 2):
    action, _ = feature.update(final_lane, model(), True, False)
    if action == PostTurnAction.resume:
      break
  else:
    raise AssertionError("consecutive-turn sequence did not resume after final stable lane")


def test_same_direction_new_blinker_edge_extends_sequence(mocker):
  feature = PostTurnResume(make_params(mocker))
  feature.update(car_state(right=True), model(), True, True)
  feature.update(car_state(right=True), model(), True, False)
  feature.update(car_state(), model(), True, False)

  feature.stable_ticks = 50
  feature.countdown = 1.0
  feature.update(car_state(right=True), model(), True, False)

  assert feature.direction == "right"
  assert feature.stable_ticks == 0
  assert feature.countdown == 0.0


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
