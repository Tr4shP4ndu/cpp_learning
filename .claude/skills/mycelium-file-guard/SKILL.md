---
name: mycelium-file-guard
description: >-
  Enforce Mycelium's file-size caps (soft 300 lines, hard 500) and split
  oversized files along the project's own patterns — Zustand slices under
  packages/myc-store/src/slices, one BaseNode per file with heavy compute
  in _helpers.py siblings, viewer renderers by image type, types/constants
  co-located. Trigger when the
  user says "this file is too big", "split this file", "over the line
  limit", "file too long", "break this up", "check file sizes", "which
  files are over cap", or after another skill flags an oversized file. Does
  NOT auto-apply splits — proposes them behind a confirmation gate.
---

# Mycelium File Guard

Enforces the Mycelium file-size caps and drives splits along the project's
layout. Self-contained; pairs with `mycelium-refactor` for the split itself.

## The caps

- **Soft 300 lines** — approaching → suggest a split.
- **Hard 500 lines** — must split unless the user overrides.

Why (from the rules): big file = every read costs tokens, every edit risks
breaking unrelated code, context fills fast.

## Find over-cap files

Scope to changed files by default; whole repo only if asked.

```
# changed, non-deleted files with their line counts
git diff --name-only HEAD | while read f; do [ -f "$f" ] && wc -l "$f"; done | sort -rn
```

Flag anything > 300 (warn) / > 500 (must-split). Ignore generated/vendored
paths: `node_modules/`, `dist/`, `.venv/`, `*.lock`, `uv.lock`.

## Split patterns

- **One responsibility per file** — single concept, single export surface.
- **Pure helpers** → `lib/` or co-located `_<topic>_helpers.py`.
- **Types/interfaces** → `types.ts` / co-located `*.types.ts`.
- **Constants/config** → `constants.ts` / `config.py`.
- Group by feature, not layer, once the feature grows.

**Backend (`server/` & `libraries/`):** one node per file under the node
`builtin/<category>/` tree; heavy compute → `_<topic>_helpers.py` or
`_run.py` siblings (the codebase already does this, e.g. `autocomp/_run.py`,
`pipeline/_yaml.py`).

**Frontend (`packages/`):** React components split when > 300 lines —
subcomponents to siblings, hooks to `hooks/`, helpers to `*.utils.ts`. The
Zustand store (`packages/myc-store/src/useStore.ts`) uses the **slice
pattern** — split into `slices/<slice>.ts` (the repo already has
`clipboardSlice`, `workflowSlice`) and compose in the root. Viewer
renderers split by image type; pure shader strings → `*.shaders.ts`.

## Naming after a split

Short + descriptive: `auth-token-validator.ts`, not
`authTokenValidationHelperUtility.ts`. No `usr/cfg/mgr`. Verb for fn, noun
for value.

## This skill does NOT auto-apply

A split is a refactor that almost always crosses confirmation gates
(rewrite >100 lines, new files, moved exports). Per `mycelium-refactor` and
`AGENTS.md`: **state intent, get a yes, then execute.**

Process once approved:

1. State the exact split ("Move `_accumulate` / `_finalize` /
   `_sample_frames` to `grain_curve_helpers.py`, leave the node class.").
2. One split per change set — behaviour identical, no feature mixed in.
3. Update imports; keep the public export surface stable.
4. Verify: `make lint && make typecheck && make test`. Report results.

## Example

```
useStore.ts — 612 lines (🔴 > 500). slice pattern:
  → slices/clipboardSlice.ts   (already exists, pattern proven)
  → slices/viewerSlice.ts      (extract viewer state + actions)
  → useStore.ts composes slices
Proposed, not applied. Confirm before I move exports.
```

```
grain_curve.py — 340 lines (🟡 > 300)
  → grain_curve.py keeps the BaseNode class + define_io/execute
  → grain_curve_helpers.py gets _accumulate/_finalize/_sample_frames
```
