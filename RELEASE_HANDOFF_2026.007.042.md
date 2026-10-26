# RivianPilot 2026.007.042 — source candidate and connection handoff

Prepared October 25, 2026. Source is ready for native build and controlled validation; this is **not a compiled or vehicle-validated release**. No remote push, build workflow, installation, reboot, or device parameter change was performed.

## Exact checkout

- Directory: `/Users/anjanpatel/Documents/Codex/2026-10-25/new-chat-2/RivianPilot-release`
- Branch: `release/2026.007.042`
- Base: `9f21a72bb10d6c2456626318d9683401203daa5c` (published `.041` source).
- Obtain the candidate SHA with `git rev-parse HEAD` in this directory.
- GitHub repository: `https://github.com/n9bpgy8vmy-hue/AdventurePilot.git`.
- This worktree shares Git metadata with the isolated `RivianPilot-review` clone. Its `origin` points to the original **local** checkout, not GitHub. Do not run a plain `git push origin` expecting GitHub.
- The original source checkout and its diagnostics remain untouched.

BSM is explicitly paused at the user's request. This candidate uses the `.041` BSM source unchanged. Do not include the later `afb390203` BSM commit or the uncommitted BSM experiments in `RivianPilot-review`. Those remain separately preserved and are not part of this branch.

## Included behavior changes

1. **Slowing and stopping:** adaptive centering no longer decreases its correction limit to zero below the former minimum speed. Below the configured hold speed, straight-road positioning holds its accepted centering target instead of following changing model-center estimates. This applies with adaptive centering on or off. The default hold threshold is 15 in the selected speed units, preserving the saved parameter value. The settings label is now **Hold Centering Below**. Highway reduction remains active above Full Centering Speed.
2. **Safety boundaries remain authoritative:** holding does not bypass boundary limits, persistent geometry loss, forks, driver steering, disengagement, gear changes, or the modeld heartbeat watchdog. Geometry loss can still return the offset to zero near a stop. Curve priority remains unchanged. This patch is not a guarantee that every reported physical jerk is resolved.
3. **Normal straight-road adjustments:** ramp-down uses the same gradual rate as ramp-up for approved center/preference targets. Safety-driven releases retain the existing faster behavior.
4. **On-screen selection:** on Comma 4's Mici road screen, tap `Lane: ...`, then choose **Left / Default / Center / Right**. The row closes after selection or six seconds of inactivity. Left and Right use the existing bias magnitude; Center uses measured centering; Default removes the persistent lane-position preference while leaving separately enabled curve/nudge features alone. The selected preference is persisted through the same parameter used by Sunnylink and refreshed from it every second.
5. **UI interaction:** the selector owns its full gesture, cancels drags, and does not trigger the home screen or bookmark. It appears only for Rivian with Lane Position Go Live already enabled. Alerts (except routine offset/nudge status), turn signals, and the temporary cruise-speed display hide it and cancel selection. Parameter writes are asynchronous; the UI shows Saving until read-back confirms the value, and reports a timeout/failure instead of claiming it saved. It does not enable Go Live or change BSM.
6. **Center reversal:** the full update loop can now ramp through zero and accept a persistent opposite centering error, instead of holding the previous target forever. Changing preference, driver override, and persistent geometry loss clear the stale held center target.
7. **Held nudges:** the original nudge direction survives a multi-cycle steering press. Same-side presses increment the nudge; opposite presses neutralize it before a subsequent gesture can reverse it.
8. **Lane-change handoff:** modeld consumes the controller's existing fade rather than immediately zeroing the transform solely because a blinker is on. Immediate cancellation for driver steering, non-drive gear, lateral disengagement, and invalid/stale producer data remains.
9. **Diagnostic volume:** resetting post-turn control state no longer resets its diagnostic sampling clock. A 100 Hz reset/update sequence now respects the two-per-second sampling limit.

## Evidence from the copied logs

The capture contains 2,500 system-log files (about 518 MB), but no route qlogs. `device-info.txt` reports installed prebuilt commit `12a83c2896ac1d200562b1dcd42d6fed25d88fbb`, built from `.041` source `9f21a72bb`.

There are 19,616 `.041` lane-position sample records. Of 472 samples below 7 m/s with a non-default preference, 167 reported `geometry_wait`, 155 `approved`, 129 `inactive`, 14 `additional_nudge_limited`, and 7 `ramp_in`. For example, the October 24 15:42 UTC stopping sequence has Left configured, adaptive center limit zero, lane confidence insufficient, and published offset zero. These are diagnostic samples, not counts of distinct stops or proof of physical lateral movement.

The logs also contain connectivity/cache warnings, invalid-system-time messages, 128 `commIssue` records, and 38 UI frame-rate warnings. Some timestamps predate the source build and are unreliable. Raw communication diagnostics include optional ignored subscriptions as well as other invalid/stale services; counts alone do not establish a new root cause or justify changing communication safety checks. Those checks are unchanged. BSM fault investigation is paused. Route qlogs/rlogs and time-aligned vehicle data are still needed for causal analysis of remaining driving symptoms.

## Validation completed on the Mac

