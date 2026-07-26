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
    "RivianPilotNudgeOffset": True,
    "RivianPilotCurveOffsetInches": 3,
    "RivianPilotCurveThreshold": 35,
    "RivianPilotNudgeOffsetInches": 3,
    "RivianPilotNudgeHoldSeconds": 10,
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
      SimpleNamespace(x=x, y=[lane_half_width] * 4),
      SimpleNamespace(x=x, y=[-lane_half_width] * 4),
    ],
    position=SimpleNamespace(x=x, y=[path_y] * 4),
  )


def test_curve_offset_is_bounded_and_away_from_inside():
  p = params()
  feature = LanePositionController(p)
  feature.update(car_state(), True, model(), SimpleNamespace(desiredCurvature=0.002), now=1.0)
  assert feature.last_output < 0.0
  assert abs(feature.last_output) <= 3 * 0.0254


def test_dynamic_offset_params_are_written_as_runtime_float_types():
  p = params()
  LanePositionController(p)

  offset_write = next(call for call in p.put.call_args_list if call.args[0] == "RivianPilotDynamicCameraOffset")
  heartbeat_write = next(call for call in p.put.call_args_list if call.args[0] == "RivianPilotDynamicCameraOffsetUpdated")
  assert isinstance(offset_write.args[1], float)
  assert isinstance(heartbeat_write.args[1], float)


def test_low_confidence_suppresses_feature():
  p = params()
  feature = LanePositionController(p)
  feature.update(car_state(), True, model(probability=0.2), SimpleNamespace(desiredCurvature=0.002), now=1.0)
  assert not feature.faulted
  assert feature.last_output == 0.0


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


def test_nudge_uses_clearance_in_requested_direction():
  p = params()
  feature = LanePositionController(p)
  cs = car_state()
  close_to_right = model(path_y=-0.6)

  # Positive CameraOffset moves the model center driver-left, away from the
  # close right boundary, so the full configured nudge remains available.
  feature.nudge_direction = 1
  feature.nudge_until = 20.0
  feature.update(cs, True, close_to_right, SimpleNamespace(desiredCurvature=0.0), now=1.0)
  assert feature.last_output == 3 * 0.0254

  # The same geometry must block a driver-right request toward that boundary.
  feature = LanePositionController(p)
  feature.nudge_direction = -1
  feature.nudge_until = 20.0
  feature.update(cs, True, close_to_right, SimpleNamespace(desiredCurvature=0.0), now=1.2)
  assert feature.last_output == 0.0


def test_directional_guard_is_symmetric():
  p = params()
  feature = LanePositionController(p)
  cs = car_state()
  close_to_left = model(path_y=0.6)

  feature.nudge_direction = -1
  feature.nudge_until = 20.0
  feature.update(cs, True, close_to_left, SimpleNamespace(desiredCurvature=0.0), now=1.0)
  assert feature.last_output == -3 * 0.0254

  feature = LanePositionController(p)
  feature.nudge_direction = 1
  feature.nudge_until = 20.0
  feature.update(cs, True, close_to_left, SimpleNamespace(desiredCurvature=0.0), now=1.2)
  assert feature.last_output == 0.0


def test_observe_only_never_publishes_nonzero():
  p = params()
  p.get_bool.side_effect = lambda key: False if key == "RivianPilotLanePositionGoLive" else bool({
    "RivianPilotLanePositionObserve": True, "RivianPilotFeatureLogging": False,
    "RivianPilotCurveOffset": True, "RivianPilotNudgeOffset": True,
  }[key])
  feature = LanePositionController(p)
  feature.update(car_state(), True, model(), SimpleNamespace(desiredCurvature=0.002), now=1.0)
  assert feature.last_output == 0.0


def test_go_live_includes_observation_without_requiring_observe():
  p = params()
  p.get_bool.side_effect = lambda key: bool({
    "RivianPilotLanePositionObserve": False, "RivianPilotLanePositionGoLive": True,
    "RivianPilotFeatureLogging": False, "RivianPilotCurveOffset": True,
    "RivianPilotNudgeOffset": True,
  }[key])
  feature = LanePositionController(p)
  feature.update(car_state(), True, model(), SimpleNamespace(desiredCurvature=0.002), now=1.0)
  assert feature.last_output < 0.0


def test_go_live_logs_torque_and_controller_diagnostics(mocker):
  p = params()
  p.get_bool.side_effect = lambda key: bool({
    "RivianPilotLanePositionObserve": True, "RivianPilotLanePositionGoLive": True,
    "RivianPilotFeatureLogging": True, "RivianPilotCurveOffset": True,
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
    "RivianPilotFeatureLogging": True, "RivianPilotCurveOffset": True,
    "RivianPilotNudgeOffset": True,
  }[key])
  feature = LanePositionController(p)
  mocker.patch.object(feature, "_diagnostics", side_effect=RuntimeError("diagnostic failure"))

  feature.update(car_state(), True, model(), SimpleNamespace(desiredCurvature=0.002), now=1.0)

  assert feature.diagnostic_faulted
  assert not feature.faulted
  assert feature.last_output < 0.0


def test_blinker_cancels_offset():
  p = params()
  feature = LanePositionController(p)
  cs = car_state()
  feature.update(cs, True, model(), SimpleNamespace(desiredCurvature=0.002), now=1.0)
  assert feature.last_output != 0.0
  cs.leftBlinker = True
  feature.update(cs, True, model(), SimpleNamespace(desiredCurvature=0.002), now=1.2)
  assert feature.last_output == 0.0
