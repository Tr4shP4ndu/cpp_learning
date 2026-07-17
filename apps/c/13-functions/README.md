# 13-functions (C)

A function has a *prototype* (declaration) and a *definition*. Declaring the
prototype above `main` lets you define the body below while `main` still knows
how to call it.

## Line-by-line
- `int add(int a, int b);` — a prototype: return type, name, parameter types, `;`.
- `void greet(void);` — `void` return means "no result"; `(void)` params means
  "takes no arguments".
- `greet();` / `add(3, 4)` — calls resolved against the visible prototypes.
- The definitions below `main` supply the actual bodies.

## Common pitfalls
- Writing `void greet()` in C means "unspecified arguments", not "none" — always
  use `(void)` for a no-argument function.
- Calling a function with no prototype in scope: C99 removed implicit-`int`, so a
  missing declaration is now an error, not a silent guess.
- Prototype and definition must agree exactly, or the compiler complains.

## Compared to C++
Same declaration/definition split, but C++ never had implicit-`int` and treats
`greet()` as taking no arguments (like C's `(void)`). See the
[C++ version](../../cpp/26-functions/).

## Build & run
```sh
make run-c-app-13-functions
```
