---
name: mycelium-minimal
description: >-
  Mycelium least-code discipline — write only what the task needs and never
  more. Climb the laziness ladder (YAGNI → reuse existing → stdlib → native
  platform feature → installed dep → one line → minimum) before writing any
  code, while never cutting validation, data-loss handling, security, or error
  surfacing. Trigger whenever the user is adding a feature, a node, a store
  field, a hook, a component, or any new code in the Mycelium repo, or says
  "keep it small", "don't over-engineer", "simplest way", "least code", "do I
  even need this", "reduce the code", or reviews a diff for
  over-building. Pairs with mycelium-simplify (simplify code that already
  exists) and mycelium-coding (general editing discipline).
version: 0.1.0
---

# Mycelium — Minimal Code

The always-on laziness ladder lives in `AGENTS.md`; this skill is the
Mycelium-specific application of it. Self-contained — the core move is below.

## Core move

Before writing code, climb the ladder and stop at the first rung that holds:

```
1. Need exist?            → no: skip (YAGNI)
2. Already in repo?       → reuse, don't rebuild
3. Stdlib does it?        → use it
4. Native platform?       → use it
5. Installed dep does it? → use it
6. One line?              → one line
7. Only then              → minimum that works
```

Lazy about the solution, never about reading the code first. Validation,
data-loss handling, security, and error surfacing are never cut.

## Examples

- Frame cache needed → reuse `ReadNode._FRAME_CACHE`, not a new class.
- Bounded cache → `collections.OrderedDict`, not a hand-rolled 30-line LRU.
- Frame field → `<input type="number">`, not a stepper dependency.
- New node param with no caller yet → don't add it; add when first used.

## Hand-offs

- Code already exists and is bloated → `mycelium-simplify`.
- File over the line cap → `mycelium-file-guard`.
- Pass/fail gate before commit → `mycelium-check`.
