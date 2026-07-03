---
name: mycelium-simplify
description: >-
  Mycelium-tuned code simplification for changed code. Apply the project's
  own refactor rules (rule-of-three dedup, functions over 50 lines or 3+
  nesting levels, parameter lists over 4, naming that lies, dead/commented
  code, debug prints/console.logs) — not generic advice. Auto-apply only
  deterministic tool fixes; propose every judgment-call refactor behind a
  confirmation gate. Trigger when the user asks to "simplify", "clean up",
  "improve for simplicity", "reduce complexity", "make this simpler",
  "refactor for readability", "tidy this up", or "is there a cleaner way"
  in the Mycelium repo. For splitting oversized files use
  mycelium-file-guard; for pass/fail checks use mycelium-check.
---

# Mycelium Simplify

Simplify changed code against Mycelium's _own_ thresholds, not generic
taste. Editing discipline, verification, and the laziness ladder come from
`AGENTS.md`; larger restructuring hands off to `mycelium-refactor`.

## Scope

Default to the diff (`git diff --name-only` + staged). Honour a path if
the user gives one. Read the relevant ranges before proposing — no
blind rewrites (`AGENTS.md`: read before edit, minimal diffs).

## What to flag (Mycelium thresholds)

- **Rule of three** — same logic in 3+ places → extract. NOT for 2
  occurrences.
- **Function > 50 lines** or **3+ levels of nesting** → consider split.
- **Param list > 4** → options object / split function.
- **Naming lies** — function name says one thing, does another → rename
  or split.
- **Single-letter names** — `e`/`n`/`r`/`s`/`f` in added/changed lines →
  rename to full word (`AGENTS.md`; whitelist `i/j/k`,
  `x/y/z/u/v`, domain math). Warn, don't force.
- **Over-built** — hand-rolled what stdlib/native/existing repo code
  already does → collapse (`AGENTS.md` ladder).
- **Dead code** — commented-out blocks → delete (git remembers).
- **Debug leftovers** — stray `print(...)` / `console.log(...)` → remove.
- **Swallowed errors** — `except: pass`, `catch {}` without a deliberate
  commented reason → surface (error handling, `mycelium-refactor`).

## What NOT to touch (anti-churn, from the rules)

- "Might need later" — YAGNI.
- Pure-aesthetic changes with no measurable benefit.
- Working code outside the change set.
- Mid-feature scaffolding — finish the feature first.
- Reformatting unrelated lines, restyling, unrequested renames.

## Auto-fix boundary — tool-driven ONLY

Apply without asking:

- `uv run ruff check --fix <changed .py>` and
  `pnpm exec prettier --write <changed .ts/.tsx>` — formatting / import order.

Everything judgment-based (extractions, renames, splits, restructured
control flow, deleting code you didn't write) is **proposed, not applied**
— these hit the confirmation gates in `mycelium-refactor`
(rewrite >100 lines, change public/exported signature, delete file). State
intent first: "Extract `_luma` dedup from X and Y into `_helpers.py`?"

## Process for an approved refactor

Per `mycelium-refactor` and `AGENTS.md`:

1. State intent before editing.
2. One refactor per change set — never mix refactor with feature work.
3. Behaviour stays identical — no new bug, no dropped feature.
4. After: `make lint && make test` (and `make typecheck` for frontend).
   Report results; never claim done without running them.

## Examples

```python
# Flag (single-letter + over-built): proposed, not applied
for n in nodes:          # → for node in nodes
    cache_evict(n)
# 30-line custom LRU      # → collections.OrderedDict (laziness ladder rung 3)
```

```
src/exec.py:42 — single-letter `n` (AGENTS.md naming) → `node`
src/exec.py:88 — except: pass (error handling) → surface error
src/exec.py:120 — hand-rolled LRU (laziness ladder) → OrderedDict
```

## Output

Terse list: `path:line — smell (which rule) → proposed change`. Group
auto-fixed vs proposed. Stop after 2 failed attempts at the same approach
and ask.
