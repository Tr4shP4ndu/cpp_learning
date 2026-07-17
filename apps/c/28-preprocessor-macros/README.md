# 28-preprocessor-macros (C)

The preprocessor rewrites your source *before* the compiler sees it. `#define`
introduces textual substitutions, `#include` pastes in another file, and
`#if`/`#ifdef` compile code conditionally.

## Line-by-line
- `#define PI 3.14159` — object-like macro: every `PI` becomes the text `3.14159`.
- `#define SQUARE(x) ((x) * (x))` — function-like macro; parenthesise each
  argument and the whole body so `SQUARE(1 + 2)` expands correctly to `9`.
- `#include <stdio.h>` — pastes the header's contents in at this point.
- `#if VERBOSE ... #else ... #endif` — keep one branch, drop the other at compile time.
- `#ifdef PI` — compile the block only if `PI` is currently defined.
- `#undef PI` — remove the definition so `PI` is no longer a macro below here.
- An **include guard** (`#ifndef GREET_H / #define GREET_H / #endif`, see lesson 29)
  uses the same directives to stop a header being pasted in twice.

## Common pitfalls
- Missing parentheses: `#define SQUARE(x) x*x` makes `SQUARE(1+2)` expand to `1+2*1+2` = 5.
- A macro is pure text with no type checking — the compiler never sees the name.
- No trailing semicolon on `#define`; the text after the name is the replacement.
- Macro arguments can be evaluated more than once (`SQUARE(i++)` increments twice).

## Compared to C++
Identical preprocessor, but C++ prefers `const`/`constexpr` for constants and
inline functions/templates for `SQUARE`, all of which are type-safe. See the
[C++ version](../../cpp/33-preprocessor-macros/).

## Build & run
```sh
make run-c-app-28-preprocessor-macros
```
