# RivianPilot Feature Roadmap

This file is the canonical definition and status of RivianPilot features. Do not broaden a feature's behavior without explicit approval.

## Feature 1 — Reverse/MADS

Prevent unwanted lane centering while parking. After returning to Drive, MADS can resume above the configured speed following a configurable 1–5 second warning. Already-active MADS stays active if speed later falls below that threshold.

Status: implemented and vehicle-validated. Additional branding, countdown clarity, and bounded diagnostics are under development.

## Feature 2 — Lane-hugging observer

Record locations where the driver corrects lane position and warn on future approaches.

Phase 1 scope is observation, bounded logging, and driver alerts only. It must not change steering commands, lateral control, MADS state, or engagement behavior. Rich curve snapshots are limited to one every two seconds and record lane/path geometry, lane confidence, actual and desired curvature, driver/EPS/requested/applied steering torque, steering angle/rate, controller saturation, and tracking errors. A diagnostics failure disables only rich logging; location observation and alerts continue.

Status: Phase 1 implementation in development; not yet vehicle-validated.

## Feature 3 — Navigation/turn assistance

Google Maps sharing, exits, lane selection, and turns.

Status: on hold.

## Feature 4 — Post-turn resume

Temporarily pause or soften MADS for a manual turn, then warn and resume after detecting a stable new lane and meeting configurable conditions.

Safety boundaries:

- Rivian-only with separate Observe and Go Live toggles, both disabled by default.
- Observe records the full decision lifecycle without changing steering or MADS. Go Live requires Observe and activates control changes.
- Arms only while MADS lateral control is active, exactly one turn signal is on, Drive is selected, and speed is at or below the configured turn threshold.
- Keeps lateral control paused until the signal is off, driver steering is released, steering angle and yaw have settled, both lane boundaries remain confident for the configured stable-lane period, and minimum resume speed is met.
- Warns for the configured 1–5 seconds before resuming.
- Treats closely spaced same- or opposite-direction turns as one continuous manual maneuver. Each new blinker edge keeps MADS paused and restarts final-lane stability evaluation from the newest turn.
- Cancels immediately when leaving Drive so Feature 1 remains authoritative for Reverse behavior.
- Does not alter ordinary high-speed behavior unless a qualifying low-speed turn already armed the pause.

Status: implementation in development; not yet built or vehicle-validated.

## Feature 5 — Current-lane boundary guard

Make automatic lane changes more conservative on undivided and single-lane roads by requiring high confidence that a same-direction adjacent travel lane exists. Uncertainty must block RivianPilot's automatic lane-change initiation and notify the driver, without restricting manual steering.

Planned phases:

0. Observe and validate Rivian left/right blind-spot signals, driver torque/nudge recognition, and the native lane-change blocked outcome. No alerts or control changes.
1. Observe and log proposed lane changes, lane-line/road-edge confidence, adjacent-lane evidence, and the eventual driver action. No alerts or control changes.
2. Add warning-only behavior for questionable lane-change requests after replay and vehicle-data validation.
3. Consider blocking automatic lane-change initiation only if collected evidence demonstrates sufficiently reliable detection.

Safety boundaries:

- The feature must never initiate a lane change or apply corrective steering.
- It must never prevent or resist the driver's manual steering input.
- It must not claim reliable yellow/white, solid/dashed, legality, or oncoming-lane classification unless a validated model output explicitly provides it.
- Offline map data may support a conservative block decision but must never authorize crossing a boundary.
- Weak, missing, stale, or conflicting evidence must disable automatic lane-change initiation rather than guess.
- Observe-only and warning-only phases must complete before any control-affecting phase is considered.

Status: Phase 0 blind-spot observer implemented for bounded diagnostics; not yet vehicle-validated. Remaining phases are roadmap only and are not approved for control-affecting use.

## Feature 6 — Manual-turn learning recorder

Record repeatable driver-executed turns when a single blinker is active so future development can compare the human trajectory with the model-predicted path.

Planned observation data:

- GPS location, approach heading, and turn direction.
- Relative trajectory from live pose before, during, and after the turn.
- Speed, acceleration, yaw rate, curvature, steering angle/rate, and driver torque.
- Requested/applied steering output, model path, lane confidence, and post-turn lane stability.
- Repetition count and trajectory variation for the same location, approach, and direction.

Safety boundaries:

- Observation only; it must not change steering, MADS, speed, or engagement.
- Never replay a raw GPS or steering trajectory.
- A recorded location must not imply turn intent; navigation intent and strong localization would be required for any future assistance.
- Use bounded, rotated diagnostics and group repeated observations instead of retaining unlimited raw samples.
- Store each qualifying turn as a separate, bounded package with individual, oldest, and all-package removal controls.
- Treat the package confidence score only as recording-quality/completeness evidence, never as an automation-safety score.
- Any warning or control-affecting phase requires separate approval, replay analysis, and vehicle validation.

Status: Phase 1 observation-only package recorder in development; not yet vehicle-validated. Assistance and vehicle-control phases remain on hold and are not approved.

## Diagnostics policy

- Every implemented RivianPilot feature must include bounded, transition-based diagnostics for effectiveness analysis.
- Optional feature diagnostics are controlled by the Sunnylink `RivianPilotFeatureLogging` toggle and default to off.
- Errors and safety anomalies remain visible through the platform's normal crash/error handling regardless of the optional diagnostics toggle.
- Do not create an unbounded standalone log. Use the existing rotated vehicle logging system.
