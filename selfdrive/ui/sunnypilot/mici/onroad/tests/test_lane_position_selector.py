from types import SimpleNamespace

import pyray as rl
import pytest

from openpilot.selfdrive.ui.sunnypilot.mici.onroad.lane_position_selector import LanePositionSelector, OPTIONS, PARAM
from openpilot.system.ui.lib.application import MouseEvent, MousePos, gui_app


@pytest.fixture
def picker(mocker):
  params = mocker.MagicMock()
  params.get.return_value = 2
  params.get_bool.return_value = True
  params.put.return_value = None
  widget = LanePositionSelector(params)
  widget.set_rect(rl.Rectangle(0, 0, 476, 240))
  widget.set_available(True)
  mocker.patch("openpilot.selfdrive.ui.sunnypilot.mici.onroad.lane_position_selector.time.monotonic", return_value=10.0)
  widget._update_state()
  return widget


def event(x, y, kind, slot=0):
  return MouseEvent(MousePos(x, y), slot, kind == 'press', kind == 'release', kind == 'down', 10.0)


def tap(widget, index=0):
  rect = widget._button_rect(index)
  x, y = rect.x + rect.width / 2, rect.y + rect.height / 2
  widget._handle_mouse_event(event(x, y, 'press'))
  widget._handle_mouse_event(event(x, y, 'release'))


@pytest.mark.parametrize("index,value", list(enumerate([2, 0, 1, 3])))
def test_two_tap_selection_writes_only_lane_preference(picker, index, value):
  tap(picker)
  assert picker.expanded
  picker.params.put.assert_not_called()
  tap(picker, index)
  picker.params.put.assert_called_once_with(PARAM, value, block=False)
  picker.params.get.return_value = value
  picker._update_state()
  assert picker.selected == value
  assert not picker.expanded
  assert picker._pending_value is None


def test_drag_off_button_never_selects_even_after_return(picker):
  tap(picker)
  rect = picker._button_rect(3)
  x, y = rect.x + 20, rect.y + 20
  picker._handle_mouse_event(event(x, y, 'press'))
  picker._handle_mouse_event(event(x - 50, y, 'down'))
  picker._handle_mouse_event(event(x, y, 'release'))
  picker.params.put.assert_not_called()
  assert picker.consumed and not picker.captured


def test_alert_or_disengaging_feature_cancels_active_tap(picker):
  tap(picker)
  rect = picker._button_rect(3)
  picker._handle_mouse_event(event(rect.x + 20, rect.y + 20, 'press'))
  picker.set_available(False)
  picker._handle_mouse_event(event(rect.x + 20, rect.y + 20, 'release'))
  picker.params.put.assert_not_called()
  assert picker.consumed and not picker.captured


def test_navigation_owns_touches_that_begin_elsewhere(picker):
  picker._handle_mouse_event(event(50, 150, 'press'))
  rect = picker._button_rect()
  picker._handle_mouse_event(event(rect.x + 20, rect.y + 20, 'release'))
  assert not picker.expanded
  assert not picker.consumed


def test_scroller_is_blocked_before_selector_handles_new_press(picker, mocker):
  rect = picker._button_rect()
  mocker.patch.object(gui_app, '_mouse_events', [event(rect.x + 20, rect.y + 20, 'press')])
  assert picker.blocks_navigation()
  mocker.patch.object(gui_app, '_mouse_events', [event(50, 150, 'press')])
  assert not picker.blocks_navigation()


def test_parent_home_click_is_suppressed_for_picker_tap(picker, mocker):
  from openpilot.selfdrive.ui.mici.onroad.augmented_road_view import AugmentedRoadView
  from openpilot.selfdrive.ui.mici.onroad.cameraview import CameraView
  road = AugmentedRoadView.__new__(AugmentedRoadView)
  road.close = lambda: None  # no GPU resources are created by this touch-only fixture
  road._lane_selector = picker
  road._bookmark_icon = SimpleNamespace(interacting=lambda: False)
  release = mocker.patch.object(CameraView, '_handle_mouse_release')
  tap(picker)
  road._handle_mouse_release(MousePos(400, 36))
  release.assert_not_called()
  picker._update_state()
  road._handle_mouse_release(MousePos(50, 150))
  release.assert_called_once()


def test_remote_preference_refresh_and_timeout(picker, mocker):
  tap(picker)
  picker.params.get.return_value = 3
  mocker.patch('openpilot.selfdrive.ui.sunnypilot.mici.onroad.lane_position_selector.time.monotonic', return_value=100000000)
  picker._update_state()
  assert picker.selected == 3
  assert not picker.expanded


def test_save_failure_keeps_old_selection_and_reports_failure(picker, mocker):
  mocker.patch('openpilot.selfdrive.ui.sunnypilot.mici.onroad.lane_position_selector.cloudlog.exception')
  picker.params.put.side_effect = OSError('read only')
  tap(picker)
  tap(picker, 3)
  assert picker.selected == 2
  assert picker._error_until == 13


def test_all_four_buttons_fit_camera_area_without_overlap(picker):
  tap(picker)
  rects = [picker._button_rect(i) for i in range(len(OPTIONS))]
  assert rects[0].x >= 96  # driver-monitoring icon remains unobstructed
  assert rects[-1].x + rects[-1].width <= 476
  assert all(a.x + a.width < b.x for a, b in zip(rects, rects[1:], strict=False))
  assert all(r.height >= 44 for r in rects)


def test_critical_alerts_signals_and_other_vehicles_hide_picker():
  from cereal import log
  alert = SimpleNamespace(status=log.SelfdriveState.AlertStatus.normal,
                          size=log.SelfdriveState.AlertSize.small, alert_type="rivianPilotActiveOffset/permanent")
  assert LanePositionSelector.should_show("rivian", True, alert, False, False)
  alert.status = log.SelfdriveState.AlertStatus.critical
  assert not LanePositionSelector.should_show("rivian", True, alert, False, False)
  assert not LanePositionSelector.should_show("rivian", False, None, False, False)
  assert not LanePositionSelector.should_show("rivian", True, None, True, False)
  assert not LanePositionSelector.should_show("toyota", True, None, False, False)
  assert not LanePositionSelector.should_show("rivian", True, None, False, False, hud_busy=True)


def test_async_write_is_not_shown_as_saved_until_read_back(picker, mocker):
  tap(picker)
  tap(picker, 3)
  assert picker.selected == 2
  assert picker._pending_value == 3
  picker._update_state()
  assert picker._pending_value == 3
  mocker.patch('openpilot.selfdrive.ui.sunnypilot.mici.onroad.lane_position_selector.time.monotonic', return_value=14.0)
  picker._update_state()
  assert picker._pending_value is None
  assert picker.selected == 2
  assert picker._error_until == 17.0
