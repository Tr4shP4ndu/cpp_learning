# 19-std-array

## Concept
`std::array<T, N>` is a thin, zero-overhead wrapper around a raw array. Same
fixed size and same memory layout, but it's a proper object: it knows its own
length, can be copied and returned from functions, and offers bounds-checked
access. In C++, prefer it over raw arrays. It's also our first look at
**standard algorithms** — `std::iota` and `std::fill` — reusable functions that
operate on a range instead of a hand-written loop.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `std::array<int, 5> ids{};` — type is element type + compile-time size. The
  `{}` value-initializes all elements to 0.
- `std::iota(ids.begin(), ids.end(), 0);` — fill with 0, 1, 2, … (from
  `<numeric>`). The pair `(begin, end)` describes the half-open range
  `[begin, end)` — `end()` points *one past* the last element.
- `ids.size()` — the length, which a raw array can't tell you.
- `ids.at(4) = 42;` — **bounds-checked** access: an invalid index throws
  `std::out_of_range` instead of silently corrupting memory.
- `ids[i]` — the fast, *unchecked* form (same as a raw array).
- `std::fill(flags.begin(), flags.end(), 1);` — set every element in the range
  to one value (from `<algorithm>`). "fill with 1" beats a three-line loop.

## Why algorithms over loops
- Intent is obvious: `std::fill(..., 42)` reads as "fill with 42".
- The same `[begin, end)` vocabulary drives `std::copy`, `std::for_each`,
  `std::transform`, `std::accumulate`, and the rest — learn the range, get the
  whole library (lesson 58 goes deeper with ranges).

## Common pitfalls
- `operator[]` does **not** bounds-check (for speed). Use `.at()` when the index
  might be invalid; `[]` when you've already guaranteed it.
- The size `N` is part of the type: `std::array<int,3>` and `std::array<int,4>`
  are different types. For a run-time-sized sequence, use `std::vector`
  (lesson 21).
- The range is **half-open**: `end()` points *past* the last element. Passing
  mismatched iterators (from two different containers) is undefined behavior.

## Build & run
```sh
make run app=19-std-array
make run app=19-std-array STD=c++03  # std::array is C++11 — fails on older
```

## Try it yourself
Fill only the first three elements: `std::fill(flags.begin(), flags.begin() + 3,
7);` and print the result. Then change `ids.at(4)` to `ids.at(99)` and watch it
throw `std::out_of_range`.
