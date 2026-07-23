from types import SimpleNamespace

import pytest

from openpilot.sunnypilot.rivianpilot.curve_confidence import (
  CurveConfidenceInputUnavailable,
  calculate_curve_confidence,
)


def make_inputs(lane_prob=0.9, edge_std=0.1, desired_curvature=0.01,
                actual_curvature=0.01, saturated=False):
  model = SimpleNamespace(
    laneLineProbs=[0.0, lane_prob, lane_prob, 0.0],
    roadEdgeStds=[edge_std, edge_std],
    position=SimpleNamespace(y=[0.0] * 20),
  )
  lateral_log = SimpleNamespace(saturated=saturated)
  lateral_state = SimpleNamespace(which=lambda: "torqueState", torqueState=lateral_log)
  controls = SimpleNamespace(desiredCurvature=desired_curvature, curvature=actual_curvature,
                             lateralControlState=lateral_state)
  car_state = SimpleNamespace(vEgo=10.0, steeringPressed=False, steeringTorque=0.0)
  return model, controls, car_state


def test_confident_curve_scores_normal():
  score, in_curve, reason, _ = calculate_curve_confidence(*make_inputs(), True, None)

  assert in_curve
  assert score >= 90
  assert reason == "curve prediction"


def test_saturation_and_tracking_error_score_critical():
  inputs = make_inputs(lane_prob=0.1, edge_std=1.0, actual_curvature=0.0, saturated=True)
  score, in_curve, reason, _ = calculate_curve_confidence(*inputs, True, None)

  assert in_curve
  assert score < 40
  assert reason == "controller tracking"


def test_non_finite_input_is_safely_unavailable():
  model, controls, car_state = make_inputs()
  controls.desiredCurvature = float("nan")

  with pytest.raises(CurveConfidenceInputUnavailable):
    calculate_curve_confidence(model, controls, car_state, True, None)
