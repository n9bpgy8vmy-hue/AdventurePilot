# RivianPilot 2026.007.043

Changes: upstream Sunnypilot road-edge lane-change controller (standard On/Off, default Off; steering nudge overrides edge blocking), retained .042 stopping behavior, smoother curve entry and timed-nudge handback, rejection of invalid lane confidence, grouped Sunnylink lane-position/nudge/curve/turn-resume settings, clearer low-speed pause switches, and zero additional warning delay after Reverse. Existing Drive/speed/turn-stability gates remain. No solid-line detector or new side-vehicle detection. No custom lane-change guard or Observe/Enforce modes.

The earlier settings edits were reconstructed in the Mac checkout because the separate Linux commits were unavailable. Settings regrouping preserves all keys without duplicates. The native runner must confirm the new zero-delay path and RELC integration.

Host validation: 235 focused tests passed with parameter storage substituted, including upstream RELC/automatic lane-change, integration cases, lane-position, reverse resume and post-turn resume. 17 settings compiler tests passed, schema validation passed, and generated output matches source. These are not device or road validation.

Build workflow additionally checks IsOnroad=0 and verifies /data/openpilot is the runner's /data/github/openpilot bind mount before clearing the build directory. Do not build from the live installation namespace. Native build/install status must be verified separately; this document does not assert deployment.

Deployment target: release/2026.007.043-prebuilt. Preserve the installed .042 tree and current parameters before switching. Fetch and verify the prebuilt's recorded source SHA against this release before installation. Do not copy obsolete driver-monitor timeout modifications from historical backups.
