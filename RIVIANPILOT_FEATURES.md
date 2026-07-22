# RivianPilot Feature Roadmap

This file is the canonical definition and status of RivianPilot features. Do not broaden a feature's behavior without explicit approval.

## Feature 1 — Reverse/MADS

Prevent unwanted lane centering while parking. After returning to Drive, MADS can resume above the configured speed following a configurable 1–5 second warning. Already-active MADS stays active if speed later falls below that threshold.

Status: implemented and vehicle-validated. Additional branding, countdown clarity, and bounded diagnostics are under development.

## Feature 2 — Lane-hugging observer

Record locations where the driver corrects lane position and warn on future approaches.

Phase 1 scope is observation, bounded logging, and driver alerts only. It must not change steering commands, lateral control, MADS state, or engagement behavior.

Status: approved definition; not yet implemented.

## Feature 3 — Navigation/turn assistance

Google Maps sharing, exits, lane selection, and turns.

Status: on hold.

## Possible Feature 4 — Post-turn resume

Temporarily pause or soften MADS for a manual turn, then warn and resume after detecting a stable new lane and meeting configurable conditions.

Status: concept only. Not implemented and not approved.

## Diagnostics policy

- Every implemented RivianPilot feature must include bounded, transition-based diagnostics for effectiveness analysis.
- Optional feature diagnostics are controlled by the Sunnylink `RivianPilotFeatureLogging` toggle and default to off.
- Errors and safety anomalies remain visible through the platform's normal crash/error handling regardless of the optional diagnostics toggle.
- Do not create an unbounded standalone log. Use the existing rotated vehicle logging system.
