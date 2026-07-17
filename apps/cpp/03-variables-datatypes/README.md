# 03-variables-datatypes

## Concept
A *variable* is a named box that holds a value of a specific *type*. C and C++
are statically typed: every variable's type is fixed at compile time, and the
type decides how many bytes it uses and what values it can hold.

## Minimal example
See `src/main.cpp`. The C version of this lesson lives in [`apps/c/03-variables-datatypes`](../../c/03-variables-datatypes/) (run it with `make run-c-app-03-variables-datatypes`).

## Line-by-line
- `int signedIntVar = -10;` — declare a variable, give it a type, initialize it.
- `sizeof(x)` — an operator that returns the size of a type/variable in bytes.
- Integer families: `short` ≤ `int` ≤ `long` ≤ `long long`; each has an
  `unsigned` version that trades negatives for a bigger positive range.
- `float` (~7 digits) vs `double` (~15 digits) for fractional numbers.
- `char` holds one byte / character; `bool` holds `true`/`false` (prints as 1/0).
- `std::string` (C++) holds text of any length and manages its own memory.

## Common pitfalls
- **Uninitialized variables** hold garbage. Always initialize: `int n = 0;` or
  `int n{};`. Brace-init `int x{3.14};` is even better — it *rejects* narrowing.
- **Sizes are not fixed by the standard** (only minimums). If you need an exact
  width, use `<cstdint>` / `<stdint.h>` types like `int32_t`.
- **Signed/unsigned mixing** in comparisons causes surprises; keep them separate.
- `char`'s signedness is implementation-defined; for raw bytes use
  `unsigned char` (or C++'s `std::byte`).

## C vs C++
| | C | C++ |
|---|---|---|
| Text | `char name[]` + `%s` | `std::string` |
| Print a value | `printf("%d", n)` needs a format specifier | `std::cout << n` figures out the type |
| Fixed-width types | `<stdint.h>` | `<cstdint>` |

The core numeric types (`int`, `long`, `float`, `double`, `char`) are the same
in both languages. The big difference is text: C strings are raw `char` arrays
you manage by hand (lesson 20), while C++ gives you `std::string`.

## Build & run
```sh
make run-cpp-app-03-variables-datatypes      # C++
make run-c-app-03-variables-datatypes    # C — note the printf format specifiers
```

## Try it yourself
Add a `long double` in C++ and print its `sizeof`. Compare it to `double`.
