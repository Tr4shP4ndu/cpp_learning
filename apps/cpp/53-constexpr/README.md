# 53-constexpr

## Concept
`constexpr` marks something that **can be computed at compile time**. A
`constexpr` variable is a true compile-time constant; a `constexpr` function
runs during compilation when its arguments are known, and falls back to a normal
run-time call otherwise. Moving work to compile time means zero run-time cost and
values usable where the language *requires* a constant (array sizes, template
arguments, `static_assert`).

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `constexpr int factorial(int n)` — a function usable at compile time. Note it's
  an ordinary loop; since C++14 `constexpr` functions can have real bodies.
- `constexpr int f5 = factorial(5);` — evaluated by the compiler; `f5` is baked
  into the program.
- `static_assert(f5 == 120, ...)` — checked at compile time (lesson 36); proves
  the value was known then.
- `std::array<int, factorial(4)>` — the size must be a compile-time constant, and
  `factorial(4)` supplies one (24). A run-time `int` here would not compile.
- `consteval int square(int)` — C++20: **must** be compile-time. `square(9)` is
  fine; `square(runtimeVar)` would be an error.
- `factorial(n)` with a run-time `n` — the same function, now called at run time.

## Why this matters
- **Performance:** results computed once by the compiler, not repeatedly at run
  time.
- **Correctness:** compile-time checks (`static_assert`) catch bad values before
  the program ever runs.
- It underpins a lot of modern generic code (lesson 54, templates) and the
  standard library's compile-time facilities.

## Common pitfalls
- `constexpr` is a *permission*, not a *guarantee*: a `constexpr` function called
  with run-time args just runs at run time. Use `consteval` to force compile time.
- Not everything is allowed in a constant expression (no I/O, no `new` until
  C++20's limited support) — the compiler tells you when a body isn't valid.
- `const` ≠ `constexpr`: `const` means "won't change," `constexpr` means "known at
  compile time."

## Build & run
```sh
make run-cpp-app-53-constexpr
make build-cpp-app-53-constexpr STD=c++17   # consteval is C++20 — fails
```

## Try it yourself
Write a `constexpr bool is_prime(int)` and use it in a `static_assert`. Then try
calling it on a run-time variable and confirm it still compiles (as a run-time
call).
