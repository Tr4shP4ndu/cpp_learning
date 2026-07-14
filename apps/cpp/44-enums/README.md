# 44-enums

## Concept
An enum defines a type whose values come from a small, fixed, named set —
`Direction` is exactly one of North/East/South/West. Enums make code
self-documenting and let the compiler catch invalid values, replacing "magic
numbers".

## Minimal example
See `src/main.cpp`.

## `enum class` vs plain `enum`
- **`enum class Direction { ... }`** (scoped, C++11) — the modern default.
  Values are accessed as `Direction::North`, and they do **not** implicitly
  convert to `int`. This prevents mixing unrelated enums and accidental
  arithmetic.
- **`enum Color { ... }`** (unscoped, C-style) — names leak into the enclosing
  scope (`Green` directly) and convert to `int` freely. Convenient but
  bug-prone.

## Line-by-line
- `enum class Direction { North, East, South, West };` — North=0, East=1, …
  by default.
- `Direction d = Direction::East;` — must qualify with the enum name.
- The `switch` over a scoped enum is exhaustive and readable; the compiler can
  warn if you miss a case.
- `Color c = Green;` then `c` prints as `1` — the unscoped enum converts to int.

## Common pitfalls
- With plain `enum`, name collisions (two enums both defining `Red`) and
  unintended int conversions. Prefer `enum class`.
- You can set explicit values (`enum class Http { Ok = 200, NotFound = 404 };`)
  and choose the underlying type (`enum class Flags : unsigned { ... }`).

## Build & run
```sh
make run app=44-enums
make run app=44-enums STD=c++98   # `enum class` is C++11 — fails on C++98
```

## Try it yourself
Add a `turnRight(Direction)` function that returns the next clockwise direction
(North→East→South→West→North) using a `switch`.
