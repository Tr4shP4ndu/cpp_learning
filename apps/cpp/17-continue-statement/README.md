# 17-continue-statement

## Concept
`continue` skips the rest of the current loop iteration and jumps to the next
one. Use it to filter — "for the items I don't care about, skip ahead" — instead
of wrapping the whole body in an `if`.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `if (i % 2 == 0) continue;` — for even `i`, skip the print and go straight to
  the loop's step.
- In a `for` loop, `continue` runs the **step** (`++i`) before re-testing the
  condition. (In a `while`, it jumps straight back to the condition — so make
  sure the update still happens, or you can loop forever.)

## `continue` vs `break`
- `continue` — skip *this* pass, keep looping.
- `break` — stop the loop entirely (lesson 16).

## Common pitfalls
- In a `while`/`do-while`, a `continue` that skips the variable update creates an
  infinite loop. Update before the `continue`, or restructure.
- Overusing `continue` can hurt readability; sometimes a positive `if` around the
  body is clearer.

## Build & run
```sh
make run-cpp-app-17-continue-statement
```

## Try it yourself
Flip the condition to `i % 2 != 0` to print the *even* numbers instead.
