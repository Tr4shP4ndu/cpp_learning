# 33-preprocessor-macros

## Concept
The **preprocessor** runs before the compiler and does pure text substitution.
`#define` creates macros, `#include` pastes files, and `#if`/`#ifdef` decide
which lines the compiler even sees. It's powerful but blunt — it knows nothing
about types or scope. Modern C++ replaces most macros with `constexpr`,
`inline`, and templates; you still need to *read* macros in real code.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `#define GREETING "..."` — an **object-like** macro: every later `GREETING`
  becomes that text. No type, no scope, no semicolon.
- `#define SQUARE(x) ((x) * (x))` — a **function-like** macro. The inner and
  outer parentheses are mandatory: `SQUARE(1 + 2)` expands to `((1 + 2) * (1 + 2))`
  = 9. Without them you'd get `1 + 2 * 1 + 2` = 5.
- `#if ENABLE_EXTRA ... #else ... #endif` — conditional compilation. Only one
  branch is ever compiled; the other doesn't exist in the binary.
- `__LINE__` / `__FILE__` — predefined macros the compiler substitutes with the
  current line number and source file name.

## Why this matters
Include guards, platform `#ifdef`s, and `assert` are all preprocessor features
you'll meet constantly. Knowing macros are *textual* explains their sharp edges.

## Common pitfalls
- **Double evaluation:** `SQUARE(i++)` expands to `((i++) * (i++))` — `i` is
  incremented twice. A macro is not a function.
- **No scope / no type checking:** a macro leaks everywhere below its `#define`
  and silently substitutes text. Prefer `constexpr` and `inline` functions.
- Forgetting parentheses around parameters is the classic precedence bug above.

## Build & run
```sh
make run app=33-preprocessor-macros
```

## Try it yourself
Add `#define MAX(a, b) ((a) > (b) ? (a) : (b))`, then rewrite it as a
`constexpr` function template and note how the function version is immune to the
double-evaluation trap.
