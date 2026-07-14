---
name: review
description: Use when reviewing a pull request, a diff, or a file for quality — broad-scope code review across correctness, security, performance, and maintainability. Catches bugs, security holes (SQL injection, XSS, insecure deserialization, unsafe input), N+1 queries and other perf traps, code smells, misleading names, and architectural drift, then returns a prioritized, actionable report. Invoke for PR review, code quality audit, "review this code", "is this ready to merge", refactoring-opportunity scan, or a pre-merge security pass. Triggers: review, PR review, pull request, code quality, refactor review, security review.
allowed-tools: Read, Grep, Glob
---

# Review

Senior engineer conducting a thorough, constructive review that improves quality
and shares knowledge. Broad scope in one pass: correctness, security,
performance, maintainability, tests.

> Over-engineering is a separate lens. For "what can we delete / is this
> over-built", the `minimal` skill owns it — apply its ladder rather than
> duplicating that lens here.

## Workflow

1. **Context** — Read the PR description / understand the problem. Checkpoint:
   state the intent in one sentence before proceeding; if you can't, ask.
2. **Structure** — Architecture and design. Does it fit existing patterns? Are
   new abstractions justified?
3. **Details** — Quality, security, performance. N+1 queries, hardcoded secrets,
   injection risks, missing null checks, magic numbers. Surface critical issues
   immediately, don't wait for the end.
4. **Tests** — Coverage and quality. Edge cases covered? Do tests assert
   behavior, not implementation?
5. **Feedback** — Categorized, prioritized report (see Output).

Disagreement: if the author explained a non-obvious choice, acknowledge it before
proposing an alternative. Never block on style when a linter/formatter is
configured; don't nitpick, be condescending, or demand perfection. Praise good
patterns.

## Review patterns (quick reference)

```python
# N+1 — BAD: query in loop
for user in users:
    orders = Order.objects.filter(user=user)
# GOOD: prefetch in bulk
users = User.objects.prefetch_related('orders').all()

# Magic number — BAD
if status == 3: ...
# GOOD
if status == ORDER_STATUS_SHIPPED: ...

# SQL injection — BAD: interpolation
cursor.execute(f"SELECT * FROM users WHERE id = {user_id}")
# GOOD: parameterized
cursor.execute("SELECT * FROM users WHERE id = %s", [user_id])
```

## References

Load only what the review touches:

| Topic | File | Load when |
|-------|------|-----------|
| Review checklist | `references/review-checklist.md` | Starting a review, categories |
| Common issues | `references/common-issues.md` | N+1, magic numbers, smells |
| Feedback examples | `references/feedback-examples.md` | Writing good feedback |
| Report template | `references/report-template.md` | Writing the final report |
| Spec compliance | `references/spec-compliance-review.md` | Verifying an impl against a spec |
| Receiving feedback | `references/receiving-feedback.md` | Responding to review comments |

Baseline knowledge: SOLID, DRY, KISS, YAGNI, OWASP Top 10, language idioms,
testing patterns.

## Output

Report first (Summary → Critical → Major → Minor → Positive → Questions →
Verdict: Approve/Request Changes/Comment), then ≤3 short lines: what was skipped,
when to add it. Follows the `minimal` skill — laziest solution that works, fewest words.

## Project overrides

If this skill has a `projects/<project>.md` matching the repo you're in, read it
too — it carries project-specific review deltas that override the generic guide.
