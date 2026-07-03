# 14-while-loop

## Concept
A `while` loop repeats its body as long as a condition is true, testing the
condition *before* each pass. Reach for it when the number of iterations isn't
known in advance (read until end-of-file, retry until success, etc.).

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `while (i < 5)` — check the condition; if true, run the body, then re-check.
- `++i;` — the body must make progress toward making the condition false, or the
  loop never ends.

## `for` vs `while`
A `for` is just a `while` with the counter parts pulled to the top. Use `for`
when you're counting a known number of times; use `while` when you're looping
until some event.

## Common pitfalls
- **Infinite loop:** forgetting to update the variable the condition depends on.
- **Condition false at entry** → the body runs zero times (that's often
  correct — it's the difference from `do-while`, lesson 15).
- `while (n)` loops until `n` becomes 0 — handy, but be sure `n` is what you
  think.

## Build & run
```sh
make run app=14-while-loop
```

## Try it yourself
Count *down* from 5 to 1: start `int i = 5;` and loop `while (i > 0)` with
`--i`.
