from types import SimpleNamespace

from openpilot.system.manager.process_config import run_rivian_vision_bsm


class FakeParams:
  def __init__(self, enabled):
    self.enabled = enabled

  def get_bool(self, key):
    assert key == "RivianPilotVisionBSMEnabled"
    return self.enabled


def test_detector_process_is_rivian_onroad_and_opt_in_only():
  rivian = SimpleNamespace(brand="rivian")
  other = SimpleNamespace(brand="toyota")

  assert run_rivian_vision_bsm(True, FakeParams(True), rivian)
  assert not run_rivian_vision_bsm(False, FakeParams(True), rivian)
  assert not run_rivian_vision_bsm(True, FakeParams(False), rivian)
  assert not run_rivian_vision_bsm(True, FakeParams(True), other)
