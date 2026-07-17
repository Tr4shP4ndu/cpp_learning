# 31-const-and-volatile (C)

`const` makes a variable read-only and documents intent — a `const` pointer
parameter promises the function won't modify the caller's data. `volatile`
tells the compiler a value may change outside normal control flow, so it must
re-read it every time instead of caching or optimising the access away.

## Line-by-line
- `int sum(const int *a, size_t n)` — `const int *` means "pointer to ints I
  won't change"; the compiler rejects any write through `a`.
- `const double PI = 3.14159;` — read-only; `PI = ...` later is a compile error.
- `volatile sig_atomic_t flag` — `sig_atomic_t` is the type safe to touch from a
  signal handler; `volatile` forces each `while (flag)` test to actually read
  memory rather than assume the value is unchanged.
- The loop clears `flag`, so it runs once and exits.

## Common pitfalls
- Casting away `const` and then writing through the pointer — undefined behaviour.
- Assuming `volatile` makes access atomic or thread-safe. It does neither; it
  only prevents the compiler from caching/eliding reads and writes.
- Using a plain `int` flag for signal handlers — use `volatile sig_atomic_t`.
- `const int *p` (data is const) vs `int * const p` (the pointer is const) —
  read the declaration right-to-left.

## Compared to C++
C++ builds a whole discipline of "const correctness" on top of the same keyword —
`const` member functions, `const` references, and `mutable`. See the
[C++ version](../../cpp/48-const-correctness/).

## Build & run
```sh
make run-c-app-31-const-and-volatile
```
