"""Compact Rivian lane-position picker, with exclusive tap/drag ownership."""
import math
import time

import pyray as rl
from cereal import log

from openpilot.common.swaglog import cloudlog
from openpilot.system.ui.lib.application import gui_app, MouseEvent
from openpilot.system.ui.widgets import Widget
from openpilot.system.ui.widgets.label import gui_label

OPTIONS = ((2, "Left"), (0, "Default"), (1, "Center"), (3, "Right"))
PARAM = "RivianPilotLanePositionPreference"


class LanePositionSelector(Widget):
  def __init__(self, params):
    super().__init__()
    self.params = params
    self.selected = 0
    self.go_live = False
    self.expanded = False
    self.available = False
    self.captured = False
    self.consumed = False
    self._cancelled = False
    self._press_option = None
    self._press_pos = None
    self._last_read = -1.0
    self._open_until = 0.0
    self._error_until = 0.0
    self._pending_value = None
    self._save_deadline = 0.0

  @staticmethod
  def should_show(brand, go_live, alert, left_blinker, right_blinker, hud_busy=False):
    routine_alert = (alert is None or (
      alert.status == log.SelfdriveState.AlertStatus.normal and
      alert.size == log.SelfdriveState.AlertSize.small and
      alert.alert_type.split("/")[0] in ("rivianPilotActiveOffset", "rivianPilotNudgeTimer")))
    return brand == "rivian" and go_live and routine_alert and not (left_blinker or right_blinker or hud_busy)

  def set_available(self, available):
    self.available = available
    if not available:
      self.expanded = False
      # Own the rest of an interrupted gesture, but never select on release.
      self._cancelled = True

  def _button_rect(self, index=0):
    width = 84 if self.expanded else 144
    total = 4 * width + 3 * 4 if self.expanded else width
    return rl.Rectangle(self.rect.x + self.rect.width - total - 28 + index * (width + 4), self.rect.y + 28, width, 44)

  def _option_at(self, pos):
    for i in range(4 if self.expanded else 1):
      if rl.check_collision_point_rec(pos, self._button_rect(i)):
        return i
    return None

  def blocks_navigation(self):
    # The scroller runs before children. Claim a new press from the previous
    # frame's rendered hit boxes before it can become a navigation swipe.
    return self.captured or self.consumed or (self.available and any(
      e.slot == 0 and e.left_pressed and self._option_at(e.pos) is not None for e in gui_app.mouse_events
    ))

  def _update_state(self):
    self.consumed = False
    now = time.monotonic()
    if now - self._last_read >= 1.0:
      self.go_live = self.params.get_bool("RivianPilotLanePositionGoLive")
      try:
        value = int(self.params.get(PARAM, return_default=True))
        self.selected = value if value in (0, 1, 2, 3) else 0
      except (TypeError, ValueError):
        self.selected = 0
      self._last_read = now
      if self._pending_value is not None:
        if self.selected == self._pending_value:
          self._pending_value = None
        elif now >= self._save_deadline:
          self._pending_value = None
          self._error_until = now + 3.0
    if self.expanded and not self.captured and now >= self._open_until:
      self.expanded = False

  def _process_mouse_events(self):
    for event in gui_app.mouse_events:
      if event.slot != 0:
        continue
      self._handle_mouse_event(event)

  def _handle_mouse_event(self, event: MouseEvent):
    if event.left_pressed and self.available and self._touch_valid():
      option = self._option_at(event.pos)
      if option is not None:
        self.captured = True
        self._cancelled = False
        self._press_option = option
        self._press_pos = event.pos
    if not self.captured:
      return
    self.consumed = True
    if not self.available or not self._touch_valid():
      self._cancelled = True
    if math.hypot(event.pos.x - self._press_pos.x, event.pos.y - self._press_pos.y) > 10:
      self._cancelled = True
    if event.left_released:
      if not self._cancelled and self._option_at(event.pos) == self._press_option:
        if not self.expanded:
          self.expanded = True
          self._open_until = time.monotonic() + 6.0
        else:
          value = OPTIONS[self._press_option][0]
          try:
            # User-triggered write only. Do not implicitly enable Go Live,
            # change the bias magnitude, or touch BSM settings.
            self.params.put(PARAM, value, block=False)
            self._pending_value = value
            self._save_deadline = time.monotonic() + 3.0
            self._last_read = time.monotonic() - 1.0
          except Exception:
            self._pending_value = None
            self._error_until = time.monotonic() + 3.0
            cloudlog.exception("Rivian lane preference could not be saved")
          self.expanded = False
      self.captured = False

  def _render(self, _):
    if not self.available:
      return
    labels = OPTIONS if self.expanded else ((self.selected, "Lane: " + dict(OPTIONS)[self.selected]),)
    for i, (value, label) in enumerate(labels):
      rect = self._button_rect(i)
      active = self.expanded and value == self.selected
      color = rl.Color(28, 108, 170, 245) if active else rl.Color(25, 29, 35, 235)
      rl.draw_rectangle_rounded(rect, 0.3, 8, color)
      if not self.expanded and self._pending_value is not None:
        label = "Saving..."
      if time.monotonic() < self._error_until:
        label = "Not saved"
      gui_label(rect, label, font_size=20, color=rl.WHITE,
                alignment=rl.GuiTextAlignment.TEXT_ALIGN_CENTER,
                alignment_vertical=rl.GuiTextAlignmentVertical.TEXT_ALIGN_MIDDLE)
