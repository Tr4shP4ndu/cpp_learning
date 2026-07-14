# 20-std-fill

## Concept
`std::fill` sets every element in a range to the same value. It's your first
*standard algorithm* — a reusable function from `<algorithm>` that works on any
container via iterators, so you don't hand-write the loop.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `std::array<int, 5> numbers{};` — the `{}` zero-initializes; important, because
  `std::fill` will overwrite anyway, but leaving it uninitialized first is a
  habit worth breaking.
- `std::fill(numbers.begin(), numbers.end(), 42);` — the pair
  `(begin, end)` describes the range `[begin, end)` (end is one-past-the-last).
  Every element in it becomes `42`.

## Why algorithms over loops
- Intent is obvious: "fill with 42" beats a three-line loop.
- Works uniformly on `std::array`, `std::vector`, and more.
- Related: `std::iota` (increasing values, lesson 19), `std::copy`,
  `std::for_each`, `std::transform`, `std::accumulate`. The half-open
  `[begin, end)` iterator range is the shared vocabulary of all of them
  (lesson 56 goes deeper with ranges).

## Common pitfalls
- The range is **half-open**: `end()` points *past* the last element, never at
  a real one.
- Passing mismatched iterators (from two different containers) is undefined
  behavior.

## Build & run
```sh
make run app=20-std-fill
```

## Try it yourself
Fill only the first three elements: `std::fill(numbers.begin(),
numbers.begin() + 3, 7);` and print the result.
