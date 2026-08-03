from types import SimpleNamespace

from openpilot.system.manager.process_config import run_rivian_vision_bsm_observer


class FakeParams:
  def __init__(self, enabled, bench=False):
    self.enabled = enabled
    self.bench = bench

  def get_bool(self, key):
    return {
      "RivianPilotVisionBSMEnabled": self.enabled,
      "RivianPilotVisionBSMBenchMode": self.bench,
    }[key]


def test_detector_process_is_rivian_onroad_and_opt_in_only():
  rivian = SimpleNamespace(brand="rivian")
  other = SimpleNamespace(brand="toyota")

  assert run_rivian_vision_bsm_observer(True, FakeParams(True), rivian)
  assert run_rivian_vision_bsm_observer(False, FakeParams(True, bench=True), rivian)
  assert not run_rivian_vision_bsm_observer(False, FakeParams(True), rivian)
  assert not run_rivian_vision_bsm_observer(True, FakeParams(False), rivian)
  assert not run_rivian_vision_bsm_observer(True, FakeParams(True), other)
