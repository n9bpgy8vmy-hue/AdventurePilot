import math
import time

import pyray as rl
from cereal import log

from openpilot.common.swaglog import cloudlog
from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.system.ui.lib.application import FONT_SCALE, FontWeight, gui_app
from openpilot.system.ui.lib.text_measure import measure_text_cached


class CurveConfidenceInputUnavailable(Exception):
  pass


class CurveConfidenceRenderer:
  """Fail-isolated, visual-only curve confidence indicator."""

  YELLOW_ENTER = 70.0
  RED_ENTER = 40.0
  YELLOW_EXIT = 75.0
  RED_EXIT = 50.0
  CURVE_LAT_ACCEL_MIN = 0.35
  YELLOW_HOLD_SECONDS = 0.5
  RED_HOLD_SECONDS = 0.3
  RECOVERY_SECONDS = 2.0

  def __init__(self):
    self._font = gui_app.font(FontWeight.BOLD)
    self.level = "normal"
    self.score = 100.0
    self.reason = ""
    self._candidate_level = "normal"
    self._candidate_since = 0.0
    self._recovery_since = 0.0
    self._previous_path_y: float | None = None
    self._last_model_frame = -1
    self._latest_result = (100.0, False, "")
    self._faulted = False
    self._error_logged = False
    self._was_started = False

  @staticmethod
  def _finite(value) -> float:
    value = float(value)
    if not math.isfinite(value):
      raise CurveConfidenceInputUnavailable("non-finite curve confidence input")
    return value

  @staticmethod
  def _clip(value: float, low: float = 0.0, high: float = 1.0) -> float:
    return max(low, min(high, value))

  def _record_error_once(self, error: str, exception: Exception) -> None:
    if self._error_logged:
      return
    self._error_logged = True
    try:
      cloudlog.event("rivianpilot feature error", feature="curve_confidence_indicator",
                     errors=[error], error_type=type(exception).__name__)
    except Exception:
      pass

  def suppress_after_error(self, exception: Exception) -> None:
    self._faulted = True
    self.level = "normal"
    self._record_error_once("scoring_failure_suppressed", exception)

  def _score(self) -> tuple[float, bool, str]:
    sm = ui_state.sm
    required = ("modelV2", "controlsState", "carState", "liveCalibration")
    if any(not sm.valid[name] or sm.recv_frame[name] < ui_state.started_frame for name in required):
      raise CurveConfidenceInputUnavailable("stale curve confidence input")
    model_frame = sm.recv_frame["modelV2"]
    if model_frame == self._last_model_frame:
      return self._latest_result

    model = sm["modelV2"]
    controls = sm["controlsState"]
    car_state = sm["carState"]
    calibration = sm["liveCalibration"]

    desired_curvature = self._finite(controls.desiredCurvature)
    actual_curvature = self._finite(controls.curvature)
    speed = max(self._finite(car_state.vEgo), 0.0)
    desired_lat_accel = abs(desired_curvature) * speed * speed
    actual_lat_accel = abs(actual_curvature) * speed * speed
    in_curve = max(desired_lat_accel, actual_lat_accel) >= self.CURVE_LAT_ACCEL_MIN

    lane_probs = [self._finite(v) for v in model.laneLineProbs]
    if len(lane_probs) < 3:
      raise CurveConfidenceInputUnavailable("lane confidence unavailable")
    lane_quality = self._clip((lane_probs[1] + lane_probs[2]) / 2.0)

    edge_stds = [abs(self._finite(v)) for v in model.roadEdgeStds]
    if len(edge_stds) < 2:
      raise CurveConfidenceInputUnavailable("road edge confidence unavailable")
    edge_quality = 1.0 - self._clip((edge_stds[0] + edge_stds[1]) / 2.0)

    path_y = [self._finite(v) for v in model.position.y]
    if len(path_y) < 6:
      raise CurveConfidenceInputUnavailable("model path unavailable")
    path_sample = path_y[min(10, len(path_y) - 1)]
    path_quality = 1.0 if self._previous_path_y is None else 1.0 - self._clip(abs(path_sample - self._previous_path_y))
    self._previous_path_y = path_sample

    curvature_scale = max(abs(desired_curvature), 0.002)
    tracking_quality = 1.0 - self._clip(abs(desired_curvature - actual_curvature) / curvature_scale)

    saturated = False
    try:
      lateral_state = controls.lateralControlState
      lateral_log = getattr(lateral_state, lateral_state.which())
      saturated = bool(getattr(lateral_log, "saturated", False))
    except Exception:
      saturated = False

    calibration_quality = 1.0 if calibration.calStatus == log.LiveCalibrationData.Status.calibrated else 0.0
    driver_penalty = 10.0 if car_state.steeringPressed and abs(self._finite(car_state.steeringTorque)) > 1.0 else 0.0
    score = (
      lane_quality * 25.0 +
      edge_quality * 15.0 +
      path_quality * 20.0 +
      tracking_quality * 20.0 +
      (0.0 if saturated else 10.0) +
      calibration_quality * 10.0 -
      driver_penalty
    )
    immediate_red = saturated and tracking_quality < 0.35
    reason = "controller tracking" if immediate_red else \
             "lane and path confidence" if lane_quality < 0.5 and edge_quality < 0.5 else \
             "curve prediction"
    self._last_model_frame = model_frame
    self._latest_result = (max(0.0, min(100.0, score)), in_curve, reason)
    return self._latest_result

  def update(self) -> None:
    if not ui_state.started:
      self.level = "normal"
      self._candidate_level = "normal"
      self._previous_path_y = None
      self._last_model_frame = -1
      self._latest_result = (100.0, False, "")
      self._faulted = False
      self._error_logged = False
      self._was_started = False
      return
    if not ui_state.curve_confidence_indicator or self._faulted:
      self.level = "normal"
      return

    self._was_started = True
    try:
      score, in_curve, reason = self._score()
      self.score = score
      self.reason = reason
    except CurveConfidenceInputUnavailable:
      self.level = "normal"
      self._candidate_level = "normal"
      self._previous_path_y = None
      return
    except Exception as e:
      self.suppress_after_error(e)
      return

    now = time.monotonic()
    if not in_curve:
      if self._recovery_since == 0.0:
        self._recovery_since = now
      if now - self._recovery_since >= self.RECOVERY_SECONDS:
        self.level = "normal"
        self._candidate_level = "normal"
      return
    self._recovery_since = 0.0

    immediate_red = self.score < self.RED_ENTER and self.reason == "controller tracking"
    target = "red" if self.score < self.RED_ENTER else "yellow" if self.score < self.YELLOW_ENTER else "normal"
    if self.level == "red" and self.score < self.RED_EXIT:
      target = "red"
    elif self.level == "yellow" and self.score < self.YELLOW_EXIT:
      target = "yellow"

    if immediate_red:
      self.level = "red"
      self._candidate_level = "red"
      self._candidate_since = now
    elif target != self._candidate_level:
      self._candidate_level = target
      self._candidate_since = now
    else:
      hold = self.RED_HOLD_SECONDS if target == "red" else self.YELLOW_HOLD_SECONDS if target == "yellow" else self.RECOVERY_SECONDS
      if now - self._candidate_since >= hold:
        self.level = target

  def render_background(self, rect: rl.Rectangle) -> None:
    if self.level == "yellow":
      rl.draw_rectangle_rec(rect, rl.Color(255, 190, 0, 42))
    elif self.level == "red":
      rl.draw_rectangle_rec(rect, rl.Color(220, 25, 35, 52))

  def render_banner(self, rect: rl.Rectangle) -> None:
    if self.level == "normal":
      return
    flash_high = int(time.monotonic() * 2) % 2 == 0
    color = rl.Color(235, 160, 0, 225 if flash_high else 165) if self.level == "yellow" else \
            rl.Color(190, 20, 30, 235 if flash_high else 175)
    text = "CURVE CONFIDENCE LOW" if self.level == "yellow" else "CURVE ASSISTANCE UNCERTAIN - BE READY"
    height = 105
    banner = rl.Rectangle(rect.x, rect.y + rect.height - height, rect.width, height)
    rl.draw_rectangle_rec(banner, color)
    font_size = 42
    size = measure_text_cached(self._font, text, font_size)
    position = rl.Vector2(banner.x + (banner.width - size.x) / 2,
                          banner.y + (banner.height - size.y * FONT_SCALE) / 2)
    rl.draw_text_ex(self._font, text, position, font_size, 0, rl.WHITE)
