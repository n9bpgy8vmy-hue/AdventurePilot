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

## Feature 4 — Experimental Lane-Change-to-Turn Handover

Create a natural transfer of lateral authority from native Nudge lane change, to a driver-controlled turn, and back to MADS after the new road is stable.

Status: on hold. Feature 2 diagnostics must first establish real Rivian timing and state sequences. Any future implementation must be separate and mutually exclusive with Classic Post-Turn Resume.

## Feature 5 — Manual-Turn Recorder

Store bounded, removable, observation-only turn packages for offline analysis. It must never replay a raw path or change steering, speed, MADS, or engagement.

Status: observation-only implementation retained. Further short-blinker and package-quality improvements are deferred.

## Feature 6 — Calibration Health

Report calibration quality, stability, drift, mount-angle consistency, and conditions that may reduce lateral confidence.

Status: backlog; diagnostic-only first phase.

## Feature 7 — Forward Collision Warning+

Provide a supplemental, model-based visual/audio collision warning without braking or steering commands. Rivian native FCW/AEB remains authoritative.

Status: backlog. Observe-only validation must precede alerts.

## Feature 8 — Vision Blind-Spot Monitor

Use the driver-camera V-ASM detector from StarPilot PR #75 as an optional supplemental lane-change veto. Existing Nudge or blinker-only configuration remains authoritative. A detection can cancel a pending request but never initiates a lane change or sends steering commands.

Phase 1 behavior:

- Off restores the existing lane-change behavior exactly.
- A Nudge made while the corresponding side is detected is consumed; torque must be released and a fresh Nudge supplied after the detection clears.
- A blinker-only request detected as occupied is cancelled; the blinker must be cycled off and on before another request.
- An already-started lane change is not reversed by the detector.
- Visual cancellation is always shown; sound is controlled independently by `RivianPilotVisionBSMLoudAlert`.
- A matching blinker and fresh Vision-BSM detection shows a side-specific screen alert before a lane-change request. Logging records only detection start/clear transitions and duration.
- Missing, stale, overloaded, or failed detector output cannot generate steering and falls back to existing lane-change behavior.

Status: experimental, default off. Based on the hardened OpenCV integration following StarPilot PR #75. Vehicle-camera polygon calibration and parked validation are required before road evaluation.

## Removed or retired

- Lane-hugging observer
- Navigation/turn assistance
- Current-lane boundary guard
- Custom Curve Confidence Indicator
- Redundant Lane Position Preference backlog entry
- Steering Smoothness Slider
- Startup Welcome/Welcome Name

## Diagnostics policy

- Diagnostics use the existing rotated vehicle logging system and never create an unbounded standalone log.
- Optional diagnostics are controlled by `RivianPilotFeatureLogging` and default to off.
- Missing or malformed inputs are omitted or marked unavailable.
- Diagnostic extraction, serialization, and write failures must never alter vehicle control.
- Location is recorded only by features with an explicit location-recording control.
