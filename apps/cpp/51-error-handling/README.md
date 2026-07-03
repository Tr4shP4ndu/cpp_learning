# 51-error-handling

## Concept
Functions fail. C++ gives you three modern tools, each for a different kind of
failure: `std::optional` (maybe no value), `std::expected` (a value *or* an
error reason), and exceptions (throw for truly exceptional situations). Picking
the right one — and using it consistently — is a mark of solid C++.

## Minimal example
See `src/main.cpp`.

## The three tools
- **`std::optional<T>`** (C++17) — "a `T`, or nothing." Use when absence is
  normal, not an error (no match found, field not set).
  - `return std::nullopt;` for empty; `if (opt)` / `*opt` to use it.
- **`std::expected<T, E>`** (C++23) — "a `T`, or an error `E` explaining why."
  Use for recoverable failures where the caller wants the reason.
  - `return std::unexpected(err);` for failure; `if (exp)` / `*exp` /
    `exp.error()`.
- **Exceptions** (`throw` / `try` / `catch`) — for exceptional, hard-to-handle-
  locally failures. They unwind the stack until a matching `catch`, running
  destructors (RAII, lesson 41) along the way.

## Line-by-line
- `firstDigit` returns `optional<int>` — a digit, or `nullopt`.
- `safeDivide` returns `expected<double, std::string>` — the quotient, or
  `unexpected("division by zero")`.
- `parsePositive` `throw`s `std::invalid_argument`; `main` catches it as
  `std::exception&` and reads `e.what()`.

## Choosing
- No value is a normal outcome → **optional**.
- Caller needs to know *why* it failed and can react → **expected**.
- Failure is rare/exceptional and would clutter every call site → **exception**.
- **Don't mix inconsistently.** Pick per API and stick to it.

## Common pitfalls
- Dereferencing an empty `optional`/`expected` (`*opt` when empty) is undefined
  behavior — check first, or use `.value()` (which throws) / `.value_or(x)`.
- Exceptions have a cost on the throwing path and need exception-safe code;
  don't use them for ordinary control flow.
- `std::expected` is **C++23** (this repo defaults to C++23).

## Build it step by step
1. **optional.** Write `firstDigit` returning `std::optional<int>`; in `main`
   handle both the "found" and "nothing" cases.
2. **expected.** Add `safeDivide` returning `std::expected<double, std::string>`;
   handle the value and the `.error()` separately.
3. **exceptions.** Add `parsePositive` that `throw`s, and wrap the call in
   `try { ... } catch (const std::exception& e) { ... }`.

## Build & run
```sh
make run app=51-error-handling
```

## Try it yourself
Change `safeDivide` to also reject producing infinity, returning a different
error string, and handle both error cases in `main`.
