# 35-variadic-functions (C)

A variadic function takes a fixed prefix of parameters followed by `...`, then
walks the extra arguments with the `<stdarg.h>` macros. `printf` is the classic
example. C gives you no way to count the extra arguments — you must pass the
count (or a sentinel, or a format string) yourself.

## Line-by-line
- `int sum_ints(int count, ...)` — `count` is named; `...` is everything after.
- `va_list ap;` — the cursor over the unnamed arguments.
- `va_start(ap, count);` — initialise it, naming the last fixed parameter.
- `va_arg(ap, int)` — read the next argument as an `int` and advance.
- `va_end(ap);` — mandatory cleanup before returning.
- `average` reads with `va_arg(ap, double)` because floats are promoted to
  `double` when passed through `...`.

## Common pitfalls
- Reading the wrong type: `va_arg(ap, float)` is wrong — default argument
  promotions turn `float` into `double`, `char`/`short` into `int`.
- Reading more arguments than were actually passed — undefined behaviour, since
  nothing records the real count.
- Forgetting `va_end`, or calling `va_start` without a matching `va_end`.

## Compared to C++
C++ can still use `<stdarg.h>`, but prefers type-safe variadic templates and
`std::format` (see [cpp/60-format](../../cpp/60-format/)) — these know the
argument types and count at compile time, eliminating the manual bookkeeping.

## Build & run
```sh
make run-c-app-35-variadic-functions
```
