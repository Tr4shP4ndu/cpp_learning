# 08-switch

## Concept
A `switch` compares one integer-like value against many constant `case` labels
and jumps to the match. It's a clean alternative to a long `if/else if` chain
when you're testing the *same* value repeatedly.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `switch (day)` — evaluate `day` once, then jump to the matching `case`.
- Stacked labels (`case 1: case 2: ...`) share one body — a legitimate,
  intentional fall-through.
- `break;` — leave the switch. **Without it, execution falls through** into the
  next case's code.
- `[[fallthrough]];` — tells the compiler "yes, I meant to fall through here",
  suppressing the warning.
- `default:` — runs when no case matches (like the final `else`).

## Common pitfalls
- **Forgetting `break`** is the classic bug: you accidentally run the next
  case's code too.
- `switch` only works on integral/enum values (and `char`), **not** on `float`
  or `std::string`. Use `if/else` for those.
- Declaring a variable inside a `case` without its own `{ }` block can cause a
  "jump bypasses initialization" error — wrap the case body in braces.

## Build & run
```sh
make run-cpp-app-08-switch
```

## Try it yourself
Change `day` to `6` and to `8`, and watch which lines print. Trace why `6`
prints two lines.
