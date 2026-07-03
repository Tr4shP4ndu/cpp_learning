---
name: mycelium-coding
description: >-
  Mycelium coding discipline. Load this skill
  whenever the user is editing, reviewing, refactoring, or generating code
  in the Mycelium repo — including any work on server/, frontend/,
  custom_nodes/, plugins/, the FastAPI backend, React/Zustand frontend,
  PyTorch image-processing nodes, node-graph engine, viewer renderers,
  EXR/colorspace handling, AI inference nodes (SD/SDXL/FLUX/LTX), or any
  BaseNode subclass. Also trigger on mentions of "Mycelium", a file path
  under this repo, "refactor", "split this file", "this file is too big",
  "clean up", "the cursor rules", or any task where the user wants Claude to
  follow the project's editing discipline (read before edit, minimal diffs,
  file-size caps, no speculation, no sprawl, refactor process, confirmation
  gates, verification). Mirrors the always-on rules in AGENTS.md.
version: 0.1.0
---

# Mycelium — Coding Discipline

The entry point for editing code in this repo. Self-contained: the core
discipline is below, and the always-on rule `AGENTS.md` (naming, least-code
ladder, editing discipline, verification) applies on top. Task-specific work
hands off to the sibling skills.

## The discipline (always applies)

- **Read before you edit.** Trace the real flow end to end first. A small diff
  in the wrong place is a second bug.
- **Minimal diffs.** Change only what the task needs — no drive-by
  reformatting, no unrequested renames of stable symbols, no speculative
  sprawl. One concern per change set (never braid a refactor into a feature).
- **Climb the laziness ladder** before writing code (`AGENTS.md`): does it
  need to exist? already in the repo? stdlib / native / installed dep? one
  line? Only then write the minimum. Validation, security, and error handling
  are never cut.
- **Name for the next reader** (`AGENTS.md`): full words, no abbreviations or
  single letters outside the small whitelist.
- **Verify before you claim done** (`AGENTS.md`): run `make lint` /
  `make typecheck` / `make test`; never say "should pass."

## Mycelium layout (where code goes)

Dependencies run **downward only**: `apps/` → `packages/` (TS `myc-*`) →
`libraries/` (Python `myc-*`). One `BaseNode` per file under
`libraries/myc-nodes/.../builtin/<category>/`, heavy compute in `_helpers.py`
/ `_run.py` siblings. Zustand store uses the slice pattern under
`packages/myc-store/src/slices`. Viewer renderers split by image type.
File-size caps: soft 300, hard 500.

## Hand-offs to the task-specific skills

- Write only what's needed / avoid over-building → `mycelium-minimal`.
- Restructure or clean up existing code → `mycelium-refactor`.
- Simplify changed code to thresholds → `mycelium-simplify`.
- File over the size cap → `mycelium-file-guard`.
- A `BaseNode` subclass → `mycelium-node-check`.
- A bug / failure / unexpected behavior → `mycelium-debug`.
- Pre-commit / pre-PR pass-fail gate → `mycelium-check`.
- Documentation → `mycelium-docs`. Reusable library/API → `mycelium-library-api`.

## Reply style

Terse and direct — concise prose, no filler. Docs are the exception: written
in normal, complete prose (`mycelium-docs`).
