"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
import numpy as np
import pytest

from openpilot.common.transformations.camera import DEVICE_CAMERAS, view_frame_from_device_frame
from openpilot.common.transformations.model import get_warp_matrix
from openpilot.common.transformations.orientation import rot_from_euler
from openpilot.sunnypilot.modeld_v2.camera_offset_helper import CameraOffsetHelper


class MockStruct:
  def __init__(self, **kwargs):
    for k, v in kwargs.items():
      setattr(self, k, v)

  def __getitem__(self, item):
    return getattr(self, item)


class TestCameraOffset:
  def setup_method(self):
    self.camera_offset = CameraOffsetHelper()
    self.dc = DEVICE_CAMERAS[('mici', 'os04c10')]

  def test_smoothing(self):
    self.camera_offset.set_offset(0.2)

    sm = MockStruct(
      deviceState=MockStruct(deviceType='mici'),
      roadCameraState=MockStruct(sensor='os04c10'),
      liveCalibration=MockStruct(rpyCalib=[0.0, 0.0, 0.0], height=[1.22])
    )

    intrinsics_main = self.dc.fcam.intrinsics
    intrinsics_extra = self.dc.ecam.intrinsics
    device_from_calib_euler = np.array([0.0, 0.0, 0.0], dtype=np.float32)
    main_transform = get_warp_matrix(device_from_calib_euler, intrinsics_main, False).astype(np.float32)
    extra_transform = get_warp_matrix(device_from_calib_euler, intrinsics_extra, True).astype(np.float32)

    self.camera_offset.update(main_transform, extra_transform, sm, False)
    np.testing.assert_almost_equal(self.camera_offset.actual_camera_offset, 0.02)
    self.camera_offset.update(main_transform, extra_transform, sm, False)
    np.testing.assert_almost_equal(self.camera_offset.actual_camera_offset, 0.038)

  def test_immediate_offset_bypasses_filter(self):
    self.camera_offset.set_offset(0.12, immediate=True)
    assert self.camera_offset.camera_offset == 0.12
    assert self.camera_offset.actual_camera_offset == 0.12

  def test_rivianpilot_dynamic_offset_watchdog(self):
    valid = self.camera_offset.valid_rivianpilot_dynamic_offset
    assert valid(0.10, 9.5, 10.0)
    assert valid(15 * 0.0254, 9.5, 10.0)
    assert not valid(float("nan"), 9.5, 10.0)
    assert not valid(15.1 * 0.0254, 9.5, 10.0)
    assert not valid(0.10, 8.9, 10.0)
    assert not valid("invalid", 9.5, 10.0)

  def test_apply_camera_offset(self):
    intrinsics = self.dc.fcam.intrinsics
    v_horizon = CameraOffsetHelper.get_v_horizon(intrinsics, [])  # pitch = 0 fallback: v_horizon == cy
    transform = np.eye(3, dtype=np.float32)
    height = 1.22
    offset = 0.1

    expected_shear = np.eye(3, dtype=np.float32)
    expected_shear[0, 1] = offset / height
    expected_shear[0, 2] = -offset / height * v_horizon

    result = CameraOffsetHelper.apply_camera_offset(transform, height, offset, v_horizon)
    np.testing.assert_array_almost_equal(result, expected_shear)

  def test_v_horizon_empty_rpy(self):
    intrinsics = self.dc.fcam.intrinsics
    v_horizon = CameraOffsetHelper.get_v_horizon(intrinsics, [])
    np.testing.assert_almost_equal(v_horizon, intrinsics[1, 2])

  def test_v_horizon_projection(self):
    intrinsics = self.dc.fcam.intrinsics
    f, cy = intrinsics[1, 1], intrinsics[1, 2]

    for pitch_deg in [6.0, -6.0, 0.0]:
      rpy = [0.0, np.radians(pitch_deg), 0.0]
      d_dev = rot_from_euler(rpy) @ np.array([1.0, 0.0, 0.0])
      view = view_frame_from_device_frame @ d_dev
      expected = cy + f * view[1] / view[2]

      v_horizon = CameraOffsetHelper.get_v_horizon(intrinsics, rpy)
      np.testing.assert_almost_equal(v_horizon, expected, decimal=4)

  def test_update(self):
    height = 1.2
    pitch = np.radians(-8.0)

    sm = MockStruct(
      deviceState=MockStruct(deviceType='mici'),
      roadCameraState=MockStruct(sensor='os04c10'),
      liveCalibration=MockStruct(rpyCalib=[0.0, pitch, 0.0], height=[height])
    )
    intrinsics_main = self.dc.fcam.intrinsics
    intrinsics_extra = self.dc.ecam.intrinsics
    device_from_calib_euler = np.array(sm['liveCalibration'].rpyCalib, dtype=np.float32)
    main_transform = get_warp_matrix(device_from_calib_euler, intrinsics_main, False).astype(np.float32)
    extra_transform = get_warp_matrix(device_from_calib_euler, intrinsics_extra, True).astype(np.float32)

    self.camera_offset.set_offset(0.0)  # test default offset doesn't change transformation
    main_out, extra_out = self.camera_offset.update(main_transform, extra_transform, sm, False)
    np.testing.assert_array_equal(main_out, main_transform)
    np.testing.assert_array_equal(extra_out, extra_transform)

    self.camera_offset.set_offset(0.2)  # test valid offset changes transformation
    main_out, extra_out = self.camera_offset.update(main_transform, extra_transform, sm, False)
    assert not np.array_equal(main_out, main_transform)
    assert not np.array_equal(extra_out, extra_transform)

    # settle the low-pass filter
    for _ in range(100):
      main_out, extra_out = self.camera_offset.update(main_transform, extra_transform, sm, False)

    # undo main_transform dot product to get shear matrix
    shear = main_out @ np.linalg.inv(main_transform)
    expected_v_horizon = intrinsics_main[1, 2] - intrinsics_main[1, 1] * np.tan(pitch)
    np.testing.assert_almost_equal(shear[0, 1], self.camera_offset.actual_camera_offset / height, decimal=4)
    np.testing.assert_almost_equal(shear[0, 2], -self.camera_offset.actual_camera_offset / height * expected_v_horizon, decimal=4)

