from types import SimpleNamespace

import pytest
from cereal import log

from openpilot.selfdrive.ui.sunnypilot.onroad.curve_confidence import (
  CurveConfidenceInputUnavailable,
  CurveConfidenceRenderer,
)


class FakeSubMaster(dict):
  def __init__(self, values):
    super().__init__(values)
    self.valid = {name: True for name in values}
    self.recv_frame = {name: 10 for name in values}


def renderer_without_ui():
  renderer = CurveConfidenceRenderer.__new__(CurveConfidenceRenderer)
  renderer._previous_path_y = None
  renderer._last_model_frame = -1
  renderer._latest_result = (100.0, False, "")
  return renderer


def make_ui_state(lane_prob=0.9, edge_std=0.1, desired_curvature=0.01,
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
  calibration = SimpleNamespace(calStatus=log.LiveCalibrationData.Status.calibrated)
  sm = FakeSubMaster({
    "modelV2": model,
    "controlsState": controls,
    "carState": car_state,
    "liveCalibration": calibration,
  })
  return SimpleNamespace(sm=sm, started_frame=1)


def test_confident_curve_scores_normal(mocker):
  state = make_ui_state()
  mocker.patch("openpilot.selfdrive.ui.sunnypilot.onroad.curve_confidence.ui_state", state)

  score, in_curve, reason = renderer_without_ui()._score()

  assert in_curve
  assert score >= 90
  assert reason == "curve prediction"


def test_saturation_and_tracking_error_score_critical(mocker):
  state = make_ui_state(lane_prob=0.1, edge_std=1.0, actual_curvature=0.0, saturated=True)
  mocker.patch("openpilot.selfdrive.ui.sunnypilot.onroad.curve_confidence.ui_state", state)

  score, in_curve, reason = renderer_without_ui()._score()

  assert in_curve
  assert score < CurveConfidenceRenderer.RED_ENTER
  assert reason == "controller tracking"


def test_non_finite_input_is_safely_unavailable(mocker):
  state = make_ui_state()
  state.sm["controlsState"].desiredCurvature = float("nan")
  mocker.patch("openpilot.selfdrive.ui.sunnypilot.onroad.curve_confidence.ui_state", state)

  with pytest.raises(CurveConfidenceInputUnavailable):
    renderer_without_ui()._score()
