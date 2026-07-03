---
name: mycelium-check
description: >-
  Mycelium pre-commit / pre-PR quality gate. Run the project's lint,
  typecheck, and test suite over the current diff, auto-apply only
  deterministic tool fixes (ruff --fix, prettier on changed files), and
  report everything else for the user to decide. Trigger whenever the user
  asks to "check", "quality check", "code check", "is this clean", "run the
  checks", "lint", "typecheck", "run tests", "ready to commit", "ready for
  PR", "verify my changes", or wants a green light before committing or
  opening a pull request in the Mycelium repo. Use this as the executable
  gate that runs the checks — mycelium-coding only describes the rules.
---

# Mycelium Check — the quality gate

Executable companion to `mycelium-coding`. That skill _describes_ the
discipline; this one _runs_ the checks and applies only safe, tool-driven
fixes.

## Scope: the diff, not the tree

Default target is the working-tree change set, not the whole repo. `make
check` already does this scoping for you (changed vs HEAD + untracked,
deletes filtered out, filenames-with-spaces safe), so prefer it over
hand-rolling `git diff | xargs`. Use `make check-all` only when the user
explicitly wants the whole repo (noisy — there's a large unformatted
backlog).

## Run order

Run these from repo root. Don't stop on first failure — collect all
results, then report once.

1. **Format + lint (changed files):** `make check` — runs
   `ruff format --check` + `ruff check` on changed `.py`, and
   `prettier --check` on changed `.ts/.tsx/.js/.json/.yaml/.md/.css`.
   Honest exit code. (ruff is the `dev` dependency-group tool, run via
   `uv run ruff`; `make lint` runs the same ruff.)
2. **Typecheck:** `make typecheck` (`pnpm -r typecheck`, builds libs first).
3. **Tests:** `make test` (pytest; graceful if no suite). Prefer scoping
   to touched packages when a path is obvious, e.g.
   `uv run pytest libraries/myc-nodes -q`.

`make typecheck` and `make build-libs` are slow (full lib build). Say so
before running; if the change is Python-only, you may skip frontend
typecheck and note that you skipped it.

4. **Naming (warn-only):** scan the diff (`git diff`) for single-letter
   function/variable names in added or changed lines. Whitelist: loop
   counters `i`/`j`/`k`, coords `x`/`y`/`z`/`u`/`v`, domain-standard math
   (`m`, `t`). Everything else (`e`, `n`, `r`, `s`, `f`, `p`, `c`, …) is a
   violation per `AGENTS.md` naming. Report each as a suggestion;
   never auto-rename (not a deterministic tool fix).

## Auto-fix boundary — tool-driven ONLY

Apply WITHOUT asking (deterministic, reversible by git):

- `make format` — runs `ruff format` + `ruff check --fix` +
  `prettier --write` on changed files only (formatting + lint autofixes +
  import sort). Pairs with `make check`. (`make format-all` does the
  whole-repo baseline — propose, never run unprompted; it's a ~700-file
  diff.)

Do NOT auto-apply, ever — propose instead:

- File splits, dedup/refactor, renames, signature changes (→ confirmation
  gates in `mycelium-refactor`).
- Logic edits to silence a type error or test failure.
- Anything ruff/prettier can't do deterministically.

After auto-fixing, re-run the relevant linter once to confirm it's clean.

## Report format

Terse. One block:

```
LINT      ✓ / ✗  (n auto-fixed)
TYPECHECK ✓ / ✗ / skipped
TEST      ✓ / ✗ / none
NAMING    ✓ / ⚠ (k single-letter names — warn only)
```

Then, only if not all green: each remaining issue as
`path:line — problem → suggested fix (proposed, not applied)`. Reference
locations, don't paste big blocks. If a check couldn't run in this env,
say so explicitly — never claim "should pass" without having run it
(`AGENTS.md` verification rule).

## Hand-offs

- File over 300/500 lines surfaced → `mycelium-file-guard`.
- A touched `BaseNode` subclass → `mycelium-node-check`.
- User wants simplification, not just pass/fail → `mycelium-simplify`.
