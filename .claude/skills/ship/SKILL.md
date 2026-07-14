---
name: ship
description: >-
  One-step ship for any repo — stage, commit, and either push to main or open a
  PR, without the user hand-rolling git add / commit / push. Trigger whenever
  the user says "ship", "ship it", "push this", "push the changes", "push to
  main", "commit and push", "send it", "make a PR", "open a PR", "raise a PR",
  or otherwise wants their working-tree changes into the repo quickly. Runs a
  fast format/lint gate first, stages everything safely (never secrets), writes
  a house-style commit message, and reports the result.
---

# Ship — commit + push or PR, no manual git

Get the working-tree changes into the repo in one step. The user should never
have to run `git add` / `commit` / `push` by hand. Two modes: **push to main**
or **open a PR**. Only commit/push when the user actually asks to ship.

## Pick the mode

- The user's words decide: "push to main" → push-to-main; "PR" / "pull
  request" → PR mode.
- If unspecified: on the default branch with a small, safe diff → push-to-main;
  a larger or riskier change → PR. Ask only when genuinely ambiguous.

## Pre-flight — fast gate (default on)

Run the repo's fast format/lint gate before committing. Skip only if the user
says "skip checks" / "no checks":

1. Auto-format changed files. Formatter output is deterministic and reversible
   by git, so apply without asking.
2. Lint the diff. If it fails on something a tool can't fix, **STOP and
   report** — don't push broken code.

This is the quick path — skip typecheck/test unless the change clearly needs
them. See `projects/<project>.md` for the exact gate commands.

## Stage — no manual git add

- `git add -A` — stage all tracked changes + new files in one go.
- **Never stage secrets/junk:** `.env*`, `*.pem`, `*.key`, `credentials*`,
  tokens, or anything matching a secret pattern → warn, don't stage. (Build
  dirs, `node_modules/`, generated output should already be gitignored.)
- If an unexpected large or generated file got staged, flag it before
  committing.
- Show the staged list (`git status --short`) so the change is visible.
- If there's nothing to commit, say so and stop — never make an empty commit.

## Branch first if on the default branch

If HEAD is the default branch (`main`/`master`), branch before committing:
`git switch -c <type>/<slug>` — short, derived from the change (e.g.
`feat/roto-matte`, `fix/proxy-colorspace`). Exception: an explicit
"push to main" on a shared default branch commits on it directly (confirm once).

## Commit message — match the house style

Read recent history (`git log --oneline -20`) and match its style — casing,
length, whether bodies are used. Add a one-line body only when the change isn't
obvious. Always write via HEREDOC, and end with the Co-Authored-By trailer the
environment requires:

```
git commit -m "$(cat <<'EOF'
<summary in the repo's house style>

Co-Authored-By: <name> <email>
EOF
)"
```

## Push to main

The default branch is shared — confirm once before the push, then:

```
git push origin HEAD
```

Report the result (branch, commit, remote).

## PR mode

1. Branch off the default branch (see above).
2. `git add -A` + commit (house style) + `git push -u origin HEAD`.
3. Open the PR:
   - If `gh` is available: `gh pr create --title "…" --body "$(cat <<'EOF' … EOF)"`.
     End the PR body with the environment's required attribution trailer.
   - Otherwise: `git push` prints a **"Create a pull request"** URL — surface
     it. Or build the compare URL from the remote:
     `<remote-web-url>/compare/<default>...<branch>?expand=1`.
   - Offer `brew install gh` if the user wants one-step PRs in future.

## Safety (non-negotiable)

- Only commit/push when the user asks — never ship unprompted.
- NEVER edit git config.
- NEVER force-push to the default branch; warn loudly if asked.
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

## Output

Ship result first, then ≤3 short lines: what was skipped, when to add it.
Follows the `minimal` skill — laziest solution that works, fewest words.

## Project overrides

If this skill has a `projects/<project>.md` matching the repo you're in, read it
too — it carries the repo's exact gate commands and commit conventions.
