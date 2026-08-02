import json
from types import SimpleNamespace

from opendbc.car import structs

from openpilot.common.constants import CV
from openpilot.sunnypilot.rivianpilot.manual_turn_recorder import (
  MAX_SAMPLES,
  ManualTurnRecorder,
  angle_delta_deg,
)


def make_params(mocker, values=None):
  values = {
    "RivianPilotManualTurnRecorder": True,
    "RivianPilotFeatureLogging": True,
    "IsMetric": False,
    "RivianPilotManualTurnMaxSpeed": 35,
    "RivianPilotManualTurnMaxPackages": 50,
    "RivianPilotManualTurnDeleteIndex": 1,
    "RivianPilotManualTurnDeleteSelected": False,
    "RivianPilotManualTurnDeleteOldest": False,
    "RivianPilotManualTurnDeleteAll": False,
    **(values or {}),
  }
  params = mocker.MagicMock()
  params.get_bool.side_effect = lambda key: bool(values.get(key, False))
  params.get.side_effect = lambda key, **kwargs: values[key]
  return params, values


def car_state(blinker="none", speed_mph=15, steering_pressed=False, torque=0.0):
  CS = structs.CarState()
  CS.vEgo = speed_mph * CV.MPH_TO_MS
  CS.aEgo = 0.1
  CS.gearShifter = structs.CarState.GearShifter.drive
  CS.leftBlinker = blinker == "left"
  CS.rightBlinker = blinker == "right"
  CS.steeringPressed = steering_pressed
  CS.steeringTorque = torque
  CS.steeringTorqueEps = 0.2
  CS.steeringAngleDeg = torque * 12.0
  CS.steeringRateDeg = torque * 2.0
  return CS


def context(bearing=0.0):
  gps = SimpleNamespace(hasFix=True, latitude=32.814, longitude=-96.949, bearingDeg=bearing)
  measurement = lambda value: SimpleNamespace(valid=True, value=[0.0, 0.0, value])
  pose = SimpleNamespace(inputsOK=True, orientationNED=measurement(bearing / 180.0 * 3.141592653589793),
                         angularVelocityDevice=measurement(0.1))
  line = lambda y: SimpleNamespace(x=[0.0, 5.0, 10.0, 20.0], y=[y, y, y, y])
  model = SimpleNamespace(laneLineProbs=[0.0, 0.9, 0.85, 0.0],
                          laneLines=[line(-3.6), line(-1.8), line(1.8), line(3.6)],
                          position=SimpleNamespace(x=[0.0, 5.0, 10.0, 20.0], y=[0.0, 0.1, 0.3, 0.8]),
                          roadEdges=[line(-4.0), line(4.0)], roadEdgeStds=[0.1, 0.2])
  lateral_log = SimpleNamespace(saturated=False)
  lateral_state = SimpleNamespace(which=lambda: "torqueState", torqueState=lateral_log)
  controls = SimpleNamespace(curvature=0.003, desiredCurvature=0.0032, lateralControlState=lateral_state)
  car_control = SimpleNamespace(actuators=SimpleNamespace(torque=0.25), latActive=False)
  car_output = SimpleNamespace(actuatorsOutput=SimpleNamespace(torque=0.24))
  return gps, model, controls, car_control, car_output, pose


def update(recorder, CS, now, bearing):
  gps, model, controls, car_control, car_output, pose = context(bearing)
  recorder.update(CS, gps, model, controls, car_control, car_output, pose, now=now)


def record_left_turn(recorder, start=1000.0):
  for tick in range(15):
    update(recorder, car_state(), start + tick * 0.2, 0.0)
  for tick in range(26):
    update(recorder, car_state("left", steering_pressed=True, torque=1.0),
           start + 3.0 + tick * 0.2, tick * 2.0)
  for tick in range(17):
    update(recorder, car_state(steering_pressed=False), start + 8.2 + tick * 0.2, 50.0)


