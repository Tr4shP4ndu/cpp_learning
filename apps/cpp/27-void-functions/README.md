# 27-void-functions

## Concept
A `void` function returns no value. You call it for its *side effect* — printing,
modifying something, logging — not to get an answer back.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `void printBanner()` — the `void` return type means "nothing comes back".
- `greet(const std::string& name)` — void functions can still take parameters.
- Inside, a bare `return;` (no value) is optional and can be used to exit early.
- In `main`, calls like `printBanner();` are complete statements — there's no
  value to store.

## Common pitfalls
- Trying to use the result — `int x = printBanner();` — is a compile error;
  there's no value.
- A `void` function may use `return;` alone, but **`return someValue;` is an
  error**.
- If you find yourself passing something in *only* to read a result back out,
  you probably want a non-`void` return value instead.

## Build & run
```sh
make run-cpp-app-27-void-functions
```

## Try it yourself
Write `void printSum(int a, int b)` that prints `a + b`, and call it from
`main`. Note you print inside the function rather than returning.
