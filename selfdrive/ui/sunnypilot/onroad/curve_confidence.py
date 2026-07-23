import time

import pyray as rl
from cereal import log

from openpilot.common.swaglog import cloudlog
from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.sunnypilot.rivianpilot.curve_confidence import (
  CurveConfidenceInputUnavailable,
  calculate_curve_confidence,
)
from openpilot.system.ui.lib.application import FONT_SCALE, FontWeight, gui_app
from openpilot.system.ui.lib.text_measure import measure_text_cached


class CurveConfidenceRenderer:
  """Fail-isolated, visual-only curve confidence indicator."""

  YELLOW_ENTER = 70.0
  RED_ENTER = 40.0
  YELLOW_EXIT = 75.0
  RED_EXIT = 50.0
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

    score, in_curve, reason, path_sample = calculate_curve_confidence(
      model, controls, car_state,
      calibration.calStatus == log.LiveCalibrationData.Status.calibrated,
      self._previous_path_y,
    )
    self._previous_path_y = path_sample
    self._last_model_frame = model_frame
    self._latest_result = (score, in_curve, reason)
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
