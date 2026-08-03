from openpilot.sunnypilot.rivianpilot.vision_bsmd import (
  CPU_TRIP_SECONDS,
  LATENCY_TRIP_MS,
  MAX_SLOW_INFERENCES,
  OVERLOAD_COOLDOWN_SECONDS,
  VisionBSMDaemon,
)


def daemon_for_guard():
  daemon = VisionBSMDaemon.__new__(VisionBSMDaemon)
  daemon._slow_inferences = 0
  daemon._cooldown_until = 0.0
  daemon._cooldown_count = 0
  daemon._cpu_overload_since = 0.0
  daemon._set_inactive = lambda reset=False: None
  daemon._log = lambda *args, **kwargs: None
  return daemon


def test_extreme_latency_immediately_enters_cooldown():
  daemon = daemon_for_guard()
  daemon._record_latency(LATENCY_TRIP_MS, 100.0)
  assert daemon._cooldown_until == 100.0 + OVERLOAD_COOLDOWN_SECONDS
  assert daemon._cooldown_count == 1


def test_recovery_prevents_sporadic_slow_samples_from_tripping():
  daemon = daemon_for_guard()
  for _ in range(MAX_SLOW_INFERENCES - 1):
    daemon._record_latency(LATENCY_TRIP_MS - 1.0, 100.0)
  daemon._record_latency(20.0, 101.0)
  assert daemon._cooldown_until == 0.0


def test_sustained_cpu_pressure_enters_cooldown():
  daemon = daemon_for_guard()
  usage = [95.0] * 8
  assert not daemon._cpu_guard_tripped(usage, 100.0)
  assert daemon._cpu_guard_tripped(usage, 100.0 + CPU_TRIP_SECONDS)
  assert daemon._cooldown_count == 1


def test_brief_cpu_spike_recovers_without_cooldown():
  daemon = daemon_for_guard()
  assert not daemon._cpu_guard_tripped([95.0] * 8, 100.0)
  assert not daemon._cpu_guard_tripped([20.0] * 8, 100.5)
  assert daemon._cpu_overload_since == 0.0