def test_records_one_removable_package_with_quality_confidence(mocker, tmp_path):
  params, _ = make_params(mocker)
  recorder = ManualTurnRecorder(params, str(tmp_path))

  record_left_turn(recorder)

  packages = list(tmp_path.glob("*.json"))
  assert len(packages) == 1
  package = json.loads(packages[0].read_text())
  assert package["direction"] == "left"
  assert package["confidence_type"] == "recording_quality_not_automation_safety"
  assert 75 <= package["confidence_score"] <= 100
  assert package["summary"]["heading_change_deg"] >= 45
  assert package["summary"]["settled_completion"]
  assert 1 <= len(package["samples"]) <= MAX_SAMPLES
  assert package["samples"][0]["left_blinker"] is False  # pre-roll context
  assert package["samples"][-1]["left_blinker"] is False  # post-turn context


def test_does_not_save_blinker_without_manual_turn(mocker, tmp_path):
  params, _ = make_params(mocker)
  recorder = ManualTurnRecorder(params, str(tmp_path))

  for tick in range(30):
    update(recorder, car_state("right", steering_pressed=False), 1000.0 + tick * 0.2, tick)
  for tick in range(17):
    update(recorder, car_state(), 1006.0 + tick * 0.2, 29.0)

  assert list(tmp_path.glob("*.json")) == []


def test_short_blinker_tap_is_latched_until_manual_turn(mocker, tmp_path):
  params, _ = make_params(mocker)
  recorder = ManualTurnRecorder(params, str(tmp_path))

  # Capture a tap between the recorder's 5 Hz sample boundaries.
  update(recorder, car_state(), 1000.0, 0.0)
  gps, model, controls, car_control, car_output, pose = context(0.0)
  recorder.update(car_state("right"), gps, model, controls, car_control, car_output, pose, now=1000.05)
  recorder.update(car_state(), gps, model, controls, car_control, car_output, pose, now=1000.10)

  for tick in range(30):
    update(recorder, car_state(steering_pressed=True, torque=1.0),
           1000.2 + tick * 0.2, -tick * 2.0)
  for tick in range(17):
    update(recorder, car_state(), 1006.2 + tick * 0.2, -58.0)

  packages = list(tmp_path.glob("*.json"))
  assert len(packages) == 1
  assert json.loads(packages[0].read_text())["direction"] == "right"


def test_selected_package_delete_is_oldest_first(mocker, tmp_path):
  params, values = make_params(mocker)
  recorder = ManualTurnRecorder(params, str(tmp_path))
  for name in ("001-left.json", "002-right.json", "003-left.json"):
    (tmp_path / name).write_text("{}")
  values["RivianPilotManualTurnDeleteIndex"] = 2
  values["RivianPilotManualTurnDeleteSelected"] = True

  recorder._process_delete_requests()

  assert sorted(path.name for path in tmp_path.glob("*.json")) == ["001-left.json", "003-left.json"]
  params.put_bool.assert_called_with("RivianPilotManualTurnDeleteSelected", False, block=False)


def test_retention_prunes_only_oldest_packages(mocker, tmp_path):
  params, _ = make_params(mocker, {"RivianPilotManualTurnMaxPackages": 2})
  recorder = ManualTurnRecorder(params, str(tmp_path))
  for package_id in ("001-left", "002-right", "003-left"):
    recorder._write_package({"package_id": package_id})

  assert sorted(path.stem for path in tmp_path.glob("*.json")) == ["002-right", "003-left"]


def test_storage_failure_is_contained(mocker, tmp_path):
  params, _ = make_params(mocker)
  invalid_dir = tmp_path / "not-a-directory"
  invalid_dir.write_text("file")
  recorder = ManualTurnRecorder(params, str(invalid_dir))

  recorder._write_package({"package_id": "test"})

  assert recorder.storage_faulted
  assert not recorder.faulted


def test_heading_delta_wraps_at_north():
  assert angle_delta_deg(350.0, 10.0) == 20.0
  assert angle_delta_deg(10.0, 350.0) == -20.0
