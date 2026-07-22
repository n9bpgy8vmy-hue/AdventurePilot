from cereal import custom
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
    "RivianMadsResumeDelay": 3,
  }[key])
  selfdrive.events = Events()
  selfdrive.events_sp = EventsSP()
  selfdrive.enabled = False
  selfdrive.enabled_prev = False
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
  mads.rivian_mads_resume_countdown = DT_CTRL
  assert mads.should_silent_lkas_enable(car_state(20.1))


def test_sunnylink_selection_changes_resume_threshold(mocker):
  mads = make_mads(mocker)
  mads.rivian_reverse_resume_pending = True
  mads.rivian_mads_auto_resume_speed = 10
  mads.rivian_mads_resume_countdown = DT_CTRL
  assert mads.should_silent_lkas_enable(car_state(11))


def test_unrelated_pause_is_not_speed_gated(mocker):
  mads = make_mads(mocker)
  assert mads.should_silent_lkas_enable(car_state(5))


def test_metric_selection_uses_kph(mocker):
  mads = make_mads(mocker)
  mads.rivian_reverse_resume_pending = True
  mads.is_metric = True
  assert not mads.should_silent_lkas_enable(car_state(12.4))  # about 20 km/h
  mads.rivian_mads_resume_countdown = DT_CTRL
  assert mads.should_silent_lkas_enable(car_state(12.5))


def test_warning_alert_starts_configured_countdown(mocker):
  mads = make_mads(mocker)
  mads.rivian_reverse_resume_pending = True
  assert not mads.should_silent_lkas_enable(car_state(21))
  assert mads.events_sp.has(custom.OnroadEventSP.EventName.rivianMadsResumeWarning3Sec)
  assert mads.rivian_mads_resume_countdown == 3 - DT_CTRL


def test_countdown_resets_if_speed_drops(mocker):
  mads = make_mads(mocker)
  mads.rivian_reverse_resume_pending = True
  assert not mads.should_silent_lkas_enable(car_state(21))
  assert mads.rivian_mads_resume_countdown > 0
  assert not mads.should_silent_lkas_enable(car_state(20))
  assert mads.rivian_mads_resume_countdown == 0


def test_countdown_resets_if_shifted_back_to_reverse(mocker):
  mads = make_mads(mocker)
  mads.rivian_reverse_resume_pending = True
  assert not mads.should_silent_lkas_enable(car_state(21))
  assert mads.rivian_mads_resume_countdown > 0
  assert not mads.should_silent_lkas_enable(car_state(0, structs.CarState.GearShifter.reverse))
  assert mads.rivian_mads_resume_countdown == 0


def test_each_configurable_delay_resumes_once(mocker):
  for delay in range(1, 6):
    mads = make_mads(mocker)
    mads.rivian_reverse_resume_pending = True
    mads.rivian_mads_resume_delay = delay

    assert not mads.should_silent_lkas_enable(car_state(21))
    warning_event = getattr(custom.OnroadEventSP.EventName, f"rivianMadsResumeWarning{delay}Sec")
    assert mads.events_sp.has(warning_event)

    # Floating-point subtraction may require one additional 10 ms control tick.
    for _ in range(round(delay / DT_CTRL) + 1):
      if mads.should_silent_lkas_enable(car_state(21)):
        break
    else:
      raise AssertionError(f"MADS did not resume after the {delay}-second warning")


def test_logs_reverse_resume_lifecycle(mocker):
  log_event = mocker.patch("openpilot.sunnypilot.mads.mads.cloudlog.event")
  mads = make_mads(mocker)
  mads.rivianpilot_feature_logging = True
  reverse = car_state(0, structs.CarState.GearShifter.reverse)
  drive = car_state(21)

  mads.arm_rivian_reverse_resume(reverse, "reverseGear")
  log_event.assert_called_once_with(
    "rivian mads reverse resume",
    action="armed",
    gear=str(reverse.gearShifter),
    speed_ms=0.0,
    threshold=20,
    is_metric=False,
    configured_delay=3,
    source="reverseGear",
  )

  log_event.reset_mock()
  assert not mads.should_silent_lkas_enable(drive)
  assert log_event.call_args.kwargs["action"] == "countdown_started"
  assert log_event.call_args.kwargs["delay_seconds"] == 3

  log_event.reset_mock()
  assert not mads.should_silent_lkas_enable(reverse)
  assert log_event.call_args.kwargs["action"] == "countdown_reset"
  assert log_event.call_args.kwargs["reason"] == "not_in_drive"


def test_logs_resume_after_countdown(mocker):
  log_event = mocker.patch("openpilot.sunnypilot.mads.mads.cloudlog.event")
  mads = make_mads(mocker)
  mads.rivianpilot_feature_logging = True
  mads.rivian_reverse_resume_pending = True
  mads.rivian_mads_resume_countdown = DT_CTRL

  assert mads.should_silent_lkas_enable(car_state(21))
  assert log_event.call_args.kwargs["action"] == "resumed"


def test_feature_logging_defaults_to_off(mocker):
  log_event = mocker.patch("openpilot.sunnypilot.mads.mads.cloudlog.event")
  mads = make_mads(mocker)
  mads.rivian_reverse_resume_pending = True

  assert not mads.should_silent_lkas_enable(car_state(21))
  log_event.assert_not_called()
