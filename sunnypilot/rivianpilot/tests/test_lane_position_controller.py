from types import SimpleNamespace
from unittest.mock import MagicMock  # noqa: TID251

from opendbc.car import structs

from openpilot.sunnypilot.rivianpilot.lane_position_controller import LanePositionController


def params():
  values = {
    "RivianPilotLanePositionObserve": True,
    "RivianPilotLanePositionGoLive": True,
    "RivianPilotFeatureLogging": False,
    "RivianPilotCurveOffset": True,
    "RivianPilotLanePositionRelaxed": False,
    "RivianPilotNudgeOffset": True,
    "RivianPilotCurveOffsetInches": 3,
    "RivianPilotCurveThreshold": 35,
    "RivianPilotNudgeOffsetInches": 3,
    "RivianPilotNudgeHoldSeconds": 10,
    "CameraOffset": 0.0,
  }
  p = MagicMock()
  p.get_bool.side_effect = lambda key: bool(values[key])
  p.get.side_effect = lambda key, **kwargs: values[key]
  return p


def car_state():
  return SimpleNamespace(
    gearShifter=structs.CarState.GearShifter.drive,
    vEgo=30.0,
    steeringTorque=0.0,
    steeringPressed=False,
    leftBlinker=False,
    rightBlinker=False,
  )


def model(probability=0.9, lane_half_width=1.8, path_y=0.0):
  x = [0.0, 10.0, 20.0, 30.0]
  return SimpleNamespace(
    laneLineProbs=[0.0, probability, probability, 0.0],
    laneLines=[
      SimpleNamespace(x=x, y=[0.0] * 4),
      SimpleNamespace(x=x, y=[-lane_half_width] * 4),
      SimpleNamespace(x=x, y=[lane_half_width] * 4),
    ],
    position=SimpleNamespace(x=x, y=[path_y] * 4),
  )


def establish_curve(feature, cs=None, road_model=None, curvature=0.002, start=1.0):
  cs = cs or car_state()
  road_model = road_model or model()
  controls = SimpleNamespace(desiredCurvature=curvature)
  for now in (start, start + 0.2, start + 0.4, start + 0.9, start + 1.4):
    feature.update(cs, True, road_model, controls, now=now)
  return controls


def test_curve_offset_is_bounded_and_away_from_inside():
  p = params()
  feature = LanePositionController(p)
  establish_curve(feature, curvature=0.002)
  assert feature.last_output < 0.0
  assert abs(feature.last_output) <= 3 * 0.0254

  feature = LanePositionController(p)
  establish_curve(feature, curvature=-0.002)
  assert feature.last_output > 0.0
  assert abs(feature.last_output) <= 3 * 0.0254


def test_dynamic_offset_params_are_written_as_runtime_float_types():
  p = params()
  LanePositionController(p)

  offset_write = next(call for call in p.put.call_args_list if call.args[0] == "RivianPilotDynamicCameraOffset")
  heartbeat_write = next(call for call in p.put.call_args_list if call.args[0] == "RivianPilotDynamicCameraOffsetUpdated")
  assert isinstance(offset_write.args[1], float)
  assert isinstance(heartbeat_write.args[1], float)


def test_low_confidence_blocks_automatic_curve_offset():
  p = params()
  feature = LanePositionController(p)
  establish_curve(feature, road_model=model(probability=0.2))
  assert not feature.faulted
  assert feature.last_output == 0.0


def test_relaxed_geometry_is_explicit_and_bounded_to_two_inches():
  p = params()
  original_get_bool = p.get_bool.side_effect
  p.get_bool.side_effect = lambda key: True if key == "RivianPilotLanePositionRelaxed" else original_get_bool(key)
  feature = LanePositionController(p)
  establish_curve(feature, road_model=model(probability=0.2))
  assert feature.last_output == -2 * 0.0254


def test_driver_input_immediately_publishes_zero_then_latches_nudge():
  p = params()
  feature = LanePositionController(p)
  cs = car_state()
  cs.steeringPressed = True
  cs.steeringTorque = 2.0
  feature.update(cs, True, model(), SimpleNamespace(desiredCurvature=0.0), now=1.0)
  assert feature.last_output == 0.0
  cs.steeringPressed = False
  cs.steeringTorque = 0.0
  feature.update(cs, True, model(), SimpleNamespace(desiredCurvature=0.0), now=1.2)
  assert feature.last_output > 0.0


