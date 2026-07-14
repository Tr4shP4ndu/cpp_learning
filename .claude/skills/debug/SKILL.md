---
name: debug
description: >-
  Systematic, root-cause-first debugging for any codebase. Use when you hit a
  bug, a test failure, a crash, wrong output, a hang, or any "why is this
  happening / it's broken / unexpected behavior" situation. Gather evidence at
  the real component boundaries BEFORE proposing a fix; reproduce with a failing
  test first; change one thing at a time; after 3 failed fixes, stop and
  question the architecture.
---

# Debug

Find the root cause before touching anything. A symptom fix that doesn't
understand the cause is how one bug becomes three. For the deeper general
process, `superpowers:systematic-debugging` covers the theory — this skill is
the codebase-flavored companion, not a re-derivation of it.

## The one law

**No fix before the root cause is understood.** If you're proposing a change
and you can't say _why_ the bad value exists, you're guessing — stop and
investigate.

## Phase 1 — Investigate (always first)

1. **Read the error completely.** Full stack trace, line numbers, file paths.
   The answer is often already in the message.
2. **Reproduce reliably.** Exact steps; does it happen every time? If you
   can't reproduce it, gather more data — don't guess.
3. **Check what changed.** `git diff`, recent commits, a new dependency, a
   config edit, a different input.
4. **Trace the data backward to its source.** Where does the bad value first
   appear? What called this with it? Keep walking up until you reach the
   origin, then fix it there — not at the symptom.

## Phase 2 — Gather evidence at the real boundaries

Most systems are multi-component. Don't theorise across a boundary — instrument
it and look. Add temporary logging where data crosses each seam (module → API,
service → store, request → response, producer → consumer), run once, and read
where it breaks. Log the shape/type/range of what enters and leaves each side.

Evidence tells you _which_ component fails. Only then investigate that one.

## Phase 3 — One hypothesis, tested minimally

State it out loud: "I think X is the cause because Y." Make the **smallest**
change that tests it — one variable at a time. Worked? Go to Phase 4. Didn't?
Form a _new_ hypothesis; do not stack more changes on top. If you don't
understand something, say so and dig — don't pretend.

## Phase 4 — Fix at the root, with a failing test

1. **Reproduce it as a failing check first.** Watch it fail for the right
   reason before you fix.
2. **One fix, at the source.** Address the cause you proved. No bundled
   refactor, no "while I'm here."
3. **Verify:** the failing test now passes, nothing else broke. Run it — never
   claim fixed without the green output.

## After 3 failed fixes — stop, question the architecture

If each fix exposes a new problem somewhere else, or fixes need "massive
refactoring," that's not a bad hypothesis — it's a wrong structure. Stop
patching. Name the coupling and discuss the design with the user before
attempting fix #4.

## Supporting techniques

- **Root-cause tracing.** When the error surfaces deep in the call stack, don't
  patch where it throws. Walk backward — who passed the bad value, and who
  passed it to _them_ — until you reach the origin, and fix it there. A guard at
  the symptom leaves every other caller still broken.
- **Defense in depth (after the root cause).** Once fixed at the source, add a
  cheap assertion at the boundary that should never have let the bad value
  through. The root-cause fix stops this bug; the guard stops the next one and
  makes the next failure loud and local.
- **Condition-based waiting.** Never paper over a race with a fixed
  `sleep`/`setTimeout`. Poll the actual condition with a timeout. Arbitrary
  delays are flaky under load and on different hardware.

## Red flags (you're guessing — return to Phase 1)

- "Quick fix now, understand it later."
- "Just try changing X and see."
- Multiple changes at once "to save time."
- Proposing a fix before tracing the data flow.
- "One more attempt" after two failures.

## Output

Root-cause first, then the fix: `ROOT CAUSE <component — cause traced to
source>` / `EVIDENCE <what the boundary logs showed>` / `FIX <one change at the
source> + failing test that now passes` / `VERIFY <command → result, run it>`.
Then ≤3 short lines: what was skipped, when to add it. Follows the `minimal` skill — laziest solution that works, fewest words.

## Project overrides

If this skill has a `projects/<project>.md` matching the repo you're in, read it
too — it names that project's real component boundaries and known failure modes.
