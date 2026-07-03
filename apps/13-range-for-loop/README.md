# 13-range-for-loop

## Concept
The range-based `for` (C++11) walks every element of a container directly. You
don't manage an index or bounds, so the whole class of off-by-one bugs
disappears. Prefer it whenever you just need "each element".

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `for (const auto& element : values)` — bind `element` to each item in turn.
  - `auto` deduces the element type for you.
  - `const &` avoids copying and forbids modifying — the right default for
    read-only passes.
- `for (auto& element : values)` — a non-const reference lets you *modify* each
  element in place (here, multiply by 10).

## Common pitfalls
- **`for (auto x : big)`** copies every element. For anything bigger than an
  `int`, use `const auto&`.
- You can't easily get the current *index* from a range-for. If you need it, use
  a classic `for` (lesson 12) or `std::views::enumerate` (C++23).
- Modifying the container's *size* (adding/removing) while iterating is
  undefined behavior — only element *values* are safe to change.

## Build & run
```sh
make run app=13-range-for-loop
make run app=13-range-for-loop STD=c++03  # range-for didn't exist yet — fails
```

## Try it yourself
Sum the elements: declare `int total = 0;` before the loop and add each
`element` to it.
