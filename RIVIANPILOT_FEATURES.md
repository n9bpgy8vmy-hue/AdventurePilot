# RivianPilot Feature Roadmap

This file is the canonical definition and status of RivianPilot features. Do not broaden a feature's behavior without explicit approval.

## Feature 1 — Reverse/MADS

Prevent unwanted lane centering while parking. After returning to Drive, MADS can resume above the configured speed following a configurable warning. Already-active MADS stays active if speed later falls below the engagement threshold.

Status: release-ready and vehicle-tested. Preserve the validated behavior without functional changes in `2026.007.002`.

## Feature 2 — Classic Post-Turn Resume

Pause MADS for a qualifying signaled low-speed manual turn, then warn and resume after detecting a stable lane or approved road-edge recovery path.

Status: vehicle-tested. `2026.007.006` distinguishes light, near-straight road-camber corrections from deliberate steering using driver torque and steering rate. Light correction pressure no longer restarts the stability timer; strong torque, rapid steering, significant wheel angle, yaw, blinkers, speed, and lane/road-edge confidence continue to block resume. The decision is logged for road validation.

## Feature 3 — Lane Position

Apply a bounded temporary model-camera offset on curves or after a deliberate steering nudge. Driver input, blinkers, inactive lateral control, invalid geometry, and unsafe boundary clearance suppress the contribution.

Modes:

- Off: no offset and no detailed samples.
- Observe: calculate and log the complete diagnostic dataset without publishing an offset.
- Go Live: apply the configured bounded offset and log the same dataset as Observe.
- Observe plus Go Live: one control execution and one diagnostic stream; never duplicate processing.

Status: Go Live has completed successful vehicle testing. `2026.007.002` preserves control behavior and expands diagnostics with torque, controller, curvature, steering, lane/road-edge confidence, boundary clearance, and requested/capped/published offset data.

`2026.007.016` adds bounded vehicle-width, boundary-buffer, and poorly-marked-road request settings plus an optional once-per-episode correction message/chime. The requested poorly-marked-road value is configurable from 1–10 inches; without trustworthy boundary clearance the effective runtime approval remains capped at three inches.

`2026.007.031` replaces curve-only lane preference with one unified Default/Center/Left/Right position. Default leaves the model path unchanged. Center corrects model bias toward trusted measured lane center; Left and Right add the configured 1–10-inch position bias on straight roads and curves. The implementation reuses the proven curve controller's geometry validation, compensated reference, fork hold, boundary clearance, ramp limiting, manual-nudge priority, logging, and fail-silent fallback. Automatic inside-curve avoidance remains a higher-priority one-sided constraint. Existing manual-nudge behavior is unchanged; its selectable hold duration now extends to 30 minutes.

`2026.007.032` isolates experimental Vision-BSM from driving-critical services. The daemon is explicitly optional to selfdrived, is restricted to Comma 4's noncritical CPU cluster, may load and warm its model only while off-road, and is capped at one blinker-gated inference per second. It remains observation-only, fail-silent, disabled by default, and never publishes vehicle controls.

`2026.007.033` separates measured center correction from Left/Right lane-position bias. Center correction is configurable from 1–10 inches; Left or Right adds its separately configured 1–10-inch bias, with the combined automatic request capped at 15 inches before the existing geometry, fork, boundary-clearance, and ramp limits. Curve and manual-nudge requests retain their 10-inch configuration limits. Observe-only calculations no longer compensate for an unpublished offset, and enabling Go Live always starts from zero with a fresh geometry reference.

## Feature 4 — Experimental Lane-Change-to-Turn Handover

Create a natural transfer of lateral authority from native Nudge lane change, to a driver-controlled turn, and back to MADS after the new road is stable.

Status: on hold. Feature 2 diagnostics must first establish real Rivian timing and state sequences. Any future implementation must be separate and mutually exclusive with Classic Post-Turn Resume.

## Feature 5 — Calibration Health

Report calibration quality, stability, drift, mount-angle consistency, and conditions that may reduce lateral confidence.

Status: backlog; diagnostic-only first phase.

## Feature 6 — Forward Collision Warning+

Provide a supplemental, model-based visual/audio collision warning without braking or steering commands. Rivian native FCW/AEB remains authoritative.

Status: backlog. Observe-only validation must precede alerts.

## Feature 7 — Vision Blind-Spot Observer

Run the exact StarPilot V-ASM model against configurable driver-camera window regions and publish diagnostic observation state only.

Status: experimental in `2026.007.020`. The observer uses StarPilot's OpenCV 4.11 runtime, 2 Hz base/5 Hz follow-up schedule, newest-frame-only processing, one-thread inference, CPU affinity, and filtered load throttling. RivianPilot additionally enters a timed cooldown after repeated inference over 250 ms or any inference over 500 ms. It has no alert, lane-change, steering, MADS, or vehicle-control integration. Stable `2026.007.019` remains the rollback release.

`2026.007.024` replaces copied CPU-affinity assumptions with a fail-silent resource design. The model loads and performs one synthetic warm-up before readiness; if power arrives directly on-road, fallback loading waits 20 seconds, requires the vehicle to remain stopped, and requires healthy driving services plus CPU headroom. Inference is blinker-only, runs at idle scheduling priority where supported, and is skipped whenever critical services or resources are unhealthy. Serious latency, sustained CPU pressure, or an unexpected loop failure suppresses Vision-BSM for the remainder of that drive. A one-time `BSM Ready ! Ready to drive` alert requires a live readiness heartbeat, successful model warm-up, and valid polygons. CPU topology, readiness, inference timing, resource skips, and suppression are logged. The feature remains observation-only with no lane-change or vehicle-control authority. Stable `2026.007.019` remains untouched.

`2026.007.030` packages single-threaded OpenCV-DNN plus its NumPy dependency and removes ONNX Runtime. Device-side model warm-up succeeds, but measured inference remains about 166 ms median and 209 ms p95. The later user test produced no `.030` Vision-BSM daemon events and left readiness/model-loaded flags false, indicating the observer process never started rather than a detector classification failure. Vision-BSM remains disabled and on hold; it is not part of lane-position control.

`2026.007.019` removes the manual-turn recorder, its stored-package controls, and the location-specific path-replay concept. It also removes the experimental Vision-BSM daemon, model, bundled OpenCV runtime, lane-change guard, alerts, and Sunnylink settings after on-device testing showed unacceptable resource contention. Native lane-change and blind-spot behavior is restored. The complete Vision-BSM experiment remains archived in branch `archive/vision-bsm-experiment-2026.007.018` for offline research.

## Removed or retired

- Lane-hugging observer
- Navigation/turn assistance
- Current-lane boundary guard
- Custom Curve Confidence Indicator
- Redundant Lane Position Preference backlog entry
- Steering Smoothness Slider
- Startup Welcome/Welcome Name
- Manual-turn recording and location-specific path replay
- Vision-BSM lane-change guard and driving integration

## Diagnostics policy

- Diagnostics use the existing rotated vehicle logging system and never create an unbounded standalone log.
- Optional diagnostics are controlled by `RivianPilotFeatureLogging` and default to off.
- Missing or malformed inputs are omitted or marked unavailable.
- Diagnostic extraction, serialization, and write failures must never alter vehicle control.
- Location is recorded only by features with an explicit location-recording control.
