# 25-functions

## Concept
A function is a named, reusable piece of logic that takes inputs (parameters),
does something, and usually returns a value. Functions let you write a
computation once and call it from many places.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `int add(int a, int b);` — a **declaration** (prototype): return type, name,
  parameter types, ending in `;`. It promises the function exists.
- `add(1, 2)` — a **call**: supply arguments, receive the return value.
- `int add(int a, int b) { return a + b; }` — the **definition**: same signature,
  plus a body.

## Declaration vs definition
- A **declaration** tells the compiler the function's shape so calls type-check.
  You can have many.
- A **definition** provides the body. There must be exactly **one** (the
  One Definition Rule).
- Putting the declaration first lets `main` call `add` before its body appears —
  the basis for splitting declarations into headers (lesson 30).

## Common pitfalls
- **No return in a non-`void` function** is undefined behavior. Every path must
  return a value (compilers warn).
- Declaration and definition must match exactly (types, `const`, etc.), or you
  get "undefined reference" at link time.
- Calling with the wrong argument types may silently convert (e.g. `add(1.9,
  2.9)` truncates to `add(1, 2)`); the sanitizer won't catch logic like this.

## Build & run
```sh
make run app=25-functions
```

## Try it yourself
Add a second function `int square(int n)` (declare it up top, define it below)
and call it from `main`.