def test_controller_handoff_is_consumed_without_zero_jump():
  from types import SimpleNamespace
  from openpilot.sunnypilot.rivianpilot.tests.test_lane_position_controller import params, car_state, model, establish_curve
  from openpilot.sunnypilot.rivianpilot.lane_position_controller import LanePositionController
  feature = LanePositionController(params())
  cs = car_state()
  establish_curve(feature, cs)
  helper = CameraOffsetHelper()
  base = 0.02
  helper.set_offset(base + feature.last_output, immediate=True)
  before = helper.actual_camera_offset
  cs.leftBlinker = True
  feature.update(cs, True, model(), SimpleNamespace(desiredCurvature=0.002), now=2.6)
  helper.set_rivianpilot_offset(base, feature.last_output, 2.6, 2.6, steering_pressed=False, in_drive=True, lat_active=True)
  assert base < helper.camera_offset < before
  assert helper.actual_camera_offset == before  # next model update applies smoothing
  # Hard overrides and a stale producer still remove the transform immediately.
  for overrides in ({"steering_pressed": True}, {"in_drive": False}, {"lat_active": False}, {"updated": 1.0}):
    args = dict(updated=2.6, now=2.6, steering_pressed=False, in_drive=True, lat_active=True)
    args.update(overrides)
    helper.set_offset(before, immediate=True)
    helper.set_rivianpilot_offset(base, feature.last_output, **args)
    assert helper.actual_camera_offset == base


@pytest.mark.parametrize("rpy", [[], [0.0], [0.0, float("nan"), 0.0], [0.0, float("inf"), 0.0]])
def test_invalid_calibration_uses_image_center(rpy):
  intrinsics = DEVICE_CAMERAS[('mici', 'os04c10')].fcam.intrinsics
  assert CameraOffsetHelper.get_v_horizon(intrinsics, rpy) == intrinsics[1, 2]


@pytest.mark.parametrize("main_wide", [False, True])
@pytest.mark.parametrize("pitch", [-8.0, 0.0, 8.0])
def test_zero_offset_preserves_both_transforms_at_any_pitch(main_wide, pitch):
  helper = CameraOffsetHelper()
  sm = MockStruct(deviceState=MockStruct(deviceType='mici'),
                  roadCameraState=MockStruct(sensor='os04c10'),
                  liveCalibration=MockStruct(rpyCalib=[0.0, np.radians(pitch), 0.0], height=[1.22]))
  transform = np.array([[1.0, 0.2, 3.0], [0.1, 1.0, 2.0], [0.0, 0.0, 1.0]], dtype=np.float32)
  main, extra = helper.update(transform, transform, sm, main_wide)
  np.testing.assert_array_equal(main, transform)
  np.testing.assert_array_equal(extra, transform)
