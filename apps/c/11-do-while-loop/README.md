# 11-do-while-loop (C)

A `do { ... } while (cond);` loop runs its body first and tests the condition
*after* — so the body always executes at least once. This is the classic
retry/menu shape (kept non-interactive here).

## Line-by-line
- `do { ... }` — the body runs unconditionally the first time.
- `attempt++;` then `printf(...)` — the work done each pass.
- `while (attempt != correct);` — re-run until the attempt matches; note the
  **required semicolon** after the condition.

## Common pitfalls
- Forgetting the trailing `;` after `while (...)` — a compile error unique to
  `do`/`while`.
- Reaching for `do`/`while` when the body might need to run **zero** times —
  use a plain `while` there instead.
- Never updating the tested variable inside the body — an infinite loop.

## Compared to C++
Identical in both languages. See the
[C++ version](../../cpp/15-do-while-loop/).

## Build & run
```sh
make run-c-app-11-do-while-loop
```
