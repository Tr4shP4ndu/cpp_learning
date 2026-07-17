# 35-testing

## Concept
A test pins down what code *should* do and fails loudly when it stops doing it.
You don't need a framework to start: a handful of checks that compare actual to
expected, count failures, and return a non-zero exit code is a real test suite.
This lesson builds that harness in ~15 lines so you understand what frameworks
like GoogleTest or Catch2 automate.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `add(int, int)` — the trivial function under test.
- `#define CHECK(expr)` — expands to: count the check, and if `expr` is false,
  count a failure and print `#expr` (the **stringized** expression) with
  `__FILE__`/`__LINE__` so you know exactly what broke and where.
- The `do { ... } while (0)` wrapper lets the macro be used like a statement
  (with a trailing `;`) even inside an `if` with no braces.
- `return failures == 0 ? 0 : 1;` — a test runner reports success through its
  **exit code**, so CI and `make` can tell pass from fail without reading output.

## Why this matters
`make` treats a non-zero exit as failure. That single convention is how automated
pipelines gate on tests — the printed summary is for humans, the exit code is for
machines.

## Common pitfalls
- Printing "FAIL" but returning `0` — the suite looks red to a human but green to
  CI. Always propagate failure through the exit code.
- Tests that depend on each other's order or shared mutable state: keep each
  check independent.
- Asserting on `==` between floating-point results — compare with a tolerance.

## Build & run
```sh
make run-cpp-app-35-testing         # prints "3/3 checks passed", exits 0
echo $?                          # 0 on success; break a CHECK to see 1
```

## Try it yourself
Add a deliberately wrong check (`CHECK(add(2, 2) == 5)`), run it, and confirm the
file:line report points at your line and that `echo $?` prints 1.
