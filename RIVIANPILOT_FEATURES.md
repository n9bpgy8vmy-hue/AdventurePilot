# RivianPilot Feature Roadmap

This file is the canonical definition and status of RivianPilot features. Do not broaden a feature's behavior without explicit approval.

## Feature 1 — Reverse/MADS

Prevent unwanted lane centering while parking. After returning to Drive, MADS can resume above the configured speed following a configurable 1–5 second warning. Already-active MADS stays active if speed later falls below that threshold.

Status: implemented, vehicle-tested, and validated as working exceptionally well. Reverse parking no longer causes unwanted lane centering, Drive resume behavior works as intended, and no regression to normal driving behavior has been reported.

## Feature 2 — Lane-hugging observer

Status: retired. Its observer, runtime integration, stored-parameter definitions, alerts, tests, and Sunnylink controls were removed. It no longer records correction locations or produces approach alerts.

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

Status: current strict implementation is built, vehicle-tested, and validated as working great. Guarded pause, warning, stable-lane resume, and consecutive-turn handling work successfully at the current code level. A new opt-in relaxed mode is under development for curb-only roads; Strict remains the default and preserves the validated behavior.

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

### Road-test observations pending validation

1. **Feature 4 on curb-only roads:** A configurable relaxed mode is being added for drivers who want post-turn resume on curb-only roads. Strict mode remains the default and continues requiring both painted lane boundaries. Relaxed mode requires two confident road edges, plausible road width, a safely contained model path, settled vehicle motion, and sustained evidence; it is not allowed as a high-speed escape.
2. **Feature 6 with a short blinker tap:** A momentary blinker may not produce a saved package. It can fall between 5 Hz samples or expire three seconds before manual steering and meaningful heading change occur, causing the candidate package to be discarded.

Potential future work, on hold until more road-test data is available:

- Road-test the opt-in curb-road recovery path for Feature 4. Painted lane lines remain preferred; GPS or a single curb never authorizes resume.
- Introduce a shared turn-intent latch for Features 4 and 6. A short or continuous single-blinker event would latch direction for a bounded 5–10 second window, manual steering or meaningful heading change would confirm the turn, an opposite blinker would replace direction, leaving Drive would cancel, and an unconfirmed candidate would expire without action or storage.
- Keep all changes observation-only until recorded curb-only and short-blinker scenarios demonstrate reliable behavior.

## Backlog

### Feature 7 — Calibration Health ⭐⭐⭐⭐⭐

Provide a clear view of camera/device calibration quality, stability, drift, mount-angle consistency, and conditions that may reduce lateral-control confidence.

Status: backlog; scope and safety requirements not yet defined.

### Feature 8 — Curve Confidence Indicator ⭐⭐⭐⭐⭐

Display how confident RivianPilot is in the predicted curve, lane/path geometry, road edges, and lateral-control tracking before and during a curve.

User priority: favorite backlog capability.

Approved behavior:

- Visual-only feature with a Sunnylink enable/disable toggle; no separate observation-mode requirement.
- Normal confidence makes no UI change.
- Low confidence adds a translucent yellow edge/background wash and a bottom `CURVE CONFIDENCE LOW` message.
- Critical confidence adds a translucent red edge/background wash and a bottom `CURVE ASSISTANCE UNCERTAIN — BE READY` message.
- The camera view, predicted path, lane boundaries, vehicles, speed, and native alerts remain visible above the overlay.
- Hysteresis and sustained thresholds prevent rapid color changes. The overlay clears after the road is straight and confidence remains recovered for a bounded period.
- The score combines lane lines, road edges, predicted-path stability, requested-versus-actual curvature, steering tracking error, controller saturation/reserve, driver correction, and calibration health.

Failure isolation:

- This feature is read-only and must never write MADS state, engagement state, steering commands, controller parameters, or vehicle-control messages.
- Missing, malformed, stale, non-finite, or out-of-range inputs suppress the indicator instead of generating a warning from unreliable data.
- Scoring and rendering use separate exception boundaries. A scoring failure disables only Feature 8 for the current drive; a rendering failure skips only its overlay frame.
- Emit one bounded error transition through existing rotated logs, with repeated failures rate-limited. Never create an unbounded standalone log.
- The base on-road renderer must execute independently so Feature 8 cannot blank, freeze, or replace key UI capabilities.

Status: approved for direct visual implementation with the above isolation requirements. It must not imply that a curve is guaranteed safe or authorize any control change.

### Feature 9 — Lane Position Preference ⭐⭐⭐⭐⭐

Allow a carefully bounded preference for lane placement, such as a small center offset or additional distance from the inside of a curve, while respecting detected lane boundaries, road edges, vehicle width, and confidence limits.

Implemented with separate Observe and Go Live controls. Curve strength is derived from predicted lateral acceleration, and a deliberate driver nudge can request a temporary preference. The requested value is capped by confident painted lane boundaries, plausible lane width, Rivian width, and a fixed clearance margin. Driver steering, blinkers, lane changes, non-Drive state, inactive lateral control, stale updates, malformed inputs, or feature errors remove the temporary contribution. Go Live defaults off pending controlled vehicle validation of direction and physical response.

### Feature 10 — Steering Smoothness Slider ⭐⭐⭐⭐

Offer a bounded driver preference for steering response smoothness without weakening safety limits, curve tracking, driver override, or controller stability.

Status: backlog; requires controller-specific design and validation. It must not directly expose unsafe raw tuning values.

## Diagnostics policy

- Every implemented RivianPilot feature must include bounded, transition-based diagnostics for effectiveness analysis.
- Optional feature diagnostics are controlled by the Sunnylink `RivianPilotFeatureLogging` toggle and default to off.
- Errors and safety anomalies remain visible through the platform's normal crash/error handling regardless of the optional diagnostics toggle.
- Do not create an unbounded standalone log. Use the existing rotated vehicle logging system.