def test_manual_nudge_is_authoritative_without_lane_geometry():
  p = params()
  feature = LanePositionController(p)
  cs = car_state()
  feature.nudge_direction = 1
  feature.nudge_until = 20.0
  feature.update(cs, True, model(probability=0.0), SimpleNamespace(desiredCurvature=0.0), now=1.0)
  assert feature.last_output == 3 * 0.0254

  feature = LanePositionController(p)
  feature.nudge_direction = -1
  feature.nudge_until = 20.0
  feature.update(cs, True, model(probability=0.0), SimpleNamespace(desiredCurvature=0.0), now=1.2)
  assert feature.last_output == -3 * 0.0254


def test_automatic_guard_uses_only_movement_side_clearance():
  p = params()
  cs = car_state()
  # Positive curvature is a left curve and requests movement driver-right.
  # A close left/inside boundary must not cap that outward movement.
  feature = LanePositionController(p)
  establish_curve(feature, cs, model(lane_half_width=1.5, path_y=-0.3))
  assert feature.last_output == -3 * 0.0254

  # On the same left curve, a close right/outside boundary caps only movement
  # toward that boundary.
  feature = LanePositionController(p)
  establish_curve(feature, cs, model(lane_half_width=1.5, path_y=0.3))
  assert -3 * 0.0254 < feature.last_output <= 0.0


def test_manual_nudge_cancels_and_suppresses_automatic_curve():
  p = params()
  feature = LanePositionController(p)
  cs = car_state()
  controls = establish_curve(feature, cs)
  assert feature.last_output < 0.0

  cs.steeringPressed = True
  cs.steeringTorque = 2.0
  feature.update(cs, True, model(), controls, now=2.6)
  assert feature.last_output == 0.0
  assert not feature.curve_active

  cs.steeringPressed = False
  cs.steeringTorque = 0.0
  feature.update(cs, True, model(), controls, now=2.8)
  assert feature.last_output == 3 * 0.0254
  assert not feature.curve_active


def test_wide_lane_never_exceeds_configured_curve_offset():
  p = params()
  feature = LanePositionController(p)
  establish_curve(feature, road_model=model(lane_half_width=2.0, path_y=-0.5))
  assert feature.last_output == -3 * 0.0254


def test_curve_reference_is_captured_before_offset_and_held_for_episode():
  p = params()
  feature = LanePositionController(p)
  cs = car_state()
  controls = SimpleNamespace(desiredCurvature=0.002)
  original_model = model(lane_half_width=1.8, path_y=0.0)
  for now in (1.0, 1.2, 1.4, 1.9, 2.4):
    feature.update(cs, True, original_model, controls, now=now)
  reference = feature.curve_reference_clearance
  assert reference is not None
  assert feature.last_output == -3 * 0.0254

  # Simulate the model path reacting to the camera transform. The pre-offset
  # reference remains stable instead of canceling the feature's own output.
  feature.update(cs, True, model(lane_half_width=1.8, path_y=0.5), controls, now=2.6)
  assert feature.curve_reference_clearance == reference
  assert feature.last_output == -3 * 0.0254


def test_persistent_geometry_loss_ramps_automatic_offset_out():
  p = params()
  feature = LanePositionController(p)
  cs = car_state()
  controls = establish_curve(feature, cs)
  assert feature.last_output < 0.0

  weak_model = model(probability=0.2)
  feature.update(cs, True, weak_model, controls, now=2.6)
  feature.update(cs, True, weak_model, controls, now=2.8)
  assert feature.last_output < 0.0
  feature.update(cs, True, weak_model, controls, now=3.0)
  assert -3 * 0.0254 < feature.last_output < 0.0
  feature.update(cs, True, weak_model, controls, now=3.2)
  assert feature.last_output == 0.0


def test_curve_hysteresis_releases_below_eighty_percent_threshold():
  p = params()
  feature = LanePositionController(p)
  cs = car_state()
  establish_curve(feature, cs)
  assert feature.curve_active

  # Thirty percent strength remains active below the 35% entry threshold but
  # above the 28% release threshold.
  feature.update(cs, True, model(), SimpleNamespace(desiredCurvature=0.001), now=2.6)
  assert feature.curve_active
  feature.update(cs, True, model(), SimpleNamespace(desiredCurvature=0.0005), now=3.0)
  assert not feature.curve_active
  assert feature.last_output == 0.0


def test_observe_only_never_publishes_nonzero():
  p = params()
  p.get_bool.side_effect = lambda key: False if key == "RivianPilotLanePositionGoLive" else bool({
    "RivianPilotLanePositionObserve": True, "RivianPilotFeatureLogging": False,
    "RivianPilotCurveOffset": True, "RivianPilotLanePositionRelaxed": False, "RivianPilotNudgeOffset": True,
  }[key])
  feature = LanePositionController(p)
  establish_curve(feature)
  assert feature.last_output == 0.0


