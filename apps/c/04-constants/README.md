# 04-constants (C)

C has three common ways to name a constant: `#define MAX 100` (preprocessor
text substitution), `const double PI = 3.14159;` (a typed read-only variable),
and `enum { LIMIT = 50 };` (a named compile-time int).

## Line-by-line
- `#define MAX 100` — the preprocessor replaces every `MAX` with `100` before
  compilation. Untyped, no storage; commonly used for array sizes.
- `enum { LIMIT = 50 };` — an anonymous enum makes `LIMIT` a compile-time int
  constant; also valid as an array size.
- `const double PI = 3.14159;` — a real typed object that is read-only;
  assigning to it is a compile error. Use it when you want a type and scope.
- `int buffer[MAX];` / `int slots[LIMIT];` — both `#define` and `enum`
  constants can size an array.
- `const int n = 8;` — a `const` int is NOT a constant expression in C, so
  `int a[n];` would be a variable-length array, not a fixed-size one.

## Common pitfalls
- `#define MAX 100;` (trailing semicolon) — the `;` gets pasted in too, often
  breaking the code where `MAX` is used.
- Assuming a `const int` can size a fixed array like in C++ — in C it cannot.
- `#define` has no scope and ignores types; a stray macro name can clash with
  variables anywhere below it.

## Compared to C++
In C++ a `const int` (or better, `constexpr`) IS a constant expression and can
size an array, so `#define` and enum tricks are rarely needed. See the
[C++ version](../../cpp/04-constants/).

## Build & run
```sh
make run-c-app-04-constants
```
