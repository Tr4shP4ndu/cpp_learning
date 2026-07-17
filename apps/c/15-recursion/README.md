# 15-recursion (C)

A recursive function calls itself on a smaller subproblem until it hits a *base
case* that returns without recursing. Factorial is the classic example.

## Line-by-line
- `if (n <= 1) return 1;` — the base case; without it the recursion never stops.
- `return n * factorial(n - 1);` — the recursive case, moving toward the base.
- The loop in `main` prints `0!` through `5!`.

## Common pitfalls
- **Missing or unreachable base case** — the calls never stop and each one adds a
  stack frame, so you overflow the stack and crash.
- Deep recursion (large `n`) can overflow the stack even with a correct base case.
- Factorials grow fast; even `unsigned long` overflows well before large `n`.

## Compared to C++
Recursion works identically in C++ — same stack frames, same base-case rule. See
the [C++ version](../../cpp/30-recursive-function/).

## Build & run
```sh
make run-c-app-15-recursion
```
