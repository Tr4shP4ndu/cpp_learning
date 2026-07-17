# 36-standard-library-tour (C)

A quick tour of some of C's most useful standard headers in one program:
sorting and searching, pseudo-random numbers, string-to-number parsing,
assertions, and the numeric limits macros.

## Line-by-line
- `cmp_int` — a comparator returning negative/zero/positive; `(x>y)-(x<y)` avoids
  the overflow of `x - y`.
- `qsort(v, n, sizeof v[0], cmp_int)` — generic in-place sort of any array.
- `bsearch(&key, v, n, ...)` — binary search on the *sorted* array; returns a
  pointer to a match or `NULL`. `%td` prints the `ptrdiff_t` index.
- `srand(1)` then `rand()` — a fixed seed makes the output reproducible.
- `strtol("-42", NULL, 10)` — parse text to `long`; `abs` gives magnitude.
- `assert(...)` — aborts the program if the condition is false (here it holds).
- `INT_MAX`, `int32_t` (printed with `PRId32`), `SIZE_MAX` — limits and
  fixed-width types from `<limits.h>`/`<stdint.h>`.

## Common pitfalls
- Calling `bsearch` on an array that isn't sorted with the same comparator.
- Writing `return a - b;` in a comparator — it can overflow for large values.
- Expecting `rand()` to be high-quality or seeded for you; without `srand` the
  seed is always 1 anyway.
- Ignoring that `assert` is disabled when `NDEBUG` is defined — never put
  side-effecting code inside `assert(...)`.

## Compared to C++
C++ offers `std::sort`, `std::binary_search`, `<random>`, and `std::from_chars`,
which are type-safe and usually faster than these C equivalents. The headers
here still exist in C++ (as `<cstdlib>` etc.). `<math.h>` and `<time.h>` are part
of the C library too, but this sampler avoids `<math.h>` (it needs a separate
`-lm` link).

## Build & run
```sh
make run-c-app-36-standard-library-tour
```
