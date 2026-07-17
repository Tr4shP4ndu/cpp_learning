# 57-variant-tuple

## Concept
Two C++17 vocabulary types for grouping values:
- `std::tuple` — a fixed-size bundle of values of **different** types (like a
  struct with unnamed fields). Good for returning several values at once.
- `std::variant` — a **type-safe union**: it holds exactly one of a fixed set of
  types and always knows which. It replaces raw `union` + a manual tag.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `std::tuple<int, std::string, double> row{...}` — three values of different
  types in one object.
- `auto [id, name, score] = row;` — **structured bindings** unpack the tuple into
  named locals; far clearer than `std::get` everywhere.
- `std::get<1>(row)` — access a tuple element by index (also by type when that
  type appears once).
- `std::variant<int, std::string> value = 42;` — currently holds an `int`.
- `std::holds_alternative<int>(value)` — ask which type is active (prints `1`).
- After `value = std::string{...}` it holds a string; `std::visit(lambda, value)`
  calls the lambda with whatever type is currently stored — the type-safe way to
  handle every case.

## Why this matters
`variant` + `visit` gives you closed, exhaustive alternatives with no inheritance
and no `void*`. `tuple` + structured bindings makes multi-value returns readable.
Both show up throughout modern C++ (and `std::optional`/`std::expected` in
lesson 59 are close cousins).

## Common pitfalls
- Reading the wrong alternative: `std::get<int>(value)` when it holds a string
  **throws** `std::bad_variant_access`. Check first, or use `std::visit`.
- A default-constructed `variant` holds its **first** alternative — order matters.
- Overusing `tuple` for data that deserves named fields — a small `struct` is
  often clearer than `std::get<2>`.

## Build & run
```sh
make run-cpp-app-57-variant-tuple
make build-cpp-app-57-variant-tuple STD=c++14   # variant/tuple bindings need C++17 — fails
```

## Try it yourself
Add a `double` alternative to the variant, assign one, and extend the `std::visit`
lambda (it already handles any type via `auto`) — then try `std::get<int>` on the
string state and observe the thrown exception.
