# 12-for-loop

## Concept
A `for` loop repeats a block a controlled number of times. It bundles the three
parts of counting — start, test, step — into one line, which keeps the loop's
bounds visible in one place.

## Minimal example
See `src/main.cpp`.

## Line-by-line
`for (init; condition; step) { body }`
- **init** (`std::size_t i = 0`) runs once before the loop.
- **condition** (`i < values.size()`) is checked *before* each pass; when false,
  the loop ends.
- **step** (`++i`) runs *after* each pass.
- The body runs once per pass while the condition holds.

## Common pitfalls
- **Sign mismatch.** `.size()` returns an unsigned `std::size_t`. Comparing it to
  a signed `int i` triggers a `-Wsign-compare` warning; declare `i` as
  `std::size_t` (as here).
- **Off-by-one.** `i <= size()` runs one time too many and reads past the end.
  Use `<` with a zero-based index.
- `++i` vs `i++` as the step behaves identically here; `++i` is conventional.

## Build & run
```sh
make run-cpp-app-12-for-loop
```

## Try it yourself
Loop *backwards*. With an unsigned counter this is tricky (`i >= 0` is always
true), so use a signed index: `for (int i = 2; i >= 0; --i)`.
