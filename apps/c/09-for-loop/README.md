# 09-for-loop (C)

A `for` loop packs the counter setup, the stop test, and the step into one
header. Here it sums the integers `1..n`.

## Line-by-line
- `int sum = 0;` — the accumulator, initialized before the loop.
- `for (int i = 1; i <= n; i++)` — three parts: **init** (`int i = 1`, runs
  once), **condition** (`i <= n`, checked before each pass), **step** (`i++`,
  runs after each pass).
- `sum += i;` — add the current counter to the running total.
- Declaring `i` inside the init clause is **C99+**; classic C89 required
  declaring the counter before the loop.

## Common pitfalls
- Off-by-one: `i < n` stops at `n-1`; use `i <= n` to include `n`.
- Forgetting the step (`i++`) — the condition never changes and the loop spins
  forever.
- Using `i` after the loop — with the C99 form it is out of scope there.

## Compared to C++
Identical syntax and semantics; C++ adds range-based `for` for containers. See
the [C++ version](../../cpp/12-for-loop/).

## Build & run
```sh
make run-c-app-09-for-loop
```
