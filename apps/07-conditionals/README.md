# 07-conditionals

## Concept
Conditionals let a program choose what to do. `if` runs a block when a test is
true; `else if` and `else` handle the other cases. The ternary `?:` is a compact
expression form.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `if (score >= 90) { ... }` — the braced block runs only when the condition is
  true.
- `else if (...)` / `else` — checked in order; the **first** matching branch
  runs and the rest are skipped.
- `(score >= 50) ? "pass" : "fail"` — evaluates to the first value if true, the
  second if false.
- `if (int bonus = score % 10; bonus > 5)` — C++17 init-statement: `bonus` is
  declared *and* scoped to just this `if`/`else`.

## Common pitfalls
- **`=` vs `==`.** `if (x = 5)` *assigns* 5 and is always true; you meant
  `if (x == 5)`. Compilers warn about this — heed the warning.
- **Missing braces.** `if (c) doA(); doB();` — `doB()` always runs. Always use
  braces, even for one line.
- A condition is "true" if it's non-zero. `if (ptr)` is idiomatic for "ptr is
  not null".

## Build & run
```sh
make run app=07-conditionals
make build app=07-conditionals STD=c++14 # warns: init-if is a C++17 extension
```

## Try it yourself
Add an `else if (score >= 60)` branch for "Grade: C" and pick a `score` that
lands in it.
