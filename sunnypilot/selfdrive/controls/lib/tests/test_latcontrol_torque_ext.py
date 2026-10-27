"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
import numpy as np
import pytest

from cereal import log, messaging
from opendbc.car.structs import car
from opendbc.car.car_helpers import interfaces
from opendbc.car.honda.values import CAR as HONDA
from opendbc.car.rivian.values import CAR as RIVIAN
from opendbc.car.vehicle_model import VehicleModel
from openpilot.common.params import Params
from openpilot.common.realtime import DT_CTRL
from openpilot.selfdrive.car.helpers import convert_to_capnp
from openpilot.selfdrive.controls.lib.latcontrol_torque import LatControlTorque
from openpilot.sunnypilot.selfdrive.controls.lib.latcontrol_torque_v0 import LatControlTorque as LatControlTorqueV0
from openpilot.selfdrive.locationd.helpers import Pose
from openpilot.common.mock.generators import generate_livePose
from openpilot.sunnypilot.selfdrive.car import interfaces as sunnypilot_interfaces
from openpilot.selfdrive.modeld.constants import ModelConstants


def _make_controller(enhanced=False, nnlc=False, car_name=HONDA.HONDA_CIVIC, legacy=False):
  params = Params()
  params.put_bool("EnforceTorqueControl", True, block=True)
  params.put_bool("LateralJerkTorqueController", enhanced, block=True)
  params.put_bool("NeuralNetworkLateralControl", nnlc, block=True)

  CarInterface = interfaces[car_name]
  CP = CarInterface.get_non_essential_params(car_name)
  CP_SP = CarInterface.get_non_essential_params_sp(CP, car_name)
  CI = CarInterface(CP, CP_SP)
  sunnypilot_interfaces.setup_interfaces(CI, params)
  CP_SP = convert_to_capnp(CP_SP)
  VM = VehicleModel(CP)
  controller_class = LatControlTorqueV0 if legacy else LatControlTorque
  controller = controller_class(CP.as_reader(), CP_SP.as_reader(), CI, DT_CTRL)
  return controller, VM, CP


def _make_model_v2():
  model = messaging.new_message('modelV2')
  position = log.XYZTData.new_message()
  position.x = [float(x) for x in 30.0 * np.array(ModelConstants.T_IDXS)]
  model.modelV2.position = position
  orientation = log.XYZTData.new_message()
  orientation.x = [0.0 for _ in ModelConstants.T_IDXS]
  orientation.y = [0.0 for _ in ModelConstants.T_IDXS]
  model.modelV2.orientation = orientation
  velocity = log.XYZTData.new_message()
  velocity.x = [30.0 for _ in ModelConstants.T_IDXS]
  model.modelV2.velocity = velocity
  acceleration = log.XYZTData.new_message()
  acceleration.x = [0.0 for _ in ModelConstants.T_IDXS]
  acceleration.y = [0.0 for _ in ModelConstants.T_IDXS]
  model.modelV2.acceleration = acceleration
  return model


def _run_update(controller, VM, curvature=0.5):
  CS = car.CarState.new_message()
  CS.vEgo = 30
  CS.steeringPressed = False
  lp = generate_livePose()
  pose = Pose.from_live_pose(lp.livePose)
  params = log.LiveParametersData.new_message()
  model_v2 = _make_model_v2().modelV2
  controller.extension.update_model_v2(model_v2)
  controller.extension.update_lateral_lag(0.2)
  return controller.update(True, CS, VM, params, False, curvature, pose, False, 0.2)


class TestLatControlTorqueExt:
  def test_init_enhanced_only(self):
    controller, VM, _ = _make_controller(enhanced=True, nnlc=False)
    assert controller.extension._jerk_aware_enabled
    assert not controller.extension.enabled  # NNLC disabled

  def test_init_nnlc_only(self):
    controller, VM, _ = _make_controller(enhanced=False, nnlc=True)
    assert not controller.extension._jerk_aware_enabled
    assert controller.extension.enabled

  def test_init_neither(self):
    controller, VM, _ = _make_controller(enhanced=False, nnlc=False)
    assert not controller.extension._jerk_aware_enabled
    assert not controller.extension.enabled

  def test_init_both_no_crash(self):
    controller, VM, _ = _make_controller(enhanced=True, nnlc=True)
    assert not controller.extension._jerk_aware_enabled
    assert not controller.extension.enabled

  def test_update_enhanced_only(self):
    controller, VM, _ = _make_controller(enhanced=True, nnlc=False)
    output_torque, _, pid_log = _run_update(controller, VM)
    assert pid_log.active

  def test_update_neither(self):
    controller, VM, _ = _make_controller(enhanced=False, nnlc=False)
    output_torque, _, pid_log = _run_update(controller, VM)
    assert pid_log.active

  def test_update_both_no_crash(self):
    controller, VM, _ = _make_controller(enhanced=True, nnlc=True)
    output_torque, _, pid_log = _run_update(controller, VM)
    assert pid_log.active

  @pytest.mark.parametrize("enhanced", [False, True])
  def test_rivian_controller_initialization_and_update(self, enhanced):
    controller, VM, _ = _make_controller(enhanced=enhanced, car_name=RIVIAN.RIVIAN_R1)
    assert controller.extension._jerk_aware_enabled == enhanced
    output, _, pid_log = _run_update(controller, VM)
    assert pid_log.active
    assert np.isfinite(output)

  @pytest.mark.parametrize("legacy", [False, True])
  @pytest.mark.parametrize("curvature", [-0.5, 0.5])
  def test_jerk_controller_respects_limits_before_and_after_live_tuning(self, legacy, curvature):
    controller, VM, _ = _make_controller(enhanced=True, car_name=RIVIAN.RIVIAN_R1, legacy=legacy)
    for frame in range(20):
      if frame == 10:
        tuning = controller.torque_params
        controller.update_live_torque_params(tuning.latAccelFactor * 1.1, tuning.latAccelOffset, tuning.friction)
      output, _, _ = _run_update(controller, VM, curvature)
      assert np.isfinite(output)
      assert abs(output) <= controller.steer_max
