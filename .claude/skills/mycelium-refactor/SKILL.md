---
name: mycelium-refactor
description: >-
  Refactor existing code in the Mycelium monorepo against the project's own
  architecture and thresholds — not generic advice. Use when the user asks to
  "refactor", "restructure", "clean up this module", "break this up", "reduce
  complexity", "make this maintainable", "this file is a mess", "split this",
  or "extract this". Knows the repo layout (apps → packages → libraries,
  one BaseNode per file with _helpers/_run siblings, Zustand slices, viewer
  renderers by image type) and the 300/500-line caps. Self-contained; defers
  naming and the laziness ladder to AGENTS.md.
  Proposes every judgment-call change behind a confirmation gate; auto-applies
  only deterministic tool fixes.
---

# Mycelium Refactor

Restructure existing Mycelium code so the next person can read and change it
safely. Behaviour stays identical — refactor changes shape, never function.
Naming and the laziness ladder follow `AGENTS.md` — the ladder decides whether
code should exist at all before you move it.

## Scope

Default to the change set: `git diff --name-only HEAD` plus staged files.
Honour an explicit path if the user gives one. **Read the relevant ranges
before proposing** — trace the real flow end to end. No blind rewrites, no
touching working code outside the change set.

## Know the layout (this repo, not a generic one)

Dependencies run **downward only**: `apps/` → `packages/` (TS `myc-*`) →
`libraries/` (Python `myc-*`). Never make a library import an app, and never
introduce a sideways dependency between two libraries to dodge a refactor.

- **Backend nodes** (`libraries/myc-nodes/src/myc_nodes/builtin/<category>/`):
  one `BaseNode` subclass per file. Heavy compute belongs in `_helpers.py` or
  `_run.py` siblings next to the node, not inside the node class.
- **Zustand store** (`packages/myc-store/`): the **slice pattern**. Split
  state + actions into `slices/<name>.ts` and compose in the root store —
  don't grow one mega-store file.
- **Viewer** (`packages/myc-viewer/`): renderers split by image type; pure
  shader strings live in `*.shaders.ts`.
- **Frontend components** (`packages/`): subcomponents → siblings, hooks →
  `hooks/`, pure helpers → `*.utils.ts`, types → `*.types.ts`.

## File-size caps

- **Soft 300 lines** — approaching → suggest a split.
- **Hard 500 lines** — must split unless the user overrides.

Find over-cap files in the change set:

```
git diff --name-only HEAD | while read f; do [ -f "$f" ] && wc -l "$f"; done | sort -rn
```

Ignore generated/vendored paths: `node_modules/`, `dist/`, `.venv/`,
`.toolchain/`, `*.lock`, `uv.lock`, `pnpm-lock.yaml`.

## What to flag

- **Rule of three** — the same logic in 3+ places → extract a helper. Two
  occurrences do not justify an abstraction.
- **Function > 50 lines** or **3+ levels of nesting** → consider a split or
  early returns.
- **Parameter list > 4** → group into an options object / dataclass, or split
  the function.
- **Naming that lies / abbreviations** → rename per `AGENTS.md`.
- **Over-built** — hand-rolled what the stdlib, a native platform feature, or
  existing repo code already does → collapse it (the laziness ladder).
- **Dead code** — commented-out blocks, unreachable branches → delete; git
  remembers.
- **Debug leftovers** — stray `print(...)` / `console.log(...)` → remove.
- **Swallowed errors** — `except: pass`, `catch {}` with no deliberate,
  commented reason → surface the error.

## What NOT to touch (anti-churn)

- "Might need it later" — YAGNI.
- Pure-aesthetic reformatting with no measurable benefit.
- Working code outside the change set.
- Mid-feature scaffolding — finish the feature first.
- Unrequested renames of stable public/exported symbols.

## Auto-fix boundary — tool-driven only

Apply without asking, on changed files only:

- `uv run ruff check --fix <changed .py>` — lint/import order.
- `pnpm exec prettier --write <changed .ts/.tsx>` — formatting.

**Everything judgment-based is proposed, not applied:** extractions, renames,
file splits, moved exports, restructured control flow, deleting code you
didn't write. State intent first, in one line:
"Extract the duplicated luma math from `grade.py` and `exposure.py` into
`_color_helpers.py` — confirm?"

## Process for an approved refactor

1. State intent before editing.
2. **One refactor per change set** — never mix a refactor with feature work.
3. Behaviour stays identical — no new bug, no dropped feature, public export
   surface stable.
4. Verify and report — never claim done without running:
   - `make lint`
   - `make typecheck` (anything touching `packages/` / `apps/` TS)
   - `make test`

Stop after 2 failed attempts at the same approach and ask.

## Output format

Terse list, grouped auto-fixed vs proposed:

```
auto-fixed:
  libraries/myc-nodes/.../grade.py — ruff import order

proposed:
  grade.py:42        single-letter `n` → `node` (AGENTS.md)
  grade.py:88        except: pass → surface the error
  exposure.py + grade.py  duplicated luma math (rule of three) → _color_helpers.py
  useStore.ts (612)  over hard cap → slices/viewerSlice.ts (slice pattern)
```
