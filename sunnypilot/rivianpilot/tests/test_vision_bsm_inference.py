from pathlib import Path

import numpy as np

from openpilot.sunnypilot.rivianpilot.vision_bsm_inference import (
  MODEL_INPUT_H,
  MODEL_INPUT_W,
  VISION_BSM_MODEL_PATH,
  VisionBSMInference,
)


def test_single_side_polygon_geometry():
  inference = VisionBSMInference(Path("unused.onnx"))
  inference.load_config({
    "width": 200,
    "height": 100,
    "poly_left": [[10, 10], [80, 10], [80, 80], [10, 80]],
    "poly_right": [],
  })
  inference._prepare_geometry(100, 200)

  assert inference.configured_sides == ("left",)
  assert inference.bboxes["left"] is not None
  assert inference.bboxes["right"] is None


def test_bundled_model_loads_and_runs_with_opencv():
  inference = VisionBSMInference(VISION_BSM_MODEL_PATH)
  assert inference.load(), inference.last_error
  inference.net.setInput(np.zeros((1, 3, MODEL_INPUT_H, MODEL_INPUT_W), dtype=np.float32))
  assert inference.net.forward().shape == (1, 299, 6)
