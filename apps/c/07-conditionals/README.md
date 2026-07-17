# 07-conditionals (C)

`if` / `else if` / `else` run different code depending on a test. In C a
condition is just an `int` expression: any nonzero value is "true", zero is
"false". C had no built-in boolean type until `<stdbool.h>` arrived in C99.

## Line-by-line
- The `if` / `else if` / `else` chain runs the first branch whose condition is
  nonzero; the rest are skipped.
- `#include <stdbool.h>` gives you `bool`, `true`, and `false` — a thin layer
  over `int` added in C99.
- `bool passed = score >= 50;` — the comparison yields `1` or `0`; printing it
  with `%d` shows `1` because `bool` is really an integer.
- `if (score)` — no comparison needed; a nonzero `score` counts as true.
- `(score >= 50) ? "pass" : "fail"` — the ternary is a compact if/else that
  produces a value.

## Common pitfalls
- Writing `if (x = 5)` (assignment) instead of `if (x == 5)` (comparison) — the
  first assigns 5, is always true, and compiles without error.
- Forgetting braces and then adding a second statement: only the first line is
  guarded by the `if`.
- Comparing floating-point values for exact equality; rounding makes `==`
  unreliable.

## Compared to C++
C++ has a real `bool` type built in (no header needed) and, since C++17, lets
an `if` declare a variable in its condition. See the
[C++ version](../../cpp/07-conditionals/).

## Build & run
```sh
make run-c-app-07-conditionals
```
