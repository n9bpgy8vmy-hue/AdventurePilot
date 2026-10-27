"""Conservative lane-change *initiation* gate; never certifies an empty/safe lane.

Uses the existing model output, not a second inference engine. Thresholds are
development defaults pending route replay and vehicle validation.
"""
import math
import time
from bisect import bisect_right


MAX_AGE = 0.25
CONFIRM_SECONDS = 1.0
MIN_PROB = 0.8
MAX_STD = 0.35
MIN_WIDTH = 3.0
MAX_WIDTH = 4.5
MAX_WIDTH_CHANGE = 0.6
HALF_VEHICLE_WIDTH = 1.125  # conservative development envelope, not a clearance measurement
EDGE_BUFFER = 0.35

REASON_TEXT = {
  "camera_unavailable": "Camera data unavailable",
  "geometry_invalid": "Lane geometry unavailable",
  "lane_uncertain": "Adjacent lane uncertain",
  "lane_width": "Adjacent lane too narrow",
  "lane_diverging": "Lane layout uncertain",
  "edge_uncertain": "Road edge uncertain",
  "edge_clearance": "Road edge too close",
  "confirming": "Adjacent lane unconfirmed",
}


def model_inputs_valid(sm, now):
  """Frame freshness is checked separately against the camera capture timestamp."""
  services = ("carState", "carControl", "liveCalibration")
  return (sm.all_checks(list(services)) and
          sm["liveCalibration"].calStatus == "calibrated" and
          all(0 <= now - sm.logMonoTime[s] * 1e-9 <= MAX_AGE for s in services[:2]))


def blocked_reason_from_messages(sm, now=None):
  """Display only: require status from the same model frame; never used to steer."""
  if not sm["modelDataV2SP"].laneChangeGuardEnabled:
    return ""
  now = time.monotonic() if now is None else now
  services = ("modelV2", "modelDataV2SP")
  # selfdrived ignores this optional service in its global health checks. Check
  # its actual validity/liveness/age explicitly before displaying its result.
  if not all(sm.valid[s] and sm.alive[s] and 0 <= now - sm.logMonoTime[s] / 1e9 <= MAX_AGE for s in services):
    return "camera_unavailable"
  status = sm["modelDataV2SP"]
  model = sm["modelV2"]
  if not 0 <= now - model.timestampEof / 1e9 <= MAX_AGE:
    return "camera_unavailable"
  direction = -1 if model.meta.laneChangeDirection == "left" else 1
  if (not status.laneChangeGuardEnabled or status.laneChangeGuardFrameId != model.frameId or
      status.laneChangeGuardDirection != direction):
    return "camera_unavailable"
  return str(status.laneChangeGuardReason)


def _curve(line):
  xs, ys = tuple(line.x), tuple(line.y)
  if not 2 <= len(xs) <= 64 or len(xs) != len(ys):
    raise ValueError("malformed line")
  if not all(math.isfinite(v) for v in (*xs, *ys)) or any(b <= a for a, b in zip(xs, xs[1:], strict=False)):
    raise ValueError("nonfinite/unordered line")
  return xs, ys


def _at(curve, x):
  xs, ys = curve
  if not xs[0] <= x <= xs[-1]:
    raise ValueError("insufficient lookahead")
  i = min(bisect_right(xs, x), len(xs) - 1)
  return ys[i - 1] + (ys[i] - ys[i - 1]) * (x - xs[i - 1]) / (xs[i] - xs[i - 1])


def geometry_reason(model, side, v_ego):
  """side -1 = vehicle left; model y increases to the right.

  Three confident lane lines must define the current and requested adjacent
  lane. A road edge alone is never positive evidence of another travel lane.
  """
  try:
    indices = (0, 1, 2) if side == -1 else (1, 2, 3)
    if len(model.laneLines) != 4 or len(model.roadEdges) != 2:
      return "geometry_invalid"
    for i in indices:
      prob, std = model.laneLineProbs[i], model.laneLineStds[i]
      if not (math.isfinite(prob) and MIN_PROB <= prob <= 1 and math.isfinite(std) and 0 <= std <= MAX_STD):
        return "lane_uncertain"
    edge_index = 0 if side == -1 else 1
    edge_std = model.roadEdgeStds[edge_index]
    if not math.isfinite(edge_std) or not 0 <= edge_std <= MAX_STD:
      return "edge_uncertain"

    lines = {i: _curve(model.laneLines[i]) for i in indices}
    edge = _curve(model.roadEdges[edge_index])
    horizon = min(90., max(30., v_ego * 3.))
    # Include model knots as well as 5 m samples to avoid interpolating over a
    # narrow/invalid segment. Do not extrapolate beyond any curve's horizon.
    samples = {5., horizon, *range(10, int(horizon), 5)}
    for xs, _ in (*lines.values(), edge):
      samples.update(x for x in xs if 5 <= x <= horizon)
    current_widths, adjacent_widths = [], []
    for x in sorted(samples):
      left, right = _at(lines[1], x), _at(lines[2], x)
      outer = _at(lines[0 if side == -1 else 3], x)
      inner = left if side == -1 else right
      current_width, adjacent_width = right - left, side * (outer - inner)
      if not (MIN_WIDTH <= current_width <= MAX_WIDTH and MIN_WIDTH <= adjacent_width <= MAX_WIDTH):
        return "lane_width"
      center = (outer + inner) / 2
      road_edge = _at(edge, x)
      if (side * (road_edge - outer) < -0.2 or
          side * (road_edge - center) < HALF_VEHICLE_WIDTH + EDGE_BUFFER + 2 * edge_std):
        return "edge_clearance"
      current_widths.append(current_width)
      adjacent_widths.append(adjacent_width)
    if any(max(widths) - min(widths) > MAX_WIDTH_CHANGE for widths in (current_widths, adjacent_widths)):
      return "lane_diverging"
    return ""
  except (AttributeError, IndexError, TypeError, ValueError, OverflowError):
    return "geometry_invalid"


class LaneChangeGeometryGuard:
  def __init__(self):
    self.since = {-1: None, 1: None}
    self.last_capture = None

  def update(self, model, v_ego, now, valid):
    try:
      capture = model.timestampEof / 1e9
      fresh = (valid and math.isfinite(now) and math.isfinite(v_ego) and v_ego >= 0 and
               math.isfinite(capture) and capture > 0 and 0 <= now - capture <= MAX_AGE and
               (self.last_capture is None or capture > self.last_capture))
    except (AttributeError, TypeError, ValueError, OverflowError):
      capture, fresh = None, False
    if not fresh:
      self.since = {-1: None, 1: None}
      return {-1: "camera_unavailable", 1: "camera_unavailable"}
    if self.last_capture is None or capture - self.last_capture > MAX_AGE:
      self.since = {-1: None, 1: None}
    self.last_capture = capture
    reasons = {}
    for side in (-1, 1):
      reason = geometry_reason(model, side, v_ego)
      if reason:
        self.since[side] = None
      elif self.since[side] is None:
        self.since[side] = capture
      reasons[side] = reason or ("confirming" if capture - self.since[side] < CONFIRM_SECONDS else "")
    return reasons
