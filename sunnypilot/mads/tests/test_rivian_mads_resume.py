from cereal import custom, log
from opendbc.car import structs

from openpilot.common.constants import CV
from openpilot.common.realtime import DT_CTRL
from openpilot.selfdrive.selfdrived.events import Events
from openpilot.sunnypilot.mads.helpers import MadsSteeringModeOnBrake
from openpilot.sunnypilot.mads.mads import ModularAssistiveDrivingSystem
from openpilot.sunnypilot.selfdrive.selfdrived.events import EventsSP


def make_mads(mocker):
  selfdrive = mocker.MagicMock()
  selfdrive.CP = structs.CarParams()
  selfdrive.CP.brand = "rivian"
  selfdrive.CP_SP = structs.CarParamsSP()
  selfdrive.params.get_bool = mocker.MagicMock(side_effect=lambda key: {
    "Mads": True,
    "MadsMainCruiseAllowed": False,
    "MadsUnifiedEngagementMode": True,
    "IsMetric": False,
  }.get(key, False))
  selfdrive.params.get = mocker.MagicMock(side_effect=lambda key, **kwargs: {
    "MadsSteeringMode": MadsSteeringModeOnBrake.REMAIN_ACTIVE,
    "RivianMadsAutoResumeSpeed": 20,
    "RivianMadsReversePauseSpeed": 10,
    "RivianMadsResumeDelay": 3,
  }[key])
  selfdrive.events = Events()
  selfdrive.events_sp = EventsSP()
  selfdrive.enabled = False
  selfdrive.enabled_prev = False
  selfdrive.CS_prev = car_state(15)
  selfdrive.state_machine = mocker.MagicMock()
  return ModularAssistiveDrivingSystem(selfdrive)


def car_state(speed_mph, gear=structs.CarState.GearShifter.drive):
  state = structs.CarState()
  state.vEgo = speed_mph * CV.MPH_TO_MS
  state.gearShifter = gear
  return state


def test_reverse_always_blocks_silent_resume(mocker):
  mads = make_mads(mocker)
  mads.rivian_reverse_resume_pending = True
  assert not mads.should_silent_lkas_enable(car_state(25, structs.CarState.GearShifter.reverse))


def test_drive_at_or_below_selected_speed_blocks_silent_resume(mocker):
  mads = make_mads(mocker)
  mads.rivian_reverse_resume_pending = True
  assert not mads.should_silent_lkas_enable(car_state(19.9))
  assert not mads.should_silent_lkas_enable(car_state(20))


def test_drive_above_selected_speed_allows_silent_resume(mocker):
  mads = make_mads(mocker)
  mads.rivian_reverse_resume_pending = True
  mads.rivian_mads_resume_delay = DT_CTRL
  assert mads.should_silent_lkas_enable(car_state(20.1))


def test_sunnylink_selection_changes_resume_threshold(mocker):
  mads = make_mads(mocker)
  mads.rivian_reverse_resume_pending = True
  mads.rivian_mads_auto_resume_speed = 10
  mads.rivian_mads_resume_delay = DT_CTRL
  assert mads.should_silent_lkas_enable(car_state(11))


def test_unrelated_pause_is_not_speed_gated(mocker):
  mads = make_mads(mocker)
  assert mads.should_silent_lkas_enable(car_state(5))


def test_metric_selection_uses_kph(mocker):
  mads = make_mads(mocker)
  mads.rivian_reverse_resume_pending = True
  mads.is_metric = True
  assert not mads.should_silent_lkas_enable(car_state(12.4))  # about 20 km/h
  mads.rivian_mads_resume_delay = DT_CTRL
  assert mads.should_silent_lkas_enable(car_state(12.5))


def test_warning_chime_starts_configured_countdown(mocker):
  mads = make_mads(mocker)
  mads.rivian_reverse_resume_pending = True
  assert not mads.should_silent_lkas_enable(car_state(21))
  assert mads.events_sp.contains(custom.OnroadEventSP.EventName.e2eChime)
  assert mads.rivian_mads_resume_countdown == 3 - DT_CTRL


def test_countdown_resets_if_speed_drops(mocker):
  mads = make_mads(mocker)
  mads.rivian_reverse_resume_pending = True
  assert not mads.should_silent_lkas_enable(car_state(21))
  assert mads.rivian_mads_resume_countdown > 0
  assert not mads.should_silent_lkas_enable(car_state(20))
  assert mads.rivian_mads_resume_countdown == 0


def test_reverse_request_above_pause_speed_is_ignored(mocker):
  mads = make_mads(mocker)
  assert mads.should_ignore_rivian_reverse_request(car_state(11, structs.CarState.GearShifter.reverse))


def test_reverse_at_or_below_pause_speed_is_not_ignored(mocker):
  mads = make_mads(mocker)
  assert not mads.should_ignore_rivian_reverse_request(car_state(10, structs.CarState.GearShifter.reverse))


def test_ignored_reverse_request_remains_ignored_while_above_threshold(mocker):
  mads = make_mads(mocker)
  assert mads.should_ignore_rivian_reverse_request(car_state(11, structs.CarState.GearShifter.reverse))
  mads.selfdrive.CS_prev.gearShifter = structs.CarState.GearShifter.reverse
  assert mads.should_ignore_rivian_reverse_request(car_state(11, structs.CarState.GearShifter.reverse))


def test_ignored_reverse_request_stops_at_pause_speed(mocker):
  mads = make_mads(mocker)
  assert mads.should_ignore_rivian_reverse_request(car_state(11, structs.CarState.GearShifter.reverse))
  mads.selfdrive.CS_prev.gearShifter = structs.CarState.GearShifter.reverse
  assert not mads.should_ignore_rivian_reverse_request(car_state(10, structs.CarState.GearShifter.reverse))


def test_high_speed_reverse_request_does_not_pause_mads(mocker):
  mads = make_mads(mocker)
  mads.enabled = True
  mads.events.add(log.OnroadEvent.EventName.wrongGear)
  mads.events.add(log.OnroadEvent.EventName.reverseGear)
  mads.update_events(car_state(11, structs.CarState.GearShifter.reverse))
  assert not mads.events.has(log.OnroadEvent.EventName.wrongGear)
  assert not mads.events.has(log.OnroadEvent.EventName.reverseGear)
  assert not mads.events_sp.has(custom.OnroadEventSP.EventName.silentLkasDisable)
