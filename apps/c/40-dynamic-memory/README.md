# 41-dynamic-memory (C)

The C version of [`apps/cpp/41-dynamic-memory`](../../cpp/41-dynamic-memory/).
`malloc`/`free` are C's manual heap allocation.

## Line-by-line
- `int *p = malloc(sizeof *p);` — request a number of **bytes**; returns `void*`
  or `NULL`. `sizeof *p` sizes it to what `p` points at.
- **Always check for `NULL`** before using the result.
- `free(p);` — release. Exactly one `free` per `malloc`.
- For an array, `malloc(n * sizeof *arr)` then one `free` for the whole block.

## Common pitfalls
- **Leak** (never `free`), **double free**, **use-after-free**, and forgetting to
  check `malloc`'s return.

## Compared to C++
C++ uses `new`/`delete`, but in practice you use smart pointers (lesson 41) and
containers that free automatically via RAII — you almost never call `free`/
`delete` by hand. See the [C++ version](../../cpp/41-dynamic-memory/).

## Build & run
```sh
make run-c-app-41-dynamic-memory
```
