from openpilot.sunnypilot.rivianpilot.vision_bsmd import (
  BASE_INTERVAL,
  CRITICAL_SERVICES,
  CPU_TRIP_SECONDS,
  DRIVING_STACK_STABLE_SECONDS,
  FOLLOWUP_INTERVAL,
  LATENCY_TRIP_MS,
  MAX_SLOW_INFERENCES,
  ONROAD_STARTUP_DELAY,
  VisionBSMDaemon,
)


def test_initial_opencv_experiment_is_capped_at_two_fps():
  assert BASE_INTERVAL == 0.5
  assert FOLLOWUP_INTERVAL == BASE_INTERVAL


class FakeSM:
  def __init__(self, healthy=True):
    self.valid = dict.fromkeys(CRITICAL_SERVICES, healthy)
    self.alive = dict.fromkeys(CRITICAL_SERVICES, healthy)
    self.freq_ok = dict.fromkeys(CRITICAL_SERVICES, healthy)


def daemon_for_guard():
  daemon = VisionBSMDaemon.__new__(VisionBSMDaemon)
  daemon._slow_inferences = 0
  daemon._cooldown_count = 0
  daemon._cpu_overload_since = 0.0
  daemon._tripped_for_drive = False
  daemon._trip_reason = ""
  daemon._set_inactive = lambda reset=False: None
  daemon._set_available = lambda available: None
  daemon._disconnect_camera = lambda: None
  daemon._log = lambda *args, **kwargs: None
  return daemon


def daemon_for_stability():
  daemon = VisionBSMDaemon.__new__(VisionBSMDaemon)
  daemon.sm = FakeSM(healthy=True)
  daemon._ready = True
  daemon._available = False
  daemon._tripped_for_drive = False
  daemon._trip_reason = ""
  daemon._onroad_since = 100.0
  daemon._stack_healthy_since = 0.0
  daemon._set_inactive = lambda reset=False: None
  daemon._disconnect_camera = lambda: None
  daemon._log = lambda *args, **kwargs: None
  daemon.params_memory = type("FakeParams", (), {
    "put_bool": lambda *args, **kwargs: None,
    "put": lambda *args, **kwargs: None,
  })()
  return daemon


def test_extreme_latency_immediately_trips_for_drive():
  daemon = daemon_for_guard()
  daemon._record_latency(LATENCY_TRIP_MS, 100.0)
  assert daemon._tripped_for_drive
  assert daemon._trip_reason == "inference_latency"
  assert daemon._cooldown_count == 1


def test_recovery_prevents_sporadic_slow_samples_from_tripping():
  daemon = daemon_for_guard()
  for _ in range(MAX_SLOW_INFERENCES - 1):
    daemon._record_latency(LATENCY_TRIP_MS - 1.0, 100.0)
  daemon._record_latency(20.0, 101.0)
  assert not daemon._tripped_for_drive


def test_sustained_cpu_pressure_trips_for_drive():
  daemon = daemon_for_guard()
  usage = [95.0] * 8
  assert not daemon._cpu_guard_tripped(usage, 100.0)
  assert daemon._cpu_guard_tripped(usage, 100.0 + CPU_TRIP_SECONDS)
  assert daemon._tripped_for_drive
  assert daemon._trip_reason == "cpu_pressure"
  assert daemon._cooldown_count == 1


def test_brief_cpu_spike_recovers_without_cooldown():
  daemon = daemon_for_guard()
  assert not daemon._cpu_guard_tripped([95.0] * 8, 100.0)
  assert not daemon._cpu_guard_tripped([20.0] * 8, 100.5)
  assert daemon._cpu_overload_since == 0.0


def test_inference_requires_healthy_driving_stack_and_cpu_headroom():
  daemon = daemon_for_guard()
  daemon.sm = FakeSM(healthy=True)
  daemon._last_resource_skip_log = 0.0
  daemon._cpu_usage = lambda: [25.0] * 8
  assert daemon._resources_allow_inference(100.0)

  daemon.sm = FakeSM(healthy=False)
  assert not daemon._resources_allow_inference(100.0)


def test_offroad_bench_inference_does_not_require_driving_stack():
  daemon = daemon_for_guard()
  daemon.sm = FakeSM(healthy=False)
  daemon._last_resource_skip_log = 0.0
  daemon._cpu_usage = lambda: [25.0] * 8
  assert daemon._resources_allow_inference(100.0, require_driving_stack=False)


def test_offroad_bench_still_requires_cpu_headroom():
  daemon = daemon_for_guard()
  daemon.sm = FakeSM(healthy=False)
  daemon._last_resource_skip_log = 0.0
  daemon._cpu_usage = lambda: [95.0, 95.0, 95.0, 95.0, 10.0, 10.0, 10.0, 10.0]
  assert not daemon._resources_allow_inference(100.0, require_driving_stack=False)


def test_inference_skips_when_multiple_cores_are_hot():
  daemon = daemon_for_guard()
  daemon.sm = FakeSM(healthy=True)
  daemon._last_resource_skip_log = 0.0
  daemon._cpu_usage = lambda: [95.0, 95.0, 95.0, 95.0, 10.0, 10.0, 10.0, 10.0]
  assert not daemon._resources_allow_inference(100.0)


def test_stack_must_be_continuously_healthy_after_startup_delay():
  daemon = daemon_for_stability()
  first_eligible = 100.0 + ONROAD_STARTUP_DELAY
  assert not daemon._update_stack_stability(True, first_eligible)
  assert not daemon._update_stack_stability(True, first_eligible + DRIVING_STACK_STABLE_SECONDS - 0.1)
  assert daemon._update_stack_stability(True, first_eligible + DRIVING_STACK_STABLE_SECONDS)
  assert daemon._available


def test_health_regression_after_availability_trips_for_drive():
  daemon = daemon_for_stability()
  first_eligible = 100.0 + ONROAD_STARTUP_DELAY
  daemon._update_stack_stability(True, first_eligible)
  assert daemon._update_stack_stability(True, first_eligible + DRIVING_STACK_STABLE_SECONDS)
  daemon.sm = FakeSM(healthy=False)
  assert not daemon._update_stack_stability(True, first_eligible + DRIVING_STACK_STABLE_SECONDS + 0.1)
  assert daemon._tripped_for_drive
  assert daemon._trip_reason == "critical_service_regression"


def test_offroad_never_reports_bsm_available():
  daemon = daemon_for_stability()
  daemon._available = True
  assert not daemon._update_stack_stability(False, 200.0)
  assert not daemon._available
