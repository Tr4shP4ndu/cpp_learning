# 04-constants

## Concept
A constant is a value that cannot change after initialization. Marking values
`const` (or `constexpr`) documents intent, lets the compiler catch accidental
writes, and can enable optimizations.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `const double PI = 3.14159;` — `PI` is read-only; assigning to it later is a
  compile error (see the commented-out line).
- `std::is_const<decltype(PI)>::value` — asks the type system "is this const?"
  (`decltype(PI)` is the type of `PI`); prints `1` for true.
- `circumference` is computed *from* the constant — constants are used in
  expressions like any other value.

## `const` vs `constexpr`
- `const` — "I won't change this through this name." The value may still be
  computed at run time.
- `constexpr` — "this can be computed at *compile* time." Required where the
  language needs a compile-time value (array sizes, `case` labels, template
  arguments): `constexpr int BUF = 1024; int arr[BUF];`.

## Common pitfalls
- Naming convention: constants are often `ALL_CAPS`, but that's style, not a
  rule.
- Casting away `const` with `const_cast` and then writing to a truly-const
  object is **undefined behavior**. Don't.
- For a constant shared across files via a header, prefer `inline constexpr`
  (C++17) to avoid multiple-definition errors.
- `const` does not mean thread-safe by itself.

## Build & run
```sh
make run app=04-constants
```

## Try it yourself
Change `const double PI` to `constexpr double PI` and add
`static_assert(PI > 3.0);` above `main`. It compiles because `constexpr` values
are known at compile time.
