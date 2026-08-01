# Vision-BSM model

`vision_bsm_model.onnx` is the OpenCV-compatible V-ASM model ported from
[StarPilot PR #75](https://github.com/firestar5683/StarPilot/pull/75) and its
subsequent hardened integration on StarPilot's `Dom` branch. The RivianPilot
integration keeps the detector isolated from steering and uses its output only
as an optional lane-change veto and visual/audible warning source.
