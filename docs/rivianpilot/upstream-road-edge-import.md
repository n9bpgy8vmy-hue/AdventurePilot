# Sunnypilot road-edge backport

Integration branch: integration/sunnypilot-road-edge. Local source change only; not built or deployed.

Sources:
- https://github.com/sunnypilot/sunnypilot/commit/2d859a8ca610bc20f48b0b3e4f7d570a80b59c29 — road-edge controller, settings, model and event wiring.
- https://github.com/sunnypilot/sunnypilot/commit/a06366ce9246962f313380e4ebe15955e720e7a5 — separate BSM/edge handling, nudge override and post-edge timer delay.

The RELC and automatic-lane-change controller match the second pinned revision, with only the cereal import namespace adapted to this older repository. Model wiring is adapted to both .042 model pipelines. The existing maneuver state machine is retained; unrelated upstream lane-change state simplification is not imported.

Removed the custom geometry detector, latch, observation/enforcement modes, status fields and custom alert. Standard RoadEdgeLaneChangeEnabled is available in Sunnylink and the device lane-change settings, default Off. Upstream behavior is preserved: the edge flag blocks automatic initiation, torque nudge can override the edge block, and available factory BSM still blocks. No solid-line or side-vehicle detector is added. No extra inference model is required.

Schema field numbers @1 and @2 are now upstream's edge flags; this replaces the never-deployed custom prototype schema. Do not mix prototype binaries/messages with this build. The .042 event enum already uses upstream's @24, so the new event uses next free @41.

Lane-position curve continuity and timed-nudge handback changes remain separate. Stopping behavior is unchanged. Previously prepared Sunnylink simplification/zero-delay commits in the unavailable Linux checkout still require integration before a combined release.

Validation: 169 focused tests passed (upstream RELC/auto-lane-change tests, six end-to-end edge/nudge/BSM cases, and lane-position regressions). Params storage was substituted for the unavailable native params_pyx module; actual controller code and cereal schemas were used. Upstream test infrastructure was adapted to this older pytest/parameterized setup. 17 settings compiler tests passed. Generated settings match source, touched Python parses, controller source equivalence checks and git diff --check pass. Full native build, device UI and road validation remain pending.
