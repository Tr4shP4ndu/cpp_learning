# 53-concepts

## Concept
Concepts (C++20) put **named, checkable requirements** on template type
parameters. Where a plain template's requirements are implicit (lesson 52), a
concept states them up front — so misuse fails with a short, clear message
("constraint not satisfied") instead of pages of template errors, and the
requirement documents itself.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `template <typename T> concept Numeric = std::integral<T> ||
  std::floating_point<T>;` — define a concept: `T` is Numeric if it's an integer
  or floating-point type.
- `template <Numeric T> T sum(T a, T b)` — constrain the template. `sum` now
  *only* accepts numeric types.
- `template <typename T> requires std::floating_point<T>` — the `requires`
  clause writes a constraint inline, for one-off cases.
- The commented `sum(std::string...)` would fail the constraint at the call
  site, clearly.

## Why concepts
- **Readable errors:** "constraint `Numeric` not satisfied" beats a 200-line
  instantiation dump.
- **Self-documenting APIs:** the signature says what types are allowed.
- **Overloading on constraints:** you can have different implementations for
  different concept-satisfying types.
The standard library ships many ready-made concepts in `<concepts>`
(`std::integral`, `std::floating_point`, `std::same_as`, `std::convertible_to`,
…).

## Common pitfalls
- Concepts are **C++20** — build with C++20 or newer (this repo defaults to
  C++23).
- A concept is a compile-time predicate, not a runtime check.

## Build & run
```sh
make run app=53-concepts
make build app=53-concepts STD=c++17   # concepts don't exist yet — fails
```

## Try it yourself
Write a concept `Printable` requiring `std::cout << t` to compile (use a
`requires { ... }` expression) and constrain a `print(T)` function with it.
