# RivianPilot 2026.007.044 candidate

Based on .043 (b0e95114c215f925635f64a8fea51680ca328db7). Source import only: no native build, installation, or road validation is asserted here. The .043 source and its manual build script remain pinned and unchanged.

## Imported changes

| Upstream commit | Change | Integration |
| --- | --- | --- |
| 53e13a7bc0c38f53b1f11a50cfa346d4f51c39c9 (#1906) | Correct inverted learned/fixed steering-delay selection | Published live delay when learning is on; cached actuator plus software delay when off. Existing user setting is preserved. |
| 347238b307027d7c0d1c10d51a38a7f6ad885649 (#2016) | Camera offset uses calibrated horizon | Same upstream formula, adapted to this fork's liveCalibration, roadCameraState and fcam/ecam names. Rivian dynamic-offset watchdog, handback and smoothing are retained. |
| 63a2a3868e0ed194ec5fedd628f711eab0411423 (#2026) | Avoid blocking model-settings read | Request the registered default for missing LagdToggleDelay rather than accidentally passing a truthy blocking argument. |
| 27c132984df60e9ef42ebd1a805738b8c851356f (#1949) | Clear duplicate MADS no-entry alerts | Upstream alert-clearing condition. No brake/resume-mode change. |
| 91a53aa1610891070cf2877c84f00c2c2965ec31 (#693) | Optional lateral jerk torque controller | Default off; upstream offroad UI and NNLC exclusion, angle-control cleanup, Sunnylink controls and controller inheritance. |

## Controller integration correction

The imported controller initially failed a Rivian regression test: a deliberately large curvature request on its first update returned a normalized torque magnitude of about 2.8 instead of staying within 1.0. This is a software-output test, not an observed vehicle command. The extension receives the parent's acceleration-space PID on each update, so relying on a prior live-tuning limit update is insufficient.

The jerk-aware update now applies its upstream torque-limit method to that PID before computing output, only when the feature is enabled. Tests cover both steering directions, the existing v0 and v1 torque controllers, first engagement and subsequent live-tuning changes. This is the one functional adaptation to the imported jerk controller; its equations are otherwise unchanged. Disabled behavior retains the existing controller path.

## Validation and limits

- 294 host tests passed: existing RELC, lane-change integration, lane positioning, stop/turn/reverse resume, plus delay selection, calibrated camera offset, upstream controller initialization/update, Rivian torque-limit regressions and settings schema tests.
- 17 settings compiler tests passed; generated settings match YAML; all 10 settings validator checks passed.
- Host tests substitute in-memory parameter storage because this checkout lacks its native Params extension. They use controller and schema code from this checkout; this does not replace the native Comma build.
- The build workflow runs the newly added delay and torque-controller tests before packaging.
- No claim of improved road handling yet. The camera and delay fixes can change steering behavior even with the new controller disabled. Keep a backup of the current installation and parameters before any later deployment.
- No new side-vehicle detection, solid-line classification, custom lane-change guard, or relaxed driver-monitoring policy is introduced.

Native build and replay/device checks remain prerequisites to deployment. Do not reuse the .043 terminal script for this release: it intentionally verifies the .043 source SHA.
