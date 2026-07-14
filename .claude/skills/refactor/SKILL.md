---
name: refactor
description: >-
  Restructure and simplify existing code without changing behaviour. Use when
  the user asks to "refactor", "clean up", "simplify", "split this file",
  "reduce complexity", "make this maintainable", "this file is a mess",
  "break this up", "tidy this up", "is there a cleaner way", "extract into a
  library / reusable module", or "this file is too big". Applies generic
  thresholds — rule-of-three, function length/nesting/param caps, naming, dead
  code, file-size caps, extract-to-reusable-unit. Auto-applies only
  deterministic tool fixes; proposes every judgment call behind a confirmation
  gate.
---

# Refactor

Restructure existing code so the next person can read and change it safely.
Behaviour stays identical — refactor changes shape, never function.

## Scope

Default to the change set: `git diff --name-only HEAD` plus staged files.
Honour an explicit path if the user gives one. **Read the relevant ranges
before proposing** — trace the real flow end to end. No blind rewrites, no
touching working code outside the change set.

## What to flag

- **Rule of three** — the same logic in 3+ places → extract a helper. Two
  occurrences do not justify an abstraction.
- **Function > 50 lines** or **3+ levels of nesting** → split, or use early
  returns.
- **Parameter list > 4** → group into an options object / struct, or split
  the function.
- **Naming that lies** — the name says one thing, the code does another →
  rename or split.
- **Single-letter names** in added/changed lines → rename to a full word
  (whitelist loop indices `i/j/k` and math coords `x/y/z`). Warn, don't force.
- **Over-built** — hand-rolled what the standard library, a native platform
  feature, or existing code already does → collapse it.
- **Dead code** — commented-out blocks, unreachable branches → delete; version
  control remembers.
- **Debug leftovers** — stray `print(...)` / `console.log(...)` → remove.
- **Swallowed errors** — `except: pass`, `catch {}` with no deliberate,
  commented reason → surface the error.

## File-size caps

- **Soft 300 lines** — approaching → suggest a split.
- **Hard 500 lines** — must split unless the user overrides.

A big file means every read costs tokens, every edit risks breaking unrelated
code, and context fills fast. Find over-cap files in the change set:

```
git diff --name-only HEAD | while read f; do [ -f "$f" ] && wc -l "$f"; done | sort -rn
```

Ignore generated/vendored paths: `node_modules/`, `dist/`, `.venv/`, `*.lock`.

Split patterns:

- **One responsibility per file** — single concept, single export surface.
- Pure helpers → co-located `_<topic>_helpers` / `*.utils`.
- Types/interfaces → `*.types` / co-located types file.
- Constants/config → their own file.
- Group by feature, not layer, once the feature grows.

Name after a split short + descriptive (`auth-token-validator`, not
`authTokenValidationHelperUtility`). Verb for a function, noun for a value.

## Extract to a reusable unit

- Extract when reused by 2+ consumers or it's a clear domain boundary.
  Used once → keep it where it is (don't pre-extract).
- Explicit public surface in one entry point; `_`-prefixed internals are
  private and callers never import them.
- Dependencies point **down only** — a shared unit never imports its consumers
  or hidden global state. Data in, data out.
- A public signature change is a confirmation gate — consumers depend on it.

## What NOT to touch (anti-churn)

- "Might need it later" — YAGNI.
- Pure-aesthetic reformatting with no measurable benefit.
- Working code outside the change set.
- Mid-feature scaffolding — finish the feature first.
- Unrequested renames of stable public/exported symbols.

## Auto-fix boundary — tool-driven only

Apply without asking, on changed files only: the project's formatter and
lint auto-fixer (e.g. `ruff check --fix`, `prettier --write`).

**Everything judgment-based is proposed, not applied:** extractions, renames,
file splits, moved exports, restructured control flow, deleting code you didn't
write. State intent first, in one line: "Extract the duplicated luma math from
`grade` and `exposure` into a `_color_helpers` module — confirm?"

## Process for an approved refactor

1. State intent before editing.
2. **One refactor per change set** — never mix a refactor with feature work.
3. Behaviour stays identical — no new bug, no dropped feature, public export
   surface stable.
4. Verify and report — run the project's lint, typecheck, and test commands;
   never claim done without running them.

Stop after 2 failed attempts at the same approach and ask.

## Output

Code/artifact first, then ≤3 short lines: what was skipped, when to add it.
Follows the `minimal` skill — laziest solution that works, fewest words.

Group findings auto-fixed vs proposed:

```
auto-fixed:
  grade.py — import order

proposed:
  grade.py:42        single-letter `n` → `node`
  grade.py:88        except: pass → surface the error
  exposure + grade   duplicated luma math (rule of three) → _color_helpers
  useStore.ts (612)  over hard cap → split by responsibility
```

## Project overrides

If this skill has a `projects/<project>.md` matching the repo you're in, read it
too — it carries project-specific layout, naming, and caps that override the
generic guidance above.
