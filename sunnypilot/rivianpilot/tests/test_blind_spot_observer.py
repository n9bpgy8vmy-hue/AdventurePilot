from cereal import log
from opendbc.car import structs

from openpilot.sunnypilot.rivianpilot.blind_spot_observer import BlindSpotObserver


def make_params(mocker, enabled=True, logging=True):
  params = mocker.MagicMock()
  params.get_bool.side_effect = lambda key: {
    "RivianPilotBlindSpotObserver": enabled,
    "RivianPilotFeatureLogging": logging,
  }.get(key, False)
  return params


def car_state(left_blinker=True, left_blindspot=False):
  CS = structs.CarState()
  CS.vEgo = 25.0
  CS.leftBlinker = left_blinker
  CS.leftBlindspot = left_blindspot
  CS.steeringTorque = 0.0
  CS.steeringPressed = False
  return CS


def test_logs_only_meaningful_transitions(mocker):
  event = mocker.patch("openpilot.sunnypilot.rivianpilot.blind_spot_observer.cloudlog.event")
  observer = BlindSpotObserver(make_params(mocker))
  CS = car_state()

  observer.update(CS, log.LaneChangeState.preLaneChange, log.LaneChangeDirection.left, now=1.0)
  observer.update(CS, log.LaneChangeState.preLaneChange, log.LaneChangeDirection.left, now=1.1)
  assert event.call_count == 1
  assert event.call_args.kwargs["action"] == "session_start"

  CS.leftBlindspot = True
  observer.update(CS, log.LaneChangeState.preLaneChange, log.LaneChangeDirection.left, now=1.3)
  assert event.call_count == 2
  assert event.call_args.kwargs["native_blocked"] is True

  CS.leftBlinker = False
  observer.update(CS, log.LaneChangeState.off, log.LaneChangeDirection.none, now=1.4)
  assert event.call_count == 3
  assert event.call_args.kwargs["action"] == "session_end"


def test_disabled_observer_never_logs(mocker):
  event = mocker.patch("openpilot.sunnypilot.rivianpilot.blind_spot_observer.cloudlog.event")
  observer = BlindSpotObserver(make_params(mocker, enabled=False))
  observer.update(car_state(), log.LaneChangeState.preLaneChange, log.LaneChangeDirection.left)
  event.assert_not_called()


def test_logging_failure_is_contained_and_reported_once(mocker):
  observer = BlindSpotObserver(make_params(mocker))
  event = mocker.patch("openpilot.sunnypilot.rivianpilot.blind_spot_observer.cloudlog.event")
  event.side_effect = [RuntimeError("logger unavailable"), None]

  observer.update(car_state(), log.LaneChangeState.preLaneChange, log.LaneChangeDirection.left)
  assert observer.faulted
  assert not observer.enabled
  assert event.call_count == 2

  observer.update(car_state(left_blindspot=True), log.LaneChangeState.preLaneChange, log.LaneChangeDirection.left)
  assert event.call_count == 2


def test_unexpected_state_failure_cannot_escape(mocker):
  observer = BlindSpotObserver(make_params(mocker))
  event = mocker.patch("openpilot.sunnypilot.rivianpilot.blind_spot_observer.cloudlog.event")
  bad_state = mocker.MagicMock()
  bad_state.leftBlinker.__bool__.side_effect = RuntimeError("bad car state")

  observer.update(bad_state, log.LaneChangeState.off, log.LaneChangeDirection.none)
  assert observer.faulted
  assert event.call_count == 1
