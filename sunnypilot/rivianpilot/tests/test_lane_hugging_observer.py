from types import SimpleNamespace

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
    "RivianPilotLaneHuggingLocations": locations or [],
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


def model_and_controls():
  lane_lines = [
    SimpleNamespace(x=[0.0, 20.0], y=[0.0, 0.0]),
    SimpleNamespace(x=[0.0, 20.0], y=[-1.8, -1.6]),
    SimpleNamespace(x=[0.0, 20.0], y=[1.8, 2.0]),
    SimpleNamespace(x=[0.0, 20.0], y=[0.0, 0.0]),
  ]
  model = SimpleNamespace(
    laneLines=lane_lines,
    laneLineProbs=[0.0, 0.91, 0.87, 0.0],
    position=SimpleNamespace(x=[0.0, 20.0], y=[0.0, 0.35]),
  )
  lateral_log = SimpleNamespace(saturated=True, output=0.22)
  lateral_state = SimpleNamespace(which=lambda: "torqueState", torqueState=lateral_log)
  controls = SimpleNamespace(curvature=0.0015, desiredCurvature=0.002, lateralControlState=lateral_state)
  car_control = SimpleNamespace(actuators=SimpleNamespace(torque=0.22))
  car_output = SimpleNamespace(actuatorsOutput=SimpleNamespace(torque=0.20))
  return model, controls, car_control, car_output


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
  assert params.put.call_args.args[1] == observer.locations


def test_storage_failure_is_suppressed_and_logged_once(mocker):
  params = make_params(mocker)
  params.put.side_effect = TypeError("storage type mismatch")
  log_event = mocker.patch("openpilot.sunnypilot.rivianpilot.lane_hugging_observer.cloudlog.event")
  observer = LaneHuggingObserver(params)
  CS = car_state(torque=1.0)

  for _ in range(round(CORRECTION_DURATION_SECONDS / DT_CTRL)):
    observer.update(CS, lateral_active=True, gps=gps(mocker), now=1000.0)

  assert observer.storage_faulted
  assert log_event.call_count == 1

  observer._save_locations()
  assert params.put.call_count == 1
  assert log_event.call_count == 1


def test_native_json_locations_load_without_deserialization(mocker):
  locations = [{"id": "known", "latitude": 1.0, "longitude": 2.0, "direction": "left"}]
  observer = LaneHuggingObserver(make_params(mocker, locations))
  assert observer.locations == locations


def test_unexpected_runtime_failure_is_suppressed_once(mocker):
  observer = LaneHuggingObserver(make_params(mocker))
  log_event = mocker.patch("openpilot.sunnypilot.rivianpilot.lane_hugging_observer.cloudlog.event")

  observer.suppress_after_error(RuntimeError("test failure"))
  observer.suppress_after_error(RuntimeError("repeated failure"))

  assert observer.faulted
  assert not observer.enabled
  assert observer.update(car_state(), lateral_active=True, gps=gps(mocker), now=1000.0) is None
  assert log_event.call_count == 1


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


def test_logs_bounded_curve_geometry_and_correction_context(mocker):
  params = make_params(mocker)
  log_event = mocker.patch("openpilot.sunnypilot.rivianpilot.lane_hugging_observer.cloudlog.event")
  observer = LaneHuggingObserver(params)
  CS = car_state(speed_mph=45, torque=1.0)
  CS.steeringAngleDeg = 3.5
  model, controls, car_control, car_output = model_and_controls()

  for tick in range(round(CORRECTION_DURATION_SECONDS / DT_CTRL)):
    observer.update(CS, lateral_active=True, gps=gps(mocker), model=model, controls_state=controls,
                    car_control=car_control, car_output=car_output, now=1000.0 + tick * DT_CTRL)

  curve_events = [call.kwargs for call in log_event.call_args_list if call.kwargs.get("action") == "curve_sample"]
  correction_events = [call.kwargs for call in log_event.call_args_list if call.kwargs.get("action") == "correction_recorded"]
  assert len(curve_events) == 1
  assert len(correction_events) == 1
  assert curve_events[0]["lane_width_m"] == 3.6
  assert curve_events[0]["predicted_path_from_lane_center_m"] == 0.15
  assert correction_events[0]["steering_angle_deg"] == 3.5
  assert correction_events[0]["left_lane_probability"] == 0.91
  assert correction_events[0]["requested_steering_torque"] == 0.22
  assert correction_events[0]["applied_steering_torque"] == 0.20
  assert correction_events[0]["steering_torque_tracking_error"] == 0.02
  assert correction_events[0]["lateral_controller_saturated"]


def test_rich_diagnostic_failure_does_not_disable_observer(mocker):
  observer = LaneHuggingObserver(make_params(mocker))
  mocker.patch.object(observer, "_model_snapshot", side_effect=ValueError("bad model data"))
  model, controls, car_control, car_output = model_and_controls()

  observer.update(car_state(speed_mph=45), lateral_active=True, gps=gps(mocker), model=model,
                  controls_state=controls, car_control=car_control, car_output=car_output, now=1000.0)

  assert observer.enabled
  assert not observer.faulted
  assert not observer.feature_logging
  assert observer.diagnostic_error_logged
