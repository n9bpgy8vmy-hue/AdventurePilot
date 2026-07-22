import json

from opendbc.car import structs

from openpilot.common.constants import CV
from openpilot.sunnypilot.rivianpilot.lane_hugging_observer import CORRECTION_DURATION_SECONDS, LaneHuggingObserver
from openpilot.common.realtime import DT_CTRL


def make_params(mocker, locations=None):
  params = mocker.MagicMock()
  params.get_bool.side_effect = lambda key: {
    "RivianPilotLaneHuggingObserver": True,
    "RivianPilotFeatureLogging": True,
    "IsMetric": False,
  }.get(key, False)
  params.get.side_effect = lambda key, **kwargs: {
    "RivianPilotLaneHuggingLocations": json.dumps(locations or []),
    "RivianPilotLaneHuggingMinSpeed": 25,
    "RivianPilotLaneHuggingAlertDistance": 150,
  }[key]
  return params


def car_state(speed_mph=30, steering_pressed=True, torque=1.0, blinker=False):
  CS = structs.CarState()
  CS.vEgo = speed_mph * CV.MPH_TO_MS
  CS.steeringPressed = steering_pressed
  CS.steeringTorque = torque
  CS.leftBlinker = blinker
  return CS


def gps(mocker, latitude=32.8140, longitude=-96.9489):
  location = mocker.MagicMock()
  location.hasFix = True
  location.latitude = latitude
  location.longitude = longitude
  return location


def test_records_sustained_driver_correction(mocker):
  params = make_params(mocker)
  observer = LaneHuggingObserver(params)
  CS = car_state(torque=1.0)

  for _ in range(round(CORRECTION_DURATION_SECONDS / DT_CTRL)):
    observer.update(CS, lateral_active=True, gps=gps(mocker), now=1000.0)

  assert len(observer.locations) == 1
  assert observer.locations[0]["direction"] == "left"
  assert observer.locations[0]["count"] == 1
  params.put.assert_called_once()


def test_never_records_without_active_lateral_or_with_blinker(mocker):
  params = make_params(mocker)
  observer = LaneHuggingObserver(params)

  for _ in range(round(CORRECTION_DURATION_SECONDS / DT_CTRL) + 2):
    observer.update(car_state(), lateral_active=False, gps=gps(mocker), now=1000.0)
    observer.update(car_state(blinker=True), lateral_active=True, gps=gps(mocker), now=1000.0)

  assert observer.locations == []
  params.put.assert_not_called()


def test_alerts_on_future_approach_without_actuation(mocker):
  locations = [{
    "id": "known-right",
    "latitude": 32.8140,
    "longitude": -96.9489,
    "direction": "right",
    "count": 3,
    "first_seen": 100.0,
    "last_seen": 100.0,
  }]
  observer = LaneHuggingObserver(make_params(mocker, locations))

  alert = observer.update(car_state(steering_pressed=False), lateral_active=True, gps=gps(mocker), now=1000.0)

  assert alert == "right"
  assert observer.update(car_state(steering_pressed=False), lateral_active=True, gps=gps(mocker), now=1001.0) is None
