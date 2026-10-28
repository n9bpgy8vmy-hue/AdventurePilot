"""Backport wiring checks: preserve upstream timer blocking and nudge override."""
from types import SimpleNamespace
import pytest
from cereal import log
from openpilot.selfdrive.controls.lib.desire_helper import DesireHelper
from openpilot.sunnypilot.selfdrive.controls.lib.relc import RoadEdgeLaneChangeController


@pytest.mark.parametrize("side", [-1, 1])
@pytest.mark.parametrize("nudge,blindspot", [(False, False), (True, False), (True, True)])
def test_edge_guard_initiation(mocker, side, nudge, blindspot):
  params = mocker.Mock()
  params.get.side_effect = lambda key, **kwargs: {"AutoLaneChangeTimer": 1, "LaneTurnValue": 0}[key]
  params.get_bool.side_effect = lambda key: key == "RoadEdgeLaneChangeEnabled"
  for module in ("auto_lane_change", "lane_turn_desire", "relc"):
    mocker.patch(f"openpilot.sunnypilot.selfdrive.controls.lib.{module}.Params", return_value=params)
  guard, helper = RoadEdgeLaneChangeController(), DesireHelper()
  edges = [SimpleNamespace(y=[-2.0]), SimpleNamespace(y=[2.0])]
  # Establish detection before the driver's request, using upstream thresholds.
  for _ in range(30):
    guard.update([0.0, 0.0], [0.0, 0.9, 0.9, 0.0], 25.0, edges)
  assert guard.left_edge_detected and guard.right_edge_detected
  cs = SimpleNamespace(vEgo=25.0, leftBlinker=side == -1, rightBlinker=side == 1,
                       steeringPressed=nudge, steeringTorque=-side if nudge else 0.0,
                       leftBlindspot=blindspot, rightBlindspot=blindspot, brakePressed=False)
  for _ in range(10):
    helper.update(cs, True, 1.0, guard.left_edge_detected, guard.right_edge_detected)
  expected = log.LaneChangeState.laneChangeStarting if nudge and not blindspot else log.LaneChangeState.preLaneChange
  assert helper.lane_change_state == expected
