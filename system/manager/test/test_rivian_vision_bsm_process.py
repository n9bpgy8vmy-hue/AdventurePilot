from types import SimpleNamespace

from openpilot.system.manager.process_config import camerad_required, run_rivian_vision_bsm_observer


class FakeParams:
  def __init__(self, enabled, bench=False):
    self.enabled = enabled
    self.bench = bench

  def get_bool(self, key):
    return {
      "RivianPilotVisionBSMEnabled": self.enabled,
      "RivianPilotVisionBSMBenchMode": self.bench,
      "IsDriverViewEnabled": False,
    }[key]


def test_detector_process_is_started_or_bench_and_opt_in_only():
  rivian = SimpleNamespace(brand="rivian")
  other = SimpleNamespace(brand="toyota")
  unknown = SimpleNamespace(brand="")

  assert run_rivian_vision_bsm_observer(True, FakeParams(True), rivian)
  assert run_rivian_vision_bsm_observer(False, FakeParams(True, bench=True), rivian)
  assert run_rivian_vision_bsm_observer(True, FakeParams(True), other)
  assert run_rivian_vision_bsm_observer(False, FakeParams(True, bench=True), unknown)
  assert not run_rivian_vision_bsm_observer(False, FakeParams(True), rivian)
  assert not run_rivian_vision_bsm_observer(True, FakeParams(False), rivian)
  assert not run_rivian_vision_bsm_observer(False, FakeParams(False, bench=True), unknown)


def test_bench_mode_starts_camera_without_starting_driver_preview():
  unknown = SimpleNamespace(brand="")

  assert camerad_required(False, FakeParams(True, bench=True), unknown)
  assert not camerad_required(False, FakeParams(True), unknown)
  assert not camerad_required(False, FakeParams(False, bench=True), unknown)
