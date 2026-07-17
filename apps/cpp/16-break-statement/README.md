# 16-break-statement

## Concept
`break` immediately exits the innermost loop (or `switch`), skipping any
remaining iterations. It's how you stop as soon as you've found what you needed.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- The loop scans `data` in order.
- `if (value >= 30) { ... break; }` — once the match is found, `break` leaves
  the loop entirely; the values after 30 are never visited.
- Execution continues at the first statement *after* the loop.

## Common pitfalls
- `break` only exits **one** loop. In nested loops it leaves the inner one; the
  outer keeps going. (Options: a flag, or factor the loop into a function and
  `return`.)
- Inside a `switch`, `break` ends the `switch`, not an enclosing loop.
- Don't confuse `break` (exit the loop) with `continue` (skip to next iteration
  — lesson 17).

## Build & run
```sh
make run-cpp-app-16-break-statement
```

## Try it yourself
Change the test to `value == 99` (not present) and confirm the loop runs to
completion without breaking.
