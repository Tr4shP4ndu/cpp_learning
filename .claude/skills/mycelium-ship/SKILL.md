---
name: mycelium-ship
description: >-
  Mycelium one-step ship — stage, commit, and either push to main or open a
  PR, without the user hand-rolling git add / commit / push. Trigger whenever
  the user says "ship", "ship it", "push this", "push the changes", "push to
  main", "commit and push", "send it", "make a PR", "open a PR", "raise a PR",
  or otherwise wants their working-tree changes into the repo quickly. Runs a
  fast format/lint gate first, stages everything safely (never secrets), writes
  a house-style commit message, and reports the result. Defer the full
  lint+typecheck+test gate to mycelium-check.
---

# Mycelium Ship — commit + push or PR, no manual git

Get the working-tree changes into the repo in one step. The user should never
have to run `git add` / `commit` / `push` by hand. Two modes: **push to main**
or **open a PR**.

## Pick the mode

- The user's words decide: "push to main" → push-to-main; "PR" / "pull
  request" → PR mode.
- If unspecified: on `main` with a small, safe diff → push-to-main; a larger or
  riskier change → PR. Ask only when genuinely ambiguous.

## Pre-flight — fast gate (default on)

Run before committing. Skip only if the user says "skip checks" / "no checks":

1. `make format` — auto-fix changed files (ruff format + ruff `--fix` +
   prettier). Deterministic and reversible by git, so apply without asking.
2. `make check` — format/lint on the diff. If it fails on something a tool
   can't fix, **STOP and report** — don't push broken code.

This is the quick path: skip `make typecheck` / `make test` unless the change
clearly needs them. For the full gate, hand off to `mycelium-check`.

## Stage — no manual git add

- `git add -A` — stage all tracked changes + new files in one go.
- **Never stage secrets/junk:** `.env*`, `*.pem`, `*.key`, `credentials*`,
  tokens, or anything matching a secret pattern → warn, don't stage.
  (`graphify-out/`, `node_modules/`, build dirs are gitignored already.)
- If an unexpected large or generated file got staged, flag it before
  committing.
- Show the staged list (`git status --short`) so the change is visible.
- If there's nothing to commit, say so and stop — never make an empty commit.

## Commit message — match the house style

This repo's log is **terse, lowercase, one line** ("docs", "graphify",
"fixing make roto2"). Match it: a short lowercase summary of what changed. Add
a one-line body only when the change isn't obvious. Always via HEREDOC:

```
git commit -m "$(cat <<'EOF'
<short lowercase summary>
EOF
)"
```

## Push to main

`main` is shared — confirm once before the push, then:

```
git push origin HEAD
```

Report the result (branch, commit, remote).

## PR mode (gh is NOT installed here)

1. Branch off main: `git switch -c <type>/<slug>` — short, derived from the
   change (e.g. `feat/roto-matte-app`, `fix/proxy-colorspace`).
2. `git add -A` + commit (house style) + `git push -u origin HEAD`.
3. Open the PR:
   - If `gh` is available: `gh pr create --title "…" --body "$(cat <<'EOF' … EOF)"`.
   - Otherwise: `git push` prints a **"Create a pull request"** URL — surface
     it. Or build it: `https://github.com/flwls/Mycelium/compare/main...<branch>?expand=1`.
   - Offer `brew install gh` if the user wants one-step PRs in future.

## Safety (non-negotiable — mirrors AGENTS.md git rules)

- NEVER edit git config.
- NEVER force-push to main; warn loudly if asked.
- No `--no-verify`, no skipping hooks, no interactive (`-i`) flags.
- Avoid `--amend` unless the HEAD commit is yours, unpushed, and the user asked.
- If a commit is rejected by a hook, fix the issue and make a **new** commit —
  never amend a rejected one.

## Report

One terse block:

```
GATE    ✓ format + lint  (or ✗ + what failed)
BRANCH  main → origin/main   (or feat/… → PR)
COMMIT  <message>   (n files)
PUSH    ✓   (or PR: <url>)
```

## Hand-offs

- Full lint + typecheck + test before a serious push → `mycelium-check`.
- Wants the change simplified/refactored first → `mycelium-simplify` /
  `mycelium-refactor`.
