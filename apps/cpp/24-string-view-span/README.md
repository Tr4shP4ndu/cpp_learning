# 25-string-view-span

## Concept
A **view** borrows data it doesn't own: a pointer plus a length, no allocation
and no copy. `std::string_view` (C++17) views a sequence of characters;
`std::span` (C++20) views any contiguous block of elements (C array,
`std::array`, `std::vector`). Take them by value in function parameters so one
function accepts many kinds of caller without copying.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `void greet(std::string_view name)` — accepts a `std::string`, a string
  literal, or a substring with **no copy**. Compare to `const std::string&`,
  which would force a temporary `std::string` for a literal.
- `sv.substr(0, 2)` — returns another `string_view` into the same buffer; it
  allocates nothing (unlike `std::string::substr`, which copies).
- `int sum(std::span<const int> values)` — one function for `raw[]`, an array,
  or a `std::vector`. `span` deduces the length (here 4 for `raw`).
- Iterating a span with a range-for works exactly like iterating the container.

## Why this matters
Views turn "which container did the caller use?" into a non-question. You write
the algorithm once against a view, and callers pass whatever they have.

## Common pitfalls
- A view **does not own** its data. Never return a `string_view`/`span` to a
  local, and don't let the underlying buffer die while a view over it lives
  (dangling view = undefined behavior).
- `string_view` is **not** guaranteed null-terminated — don't hand `.data()` to
  a C API expecting a C string.
- `string_view` needs **C++17**, `span` needs **C++20**.

## Build & run
```sh
make run app=25-string-view-span
make build app=25-string-view-span STD=c++14   # span & string_view unavailable — fails
```

## Try it yourself
Write `first_word(std::string_view)` that returns the view up to the first
space — no allocation. Then add a `span` overload of `sum` that skips every
other element using `values.subspan(...)`.
