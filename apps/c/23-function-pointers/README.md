# 23-function-pointers (C)

A function pointer is a variable that stores the address of a function, so you
can swap behavior at runtime or hand a function to another function as a
callback. The standard library's `qsort` is built on exactly this.

## Line-by-line
- `int (*op)(int, int) = add;` — `op` points to a function taking two `int`s and
  returning `int`. The parentheses around `*op` are required.
- `op(3, 4)` — call through the pointer (no `*` needed); `op = mul;` re-points it.
- `cmp_int(const void *a, const void *b)` — `qsort`'s comparator signature; cast
  the `void*` back to `const int*`, then dereference to compare.
- `(x > y) - (x < y)` — yields `-1`/`0`/`1` safely (subtracting the ints could
  overflow).
- `qsort(nums, n, sizeof nums[0], cmp_int)` — passes the comparator as a callback.

## Common pitfalls
- Dropping the parentheses: `int *op(int,int)` declares a *function returning
  `int*`*, not a pointer.
- Wrong comparator signature, or forgetting to cast the `void*` before deref.
- Returning `x - y` from the comparator — can overflow for large values.

## Compared to C++
C++ can use raw function pointers too, but usually prefers `std::function` and
lambdas (see cpp/57-lambdas) for callbacks — they capture state and read more
clearly than a bare function pointer.

## Build & run
```sh
make run-c-app-23-function-pointers
```
