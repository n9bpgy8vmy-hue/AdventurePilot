import json
import math
import time

from opendbc.car import structs

from openpilot.common.constants import CV
from openpilot.common.params import Params
from openpilot.common.realtime import DT_CTRL
from openpilot.common.swaglog import cloudlog


MAX_LOCATIONS = 200
MERGE_DISTANCE_METERS = 30.0
CORRECTION_DURATION_SECONDS = 0.5
APPROACH_CHECK_PERIOD_SECONDS = 0.5
NEW_LOCATION_ALERT_SUPPRESSION_SECONDS = 60.0
POINT_ALERT_COOLDOWN_SECONDS = 60.0


def distance_meters(lat1: float, lon1: float, lat2: float, lon2: float) -> float:
  radius = 6_371_000.0
  phi1, phi2 = math.radians(lat1), math.radians(lat2)
  d_phi = math.radians(lat2 - lat1)
  d_lambda = math.radians(lon2 - lon1)
  a = math.sin(d_phi / 2) ** 2 + math.cos(phi1) * math.cos(phi2) * math.sin(d_lambda / 2) ** 2
  return radius * 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))


class LaneHuggingObserver:
  """Observe driver lateral corrections and alert near previously observed locations.

  This class is deliberately non-actuating: it never changes steering, lateral-active, or MADS state.
  """

  def __init__(self, params: Params | None = None):
    self.params = params or Params()
    self.enabled = False
    self.feature_logging = False
    self.is_metric = False
    self.min_speed = 25
    self.alert_distance = 150
    self.locations = self._load_locations()
    self.correction_ticks = 0
    self.correction_latched = False
    self.last_params_read = 0.0
    self.last_approach_check = 0.0
    self.alerted_points: dict[str, float] = {}
    self.get_params()

  def _load_locations(self) -> list[dict]:
    try:
      raw = self.params.get("RivianPilotLaneHuggingLocations")
      locations = json.loads(raw) if raw else []
      return locations[-MAX_LOCATIONS:] if isinstance(locations, list) else []
    except (TypeError, ValueError, json.JSONDecodeError):
      cloudlog.event("rivianpilot feature error", feature="lane_hugging_observer", errors=["invalid_location_database"])
      return []

  def get_params(self) -> None:
    self.enabled = self.params.get_bool("RivianPilotLaneHuggingObserver")
    self.feature_logging = self.params.get_bool("RivianPilotFeatureLogging")
    self.is_metric = self.params.get_bool("IsMetric")
    self.min_speed = self.params.get("RivianPilotLaneHuggingMinSpeed", return_default=True)
    self.alert_distance = self.params.get("RivianPilotLaneHuggingAlertDistance", return_default=True)

  def _log(self, action: str, **kwargs) -> None:
    if self.feature_logging:
      cloudlog.event("rivianpilot lane hugging observer", action=action, **kwargs)

  def _save_locations(self) -> None:
    self.locations = sorted(self.locations, key=lambda point: point.get("last_seen", 0))[-MAX_LOCATIONS:]
    self.params.put("RivianPilotLaneHuggingLocations", json.dumps(self.locations, separators=(",", ":")), block=False)

  def _record_correction(self, latitude: float, longitude: float, direction: str, now: float, speed_ms: float) -> None:
    nearest = None
    nearest_distance = MERGE_DISTANCE_METERS
    for point in self.locations:
      if point.get("direction") != direction:
        continue
      distance = distance_meters(latitude, longitude, point["latitude"], point["longitude"])
      if distance <= nearest_distance:
        nearest = point
        nearest_distance = distance

    if nearest is None:
      nearest = {
        "id": f"{latitude:.6f},{longitude:.6f},{direction}",
        "latitude": round(latitude, 7),
        "longitude": round(longitude, 7),
        "direction": direction,
        "count": 0,
        "first_seen": now,
      }
      self.locations.append(nearest)

    nearest["count"] = int(nearest.get("count", 0)) + 1
    nearest["last_seen"] = now
    self._save_locations()
    self._log("correction_recorded", direction=direction, latitude=latitude, longitude=longitude,
              speed_ms=round(speed_ms, 3), observations=nearest["count"])

  def _approach_alert(self, latitude: float, longitude: float, now: float) -> str | None:
    if now - self.last_approach_check < APPROACH_CHECK_PERIOD_SECONDS:
      return None
    self.last_approach_check = now

    nearest = None
    nearest_distance = float(self.alert_distance)
    for point in self.locations:
      if now - point.get("last_seen", 0) < NEW_LOCATION_ALERT_SUPPRESSION_SECONDS:
        continue
      distance = distance_meters(latitude, longitude, point["latitude"], point["longitude"])
      if distance <= nearest_distance:
        nearest = point
        nearest_distance = distance

    if nearest is None:
      return None

    point_id = nearest.get("id", f"{nearest['latitude']},{nearest['longitude']},{nearest['direction']}")
    if now - self.alerted_points.get(point_id, 0) < POINT_ALERT_COOLDOWN_SECONDS:
      return None

    self.alerted_points[point_id] = now
    self._log("approach_alert", direction=nearest["direction"], distance_m=round(nearest_distance, 1),
              observations=nearest.get("count", 1))
    return nearest["direction"]

  def update(self, CS: structs.CarState, lateral_active: bool, gps, now: float | None = None) -> str | None:
    now = time.time() if now is None else now
    monotonic_now = time.monotonic()
    if monotonic_now - self.last_params_read > 10.0:
      self.get_params()
      self.last_params_read = monotonic_now

    if not self.enabled or gps is None or not gps.hasFix:
      self.correction_ticks = 0
      self.correction_latched = False
      return None

    speed_factor = CV.KPH_TO_MS if self.is_metric else CV.MPH_TO_MS
    correction_candidate = lateral_active and CS.steeringPressed and not (CS.leftBlinker or CS.rightBlinker) and \
                           CS.vEgo >= self.min_speed * speed_factor

    if correction_candidate:
      self.correction_ticks += 1
      if not self.correction_latched and self.correction_ticks * DT_CTRL >= CORRECTION_DURATION_SECONDS:
        direction = "left" if CS.steeringTorque > 0 else "right"
        self._record_correction(gps.latitude, gps.longitude, direction, now, CS.vEgo)
        self.correction_latched = True
    elif not CS.steeringPressed:
      self.correction_ticks = 0
      self.correction_latched = False

    return self._approach_alert(gps.latitude, gps.longitude, now)
