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

## Removed or retired

- Lane-hugging observer
- Navigation/turn assistance
- Blind-spot observer and current-lane boundary guard
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
