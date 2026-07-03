---
name: mycelium-debug
description: >-
  Systematic, root-cause-first debugging for the Mycelium monorepo. Use when
  the user hits a bug, a test failure, a crash, a wrong pixel/result, a hung
  bake, a node that won't register, an EXR/colorspace mismatch, a viewer that
  renders wrong, or any "why is this happening / it's broken / unexpected
  behavior" situation. Investigate the root cause and gather evidence at the
  real component boundaries (engine → node graph → execute → viewer; API →
  service; bake → serve_frame) BEFORE proposing a fix; reproduce with a
  failing test first; change one thing at a time; after 3 failed fixes,
  stop and question the architecture. Pairs with mycelium-check to verify.
---

# Mycelium Debug

Find the root cause before touching anything. A symptom fix that doesn't
understand the cause is how one bug becomes three. Naming, verification, and
the laziness ladder for the eventual fix all live in `AGENTS.md`.

## The one law

**No fix before the root cause is understood.** If you're proposing a change
and you can't say _why_ the bad value exists, you're guessing — stop and
investigate.

## Phase 1 — Investigate (always first)

1. **Read the error completely.** Full stack trace, line numbers, file paths.
   The answer is often already in the message.
2. **Reproduce reliably.** Exact steps; does it happen every time? If you
   can't reproduce it, gather more data — don't guess.
3. **Check what changed.** `git diff`, recent commits, a new dep, a config or
   workflow YAML edit, a different EXR/colorspace input.
4. **Trace the data backward to its source.** Where does the bad value first
   appear? What called this with it? Keep walking up until you reach the
   origin, then fix it there — not at the symptom.

## Phase 2 — Gather evidence at the real boundaries

Mycelium is multi-component. Don't theorise across a boundary — instrument it
and look. Add temporary logging at each layer the data crosses, run once, read
where it breaks:

- **Engine / graph:** is the node graph wired as expected? `resolve` order,
  upstream outputs, `DataType` of each socket.
- **Node `execute`:** log the shape/dtype/range of tensors in and out. A wrong
  pixel is usually a wrong input, not wrong math.
- **Bake protocol:** `IS_LONG_RUNNING` nodes — is `bake` producing what
  `serve_frame` reads back? A hang or stale frame lives at that seam.
- **EXR / colorspace:** log the working space, the input transform, premult
  state. "Looks too dark/bright" is almost always a transform applied twice or
  not at all.
- **API → service:** what payload enters the endpoint vs. what the service
  receives — log both sides of the call.

Evidence tells you _which_ component fails. Only then investigate that one.

## Phase 3 — One hypothesis, tested minimally

State it out loud: "I think X is the cause because Y." Make the **smallest**
change that tests it — one variable at a time. Worked? Go to Phase 4. Didn't?
Form a _new_ hypothesis; do not stack more changes on top. If you don't
understand something, say so and dig — don't pretend.

## Phase 4 — Fix at the root, with a failing test

1. **Reproduce it as a failing check first.** A `pytest` case under the owning
   library (e.g. `libraries/myc-nodes/tests/...`) or, if no framework fits,
   the one runnable assert-based check `AGENTS.md` already requires. Watch
   it fail for the right reason before you fix.
2. **One fix, at the source.** Address the cause you proved. No bundled
   refactor, no "while I'm here."
3. **Verify per `AGENTS.md`:** the failing test now passes, nothing else
   broke. Run it — never claim fixed without the green output. Hand the full
   diff to `mycelium-check` for the lint/typecheck/test battery.

## After 3 failed fixes — stop, question the architecture

If each fix exposes a new problem somewhere else, or fixes need "massive
refactoring," that's not a bad hypothesis — it's a wrong structure. Stop
patching. Name the coupling and discuss the design with the user before
attempting fix #4.

## Supporting techniques

- **Root-cause tracing.** When the error surfaces deep in the call stack,
  don't patch where it throws. Walk backward — who passed the bad value, and
  who passed it to _them_ — until you reach the origin, and fix it there. A
  guard at the symptom leaves every other caller still broken.
- **Defense in depth (after the root cause).** Once fixed at the source, add a
  cheap assertion or validation at the boundaries that should never have let
  the bad value through (node `execute` input checks, socket `DataType`
  guards, API payload validation). The root-cause fix stops this bug; the
  guards stop the next one and make the next failure loud and local.
- **Condition-based waiting.** Never paper over a race with a fixed
  `sleep`/`setTimeout`. Poll the actual condition (bake complete, frame in
  cache, websocket ready) with a timeout. Arbitrary delays are flaky under
  load and on different hardware — a hung bake or stale frame is usually a
  wait on time instead of on state.

## Red flags (you're guessing — return to Phase 1)

- "Quick fix now, understand it later."
- "Just try changing X and see."
- Multiple changes at once "to save time."
- Proposing a fix before tracing the data flow.
- "One more attempt" after two failures.

## Output

```
ROOT CAUSE  <component> — <the actual cause, traced to source>
EVIDENCE    <what the boundary logs showed>
FIX         <one change at the source> + failing test that now passes
VERIFY      make test / scoped pytest → result (run it)
```
