from __future__ import annotations

from pathlib import Path
import sys
import time


# Prebuilt devices use comma's fixed runtime environment, which does not include
# OpenCV. The prebuilt workflow vendors the ARM64 wheel here so updater cleanup
# cannot remove it as an untracked installation.
VISION_BSM_VENDOR_DIR = Path(__file__).resolve().parents[2] / "third_party" / "vision_bsm_python"
if VISION_BSM_VENDOR_DIR.is_dir():
  sys.path.insert(0, str(VISION_BSM_VENDOR_DIR))

import cv2
import numpy as np

try:
  import onnxruntime as ort
except ImportError:
  ort = None


ASSETS_DIR = Path(__file__).resolve().parent / "assets"
VISION_BSM_MODEL_PATH = ASSETS_DIR / "vision_bsm_model.onnx"

MODEL_INPUT_H = 256
MODEL_INPUT_W = 352
HYSTERESIS_ON = 0.65
HYSTERESIS_OFF = 0.25


class VisionBSMInference:
  """PR #75 detector, isolated from lane-change and steering decisions."""

  def __init__(self, model_path: Path = VISION_BSM_MODEL_PATH):
    self.model_path = model_path
    self.net = None
    self.session = None
    self.input_name = ""
    self.output_name = ""
    self.backend = "none"
    self._valid = False
    self.last_error = ""
    self.frame_res = (0, 0)
    self.config_width = 0
    self.config_height = 0
    self.masks = {"left": None, "right": None}
    self.bboxes = {"left": None, "right": None, "left_raw": None, "right_raw": None}
    self.reset_state()

  def load(self) -> bool:
    if not self.model_path.is_file():
      self.last_error = f"Missing model: {self.model_path}"
      self._valid = False
      return False
    try:
      if ort is not None:
        options = ort.SessionOptions()
        options.intra_op_num_threads = 4
        options.inter_op_num_threads = 1
        options.execution_mode = ort.ExecutionMode.ORT_SEQUENTIAL
        options.graph_optimization_level = ort.GraphOptimizationLevel.ORT_ENABLE_ALL
        options.log_severity_level = 3
        self.session = ort.InferenceSession(str(self.model_path), sess_options=options,
                                            providers=["CPUExecutionProvider"])
        self.input_name = self.session.get_inputs()[0].name
        self.output_name = self.session.get_outputs()[0].name
        self.backend = "onnxruntime"
      else:
        # Source-tree developer tests may not have the prebuilt-only runtime.
        # Production packaging requires ONNX Runtime; OpenCV is only a
        # fail-silent compatibility fallback.
        self.net = cv2.dnn.readNetFromONNX(str(self.model_path))
        self.net.setPreferableBackend(cv2.dnn.DNN_BACKEND_OPENCV)
        self.net.setPreferableTarget(cv2.dnn.DNN_TARGET_CPU)
        self.backend = "opencv"
      self._valid = True
      self.last_error = ""
    except Exception as exc:
      self.last_error = f"Failed to load model: {exc}"
      self._valid = False
    return self._valid

  def warmup(self) -> tuple[bool, float]:
    """Run one synthetic inference so first use on-road does no lazy setup."""
    if not self.valid:
      return False, 0.0
    started = time.monotonic()
    try:
      blob = np.zeros((1, 3, MODEL_INPUT_H, MODEL_INPUT_W), dtype=np.float32)
      if self.session is not None:
        self.session.run([self.output_name], {self.input_name: blob})
      elif self.net is not None:
        self.net.setInput(blob)
        self.net.forward()
      else:
        raise RuntimeError("No inference backend")
      self.last_error = ""
      return True, (time.monotonic() - started) * 1000.0
    except Exception as exc:
      self.last_error = f"Failed to warm up model: {exc}"
      self._valid = False
      return False, (time.monotonic() - started) * 1000.0

  @property
  def valid(self) -> bool:
    return self._valid

  @property
  def configured_sides(self) -> tuple[str, ...]:
    return tuple(side for side in ("left", "right") if self.bboxes.get(f"{side}_raw") is not None)

  def reset_state(self) -> None:
    self._scores = {"left": 0.0, "right": 0.0}
    self.active = {"left": False, "right": False}
    self.confidence = {"left": 0.0, "right": 0.0}

  def load_config(self, config: dict) -> None:
    self.frame_res = (0, 0)
    self.config_width = int(config.get("width", 0))
    self.config_height = int(config.get("height", 0))
    for side in ("left", "right"):
      poly = config.get(f"poly_{side}", [])
      self.bboxes[f"{side}_raw"] = np.array(poly, dtype=np.float32) if len(poly) >= 3 else None
      if len(poly) < 3:
        self.bboxes[side] = None
        self.masks[side] = None

  def _prepare_geometry(self, height: int, width: int) -> None:
    if (height, width) == self.frame_res:
      return
    self.frame_res = (height, width)
    scale_x = width / float(self.config_width) if self.config_width > 0 else 1.0
    scale_y = height / float(self.config_height) if self.config_height > 0 else 1.0

    for side in ("left", "right"):
      raw_points = self.bboxes.get(f"{side}_raw")
      if raw_points is None:
        self.bboxes[side] = None
        self.masks[side] = None
        continue

      points = raw_points.copy()
      points[:, 0] *= scale_x
      points[:, 1] *= scale_y
      x, y, w, h = cv2.boundingRect(points.astype(np.int32))
      x, y = (x // 2) * 2, (y // 2) * 2
      w, h = ((w + 1) // 2) * 2, ((h + 1) // 2) * 2
      x, y = max(0, min(x, width - 2)), max(0, min(y, height - 2))
      w, h = max(2, min(w, width - x)), max(2, min(h, height - y))
      w, h = (w // 2) * 2, (h // 2) * 2
      self.bboxes[side] = (x, y, w, h)

      mask = np.zeros((h, w), dtype=np.uint8)
      cv2.fillPoly(mask, [points.astype(np.int32) - [x, y]], 255)
      self.masks[side] = mask

  def _run_inference(self, raw_image, camera_height: int, side: str) -> float:
    bbox = self.bboxes[side]
    if bbox is None or (self.session is None and self.net is None):
      return 0.0
    x, y, w, h = bbox
    y_crop = raw_image[y:y + h, x:x + w]
    uv_crop = raw_image[camera_height + y // 2:camera_height + (y + h) // 2, x:x + w]
    nv12_crop = np.vstack([y_crop, uv_crop])
    crop_rgb = cv2.cvtColor(nv12_crop, cv2.COLOR_YUV2RGB_NV12)
    if self.masks[side] is not None:
      crop_rgb = cv2.bitwise_and(crop_rgb, crop_rgb, mask=self.masks[side])

    resized = cv2.resize(crop_rgb, (MODEL_INPUT_W, MODEL_INPUT_H), interpolation=cv2.INTER_LINEAR)
    blob = np.expand_dims(np.transpose(resized.astype(np.float32) / 255.0, (2, 0, 1)), axis=0)
    if self.session is not None:
      predictions = np.squeeze(self.session.run([self.output_name], {self.input_name: blob})[0])
    else:
      self.net.setInput(blob)
      predictions = np.squeeze(self.net.forward())
    if predictions.ndim == 2:
      if predictions.shape[0] < predictions.shape[1]:
        predictions = predictions.T
      if predictions.shape[1] >= 6:
        relevant = predictions[np.round(predictions[:, 5]).astype(int) == 0]
        return 0.0 if len(relevant) == 0 else float(np.max(relevant[:, 4]))
      if predictions.shape[1] >= 5:
        return float(np.max(predictions[:, 4]))
      return float(np.max(predictions[:, 0]))
    return float(np.max(predictions)) if predictions.ndim == 1 and predictions.size else 0.0

  def update(self, raw_image, width: int, height: int, dt: float, confidence_threshold: float,
             smooth_seconds: float, side: str) -> tuple[bool, bool]:
    if not self.valid or side not in self.configured_sides:
      return self.active["left"], self.active["right"]
    self._prepare_geometry(height, width)
    alpha = min(1.0, dt / max(smooth_seconds, 0.001))
    raw_confidence = self._run_inference(raw_image, height, side)
    self.confidence[side] = raw_confidence
    if raw_confidence >= confidence_threshold:
      self._scores[side] = min(1.0, self._scores[side] + alpha)
    else:
      self._scores[side] = max(0.0, self._scores[side] - alpha)
    if self._scores[side] >= HYSTERESIS_ON:
      self.active[side] = True
    elif self._scores[side] <= HYSTERESIS_OFF:
      self.active[side] = False
    return self.active["left"], self.active["right"]
