# 14-function-parameters (C)

C passes arguments *by value* — the function gets a copy. To let a function
change the caller's variable you pass a pointer to it. Arrays are the exception:
they decay to a pointer, so the function sees the original.

## Line-by-line
- `void try_increment(int n)` — `n` is a copy; `n = n + 1` changes only the copy.
- `void increment(int *n)` — take the address (`&x`), write through it with `*n`.
- `void fill(int a[], int n)` — `a` is really `int *`; writes land in the caller's
  array. You pass the length separately because the pointer doesn't carry it.
- `increment(&x)` / `fill(nums, 3)` — `&x` is an address; `nums` decays to a pointer.

## Common pitfalls
- Expecting `try_increment(x)` to change `x` — it can't; pass `&x` instead.
- Forgetting the `&` when a function wants a pointer, or the `*` when reading it.
- Using `sizeof` on an array parameter to get its length — it measures the
  pointer, not the array, so always pass the count explicitly.

## Compared to C++
C++ adds reference parameters (`int &n`) so you can modify the caller without the
`&`/`*` syntax, and containers that carry their own size. See the
[C++ version](../../cpp/28-functions-with-parameters/).

## Build & run
```sh
make run-c-app-14-function-parameters
```
