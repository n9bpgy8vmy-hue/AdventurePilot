"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
import math

import numpy as np

from openpilot.common.transformations.camera import DEVICE_CAMERAS


class CameraOffsetHelper:
  MAX_RIVIANPILOT_DYNAMIC_OFFSET_M = 15.0 * 0.0254
  RIVIANPILOT_HEARTBEAT_TIMEOUT_S = 1.0

  def __init__(self):
    self.camera_offset = 0.0
    self.actual_camera_offset = 0.0

  @staticmethod
  def get_v_horizon(intrinsics, rpy_calib):
    cy = intrinsics[1, 2]
    if len(rpy_calib) == 3 and np.isfinite(rpy_calib).all():
      fy = intrinsics[1, 1]
      pitch = rpy_calib[1]
      return float(cy - fy * np.tan(pitch))
    return float(cy)

  @staticmethod
  def apply_camera_offset(model_transform, height, offset_param, v_horizon):
    shear = np.eye(3, dtype=np.float32)
    shear[0, 1] = offset_param / height
    shear[0, 2] = -offset_param / height * v_horizon
    model_transform = (shear @ model_transform).astype(np.float32)
    return model_transform

  @classmethod
  def valid_rivianpilot_dynamic_offset(cls, offset, updated, now):
    try:
      return (math.isfinite(float(offset)) and
              abs(float(offset)) <= cls.MAX_RIVIANPILOT_DYNAMIC_OFFSET_M and
              math.isfinite(float(updated)) and
              0.0 <= float(now) - float(updated) < cls.RIVIANPILOT_HEARTBEAT_TIMEOUT_S)
    except (TypeError, ValueError, OverflowError):
      return False

  def set_rivianpilot_offset(self, base_offset, dynamic_offset, updated, now, *, steering_pressed, in_drive, lat_active):
    """Consume the controller's handoff ramp; cancel immediately on hard overrides.

    Blinkers/native lane changes are handled by LanePositionController. They
    must not bypass that ramp here. Its existing one-second heartbeat watchdog
    still removes an unavailable or invalid producer.
    """
    force_zero = (steering_pressed or not in_drive or not lat_active or
                  not self.valid_rivianpilot_dynamic_offset(dynamic_offset, updated, now))
    self.set_offset(base_offset + (0.0 if force_zero else dynamic_offset), immediate=force_zero)

  def set_offset(self, offset, immediate=False):
    self.camera_offset = offset
    if immediate:
      self.actual_camera_offset = offset

  def update(self, model_transform_main, model_transform_extra, sm, main_wide_camera):
    self.actual_camera_offset = (0.9 * self.actual_camera_offset) + (0.1 * self.camera_offset)
    dc = DEVICE_CAMERAS[(str(sm['deviceState'].deviceType), str(sm['roadCameraState'].sensor))]
    height = sm["liveCalibration"].height[0] if sm['liveCalibration'].height else 1.22
    rpy_calib = sm['liveCalibration'].rpyCalib

    intrinsics_main = dc.ecam.intrinsics if main_wide_camera else dc.fcam.intrinsics
    v_horizon_main = self.get_v_horizon(intrinsics_main, rpy_calib)
    model_transform_main = self.apply_camera_offset(model_transform_main, height, self.actual_camera_offset, v_horizon_main)

    intrinsics_extra = dc.ecam.intrinsics
    v_horizon_extra = self.get_v_horizon(intrinsics_extra, rpy_calib)
    model_transform_extra = self.apply_camera_offset(model_transform_extra, height, self.actual_camera_offset, v_horizon_extra)
    return model_transform_main, model_transform_extra