- **167 focused tests passed**, including RivianPilot, Rivian MADS resume, camera offsets, existing BSM process predicates, and the new selector tests. Existing BSM tests were run as regression checks only; no BSM behavior was edited.
- New complete-loop tests cover deceleration, stop/hold/restart, both bias directions, Center, adaptive on/off, model-center noise at a stop, geometry-loss release, preference changes at a stop, center reversal, multi-cycle nudge gestures, and controller-to-modeld handoff.
- Tests also cover exclusive selector taps, drag cancellation, alerts/signals, cruise-display priority, parameter mapping, remote refresh, delayed/failed saves, and button bounds.
- Five new behavior regressions were separately run against the original `.041` class: left/right stopping with adaptive centering, center reversal, held opposite nudge, and held same-side nudge. All reproduced the original failures and pass with this candidate.
- Ruff passed for all 10 changed/new Python files; syntax parsing and `git diff --check` passed.
- Sunnylink JSON was regenerated from YAML, matches a fresh compile, and passes all 10 structural validation checks.
- UI imports succeed with `SCALE=1`. The software layout preview is `docs/rivianpilot/releases/2026.007.042-selector.png`; it uses the actual widget's drawing calls with a substitute system font. It is **not** a native GPU render or a device screenshot.

Host tests use the existing Python environment/native Params extension from the original checkout; repository-wide conftest and configured extra pytest options are disabled. This is not the full device CI suite. Native GUI rendering could not be verified in this session. This checkout still has LFS pointer assets, and Git LFS is not installed on this Mac shell. Do not rsync this raw source tree over the running device as a prebuilt installation.

## Next session: connect, build, and publish

The user authorized preparation of this version and plans to use another model/session for connection. Their intended workflow includes source push, native build, and deployment. Start with read-only checks and preserve their existing work.

1. Open this exact release worktree, check `git status --short`, `git branch --show-current`, and `git rev-parse HEAD`. Read this handoff. Keep BSM excluded. Verify the current GitHub branch state before pushing; do not overwrite another session's commits or force-push.
2. SSH endpoint: `comma@192.168.1.64`; key stays on the Mac at `/Users/anjanpatel/.ssh/id_ed25519_comma4`. A minimal read-only check is:

   ```sh
   ssh -i /Users/anjanpatel/.ssh/id_ed25519_comma4 -o BatchMode=yes -o StrictHostKeyChecking=yes -o ConnectTimeout=8 comma@192.168.1.64 'hostname; uptime; git -C /data/openpilot log -1 --oneline'
   ```

   This session repeatedly received `Operation not permitted` before authentication, while the user's Terminal connected and copied the system logs. Do not rotate keys or change device SSH configuration to solve that host-session restriction.
3. Once connected, confirm the vehicle is parked/offroad; record installed source/prebuilt SHA, device status, and current lane-position settings. Preserve the current runnable prebuilt for rollback. Inventory `/data/media/0/realdata` and determine why the qlog copy produced an empty folder before attempting another transfer. Do not assume all 2,000 miles remain on-device.
4. Push the candidate branch to the **GitHub URL**, not the local `origin`, after confirming authentication and the target:

   ```sh
   git push https://github.com/n9bpgy8vmy-hue/AdventurePilot.git HEAD:refs/heads/release/2026.007.042
   ```

   This branch is not included in the workflow's push triggers. Native build requires an explicit dispatch of `.github/workflows/build-prebuilt.yaml` with `source_branch=release/2026.007.042` once its prerequisites are verified. The existing workflow publishes `release/2026.007.042-prebuilt` for that source name.
5. **Verify runner mount isolation before dispatching the existing build workflow.** Its setup and cleanup delete the contents of `BUILD_DIR=/data/openpilot`. The documented runner design uses a private mount namespace binding `/data/github/openpilot` there. Prove that isolation in the actual runner/job namespace before relying on it; an ordinary SSH mount check outside that namespace is insufficient. If isolation cannot be verified, stop that build path. Do not change or remove `.overlay_consistent` or other overlay markers. Do not run the old local build/install scripts blindly.
6. The native build must fetch Git LFS assets/submodules, use the locked device dependencies, compile modeld and the rest of the release, verify the packaged selector is included, and tie the prebuilt metadata to the exact candidate source SHA. The workflow already expresses the native SCons stages; do not substitute a host pytest pass for that build.
7. Install only the verified prebuilt through the established parked-device workflow. Keep BSM paused. Verify cold start, manager/service health, touch selection and alert visibility, persistence after restart, and all four preference mappings. Then perform controlled validation of stop/restart, curve transitions, lane-change fade, and both nudge directions. Collect route logs and compare commanded offsets with consumed transforms and measured steering response. Roll back on new control errors or worse steering behavior.

## Remaining work outside this candidate

BSM fixes, detector tuning, changing the high-frequency Params transport, full input-freshness contracts, telemetry of the actually consumed camera transform, and broad controller refactoring are deferred. They require separate validation and are not silently included in this stopping/UI release. No evidence-based tuning of steering gains, braking, or safety thresholds was made from these incomplete logs.
