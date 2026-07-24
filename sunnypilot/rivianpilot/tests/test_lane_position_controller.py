from types import SimpleNamespace
from unittest.mock import MagicMock

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


def model(probability=0.9, lane_half_width=1.8):
  x = [0.0, 10.0, 20.0, 30.0]
  return SimpleNamespace(
    laneLineProbs=[0.0, probability, probability, 0.0],
    laneLines=[
      SimpleNamespace(x=x, y=[0.0] * 4),
      SimpleNamespace(x=x, y=[lane_half_width] * 4),
      SimpleNamespace(x=x, y=[-lane_half_width] * 4),
    ],
    position=SimpleNamespace(x=x, y=[0.0] * 4),
  )


def test_curve_offset_is_bounded_and_away_from_inside():
  p = params()
  feature = LanePositionController(p)
  feature.update(car_state(), True, model(), SimpleNamespace(desiredCurvature=0.002), now=1.0)
  assert feature.last_output < 0.0
  assert abs(feature.last_output) <= 3 * 0.0254


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


def test_observe_only_never_publishes_nonzero():
  p = params()
  p.get_bool.side_effect = lambda key: False if key == "RivianPilotLanePositionGoLive" else bool({
    "RivianPilotLanePositionObserve": True, "RivianPilotFeatureLogging": False,
    "RivianPilotCurveOffset": True, "RivianPilotNudgeOffset": True,
  }[key])
  feature = LanePositionController(p)
  feature.update(car_state(), True, model(), SimpleNamespace(desiredCurvature=0.002), now=1.0)
  assert feature.last_output == 0.0


def test_blinker_cancels_offset():
  p = params()
  feature = LanePositionController(p)
  cs = car_state()
  feature.update(cs, True, model(), SimpleNamespace(desiredCurvature=0.002), now=1.0)
  assert feature.last_output != 0.0
  cs.leftBlinker = True
  feature.update(cs, True, model(), SimpleNamespace(desiredCurvature=0.002), now=1.2)
  assert feature.last_output == 0.0
