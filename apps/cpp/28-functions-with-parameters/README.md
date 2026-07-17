# 28-functions-with-parameters

## Concept
Parameters feed data into a function. *How* you declare them decides whether the
function gets a copy, a read-only view, or the power to modify the caller's
variable. Getting this right is central to writing efficient, correct C++.

## Minimal example
See `src/main.cpp`.

## The three ways to take a parameter
- **By value** (`int x`) — the function gets its own **copy**. Changes don't
  affect the caller. Ideal for small, cheap types (`int`, `double`, `char`).
- **By const reference** (`const std::string& text`) — the function sees the
  original **without copying** and promises not to change it. The default for
  strings, vectors, and other non-trivial objects.
- **By (non-const) reference** (`int& n`) — the function can **modify** the
  caller's variable. Use when the whole point is to change the input.

## Line-by-line
- `multiply(int x, int y)` — small ints, so by value is fine.
- `lengthOf(const std::string& text)` — avoids copying a whole string just to
  read its length.
- `doubleInPlace(int& n)` — `n` *is* the caller's `value`; `n *= 2` changes it.

## Common pitfalls
- **Copying big objects by value** in hot paths is a classic performance bug.
  Reach for `const&`.
- A non-const reference parameter silently lets a function mutate your variable —
  make that intent obvious at the call site or prefer returning a new value.
- You can't bind a non-const reference to a temporary/literal
  (`doubleInPlace(5)` won't compile) — that's the type system protecting you.

## Build & run
```sh
make run-cpp-app-28-functions-with-parameters
```

## Try it yourself
Add `void appendBang(std::string& s)` that does `s += "!";` and call it on a
string in `main`. Confirm the caller's string changed.
