from unittest.mock import Mock

from openpilot.sunnypilot.livedelay.helpers import get_lat_delay


def test_learning_uses_published_delay_instead_of_stale_cache():
  params = Mock()
  params.get_bool.return_value = True
  params.get.return_value = 0.8
  assert get_lat_delay(params, 0.23) == 0.23
  params.get.assert_not_called()


def test_fixed_mode_uses_cached_actuator_plus_software_delay():
  params = Mock()
  params.get_bool.return_value = False
  params.get.return_value = 0.45
  assert get_lat_delay(params, 0.23) == 0.45
  params.get.assert_called_once_with("LagdValueCache", return_default=True)