def test_go_live_includes_observation_without_requiring_observe():
  p = params()
  p.get_bool.side_effect = lambda key: bool({
    "RivianPilotLanePositionObserve": False, "RivianPilotLanePositionGoLive": True,
    "RivianPilotFeatureLogging": False, "RivianPilotCurveOffset": True, "RivianPilotLanePositionRelaxed": False,
    "RivianPilotNudgeOffset": True,
  }[key])
  feature = LanePositionController(p)
  establish_curve(feature)
  assert feature.last_output < 0.0


def test_go_live_logs_torque_and_controller_diagnostics(mocker):
  p = params()
  p.get_bool.side_effect = lambda key: bool({
    "RivianPilotLanePositionObserve": True, "RivianPilotLanePositionGoLive": True,
    "RivianPilotFeatureLogging": True, "RivianPilotCurveOffset": True, "RivianPilotLanePositionRelaxed": False,
    "RivianPilotNudgeOffset": True,
  }[key])
  feature = LanePositionController(p)
  cs = car_state()
  cs.steeringTorqueEps = 0.2
  cs.steeringAngleDeg = 1.5
  cs.steeringRateDeg = 0.4
  cs.yawRate = 0.01
  cs.aEgo = -0.1
  lateral_log = SimpleNamespace(saturated=True, output=0.21)
  lateral_state = SimpleNamespace(which=lambda: "torqueState", torqueState=lateral_log)
  controls = SimpleNamespace(desiredCurvature=0.002, curvature=0.0018, lateralControlState=lateral_state)
  car_control = SimpleNamespace(actuators=SimpleNamespace(torque=0.22))
  car_output = SimpleNamespace(actuatorsOutput=SimpleNamespace(torque=0.20))
  event = mocker.patch("openpilot.sunnypilot.rivianpilot.lane_position_controller.cloudlog.event")

  feature.update(cs, True, model(), controls, car_control, car_output, now=1.0)

  sample = next(call for call in event.call_args_list if call.kwargs.get("action") == "sample")
  assert sample.kwargs["eps_torque"] == 0.2
  assert sample.kwargs["requested_torque"] == 0.22
  assert sample.kwargs["applied_torque"] == 0.2
  assert sample.kwargs["controller_type"] == "torqueState"
  assert sample.kwargs["controller_saturated"]


def test_diagnostic_failure_does_not_disable_lane_position(mocker):
  p = params()
  p.get_bool.side_effect = lambda key: bool({
    "RivianPilotLanePositionObserve": True, "RivianPilotLanePositionGoLive": True,
    "RivianPilotFeatureLogging": True, "RivianPilotCurveOffset": True, "RivianPilotLanePositionRelaxed": False,
    "RivianPilotNudgeOffset": True,
  }[key])
  feature = LanePositionController(p)
  mocker.patch.object(feature, "_diagnostics", side_effect=RuntimeError("diagnostic failure"))

  feature.update(car_state(), True, model(), SimpleNamespace(desiredCurvature=0.002), now=1.0)

  assert feature.diagnostic_faulted
  assert not feature.faulted
  assert feature.last_output == 0.0


def test_blinker_cancels_offset():
  p = params()
  feature = LanePositionController(p)
  cs = car_state()
  establish_curve(feature, cs)
  assert feature.last_output != 0.0
  cs.leftBlinker = True
  feature.update(cs, True, model(), SimpleNamespace(desiredCurvature=0.002), now=2.6)
  assert feature.last_output == 0.0


def test_runtime_failure_falls_back_to_zero_and_stops_heartbeat():
  p = params()
  feature = LanePositionController(p)
  establish_curve(feature)
  assert feature.last_output != 0.0

  heartbeat_writes = sum(call.args[0] == "RivianPilotDynamicCameraOffsetUpdated" for call in p.put.call_args_list)
  feature.suppress_after_error(RuntimeError("injected failure"))

  assert feature.faulted
  assert feature.last_output == 0.0
  zero_write = next(call for call in reversed(p.put.call_args_list)
                    if call.args[0] == "RivianPilotDynamicCameraOffset")
  assert zero_write.args[1] == 0.0
  assert sum(call.args[0] == "RivianPilotDynamicCameraOffsetUpdated" for call in p.put.call_args_list) == heartbeat_writes + 1


def test_params_write_failure_is_contained_by_outer_suppression():
  p = params()
  feature = LanePositionController(p)
  p.put.side_effect = RuntimeError("injected params failure")

  try:
    feature.update(car_state(), True, model(), SimpleNamespace(desiredCurvature=0.002), now=1.0)
  except RuntimeError as e:
    feature.suppress_after_error(e)

  assert feature.faulted
  assert feature.last_output == 0.0
