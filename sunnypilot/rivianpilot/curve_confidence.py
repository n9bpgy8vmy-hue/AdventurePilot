import math


CURVE_LAT_ACCEL_MIN = 0.35


class CurveConfidenceInputUnavailable(Exception):
  pass


def _finite(value) -> float:
  value = float(value)
  if not math.isfinite(value):
    raise CurveConfidenceInputUnavailable("non-finite curve confidence input")
  return value


def _clip(value: float, low: float = 0.0, high: float = 1.0) -> float:
  return max(low, min(high, value))


def calculate_curve_confidence(model, controls, car_state, calibrated: bool,
                               previous_path_y: float | None) -> tuple[float, bool, str, float]:
  """Pure, read-only curve confidence calculation with no UI or messaging dependencies."""
  desired_curvature = _finite(controls.desiredCurvature)
  actual_curvature = _finite(controls.curvature)
  speed = max(_finite(car_state.vEgo), 0.0)
  desired_lat_accel = abs(desired_curvature) * speed * speed
  actual_lat_accel = abs(actual_curvature) * speed * speed
  in_curve = max(desired_lat_accel, actual_lat_accel) >= CURVE_LAT_ACCEL_MIN

  lane_probs = [_finite(v) for v in model.laneLineProbs]
  if len(lane_probs) < 3:
    raise CurveConfidenceInputUnavailable("lane confidence unavailable")
  lane_quality = _clip((lane_probs[1] + lane_probs[2]) / 2.0)

  edge_stds = [abs(_finite(v)) for v in model.roadEdgeStds]
  if len(edge_stds) < 2:
    raise CurveConfidenceInputUnavailable("road edge confidence unavailable")
  edge_quality = 1.0 - _clip((edge_stds[0] + edge_stds[1]) / 2.0)

  path_y = [_finite(v) for v in model.position.y]
  if len(path_y) < 6:
    raise CurveConfidenceInputUnavailable("model path unavailable")
  path_sample = path_y[min(10, len(path_y) - 1)]
  path_quality = 1.0 if previous_path_y is None else 1.0 - _clip(abs(path_sample - previous_path_y))

  curvature_scale = max(abs(desired_curvature), 0.002)
  tracking_quality = 1.0 - _clip(abs(desired_curvature - actual_curvature) / curvature_scale)

  saturated = False
  try:
    lateral_state = controls.lateralControlState
    lateral_log = getattr(lateral_state, lateral_state.which())
    saturated = bool(getattr(lateral_log, "saturated", False))
  except Exception:
    saturated = False

  driver_penalty = 10.0 if car_state.steeringPressed and abs(_finite(car_state.steeringTorque)) > 1.0 else 0.0
  score = (
    lane_quality * 25.0 +
    edge_quality * 15.0 +
    path_quality * 20.0 +
    tracking_quality * 20.0 +
    (0.0 if saturated else 10.0) +
    (10.0 if calibrated else 0.0) -
    driver_penalty
  )
  immediate_red = saturated and tracking_quality < 0.35
  reason = "controller tracking" if immediate_red else \
           "lane and path confidence" if lane_quality < 0.5 and edge_quality < 0.5 else \
           "curve prediction"
  return max(0.0, min(100.0, score)), in_curve, reason, path_sample
