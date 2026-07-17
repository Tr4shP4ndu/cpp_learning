# 15-do-while-loop

## Concept
A `do { } while (cond);` loop runs its body first, *then* checks the condition.
The body therefore always runs **at least once** — the key difference from a
plain `while`.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `do { ... }` — run the body.
- `while (i < 5);` — check afterward; if true, loop again. **The trailing
  semicolon is required.**
- The second loop shows the defining trait: its condition (`100 < 5`) is false,
  yet the body still runs once.

## When to use it
Menus, prompts, and retry logic — anything that must happen once before you can
even evaluate whether to repeat ("show the menu, then decide whether to show it
again").

## Common pitfalls
- **Forgetting the semicolon** after `while (...)` is a compile error unique to
  this form.
- Using `do-while` when the body should be skippable — if zero iterations is
  valid, use a plain `while`.

## Build & run
```sh
make run-cpp-app-15-do-while-loop
```

## Try it yourself
Turn it into an input loop: read a number and repeat "until the user enters 0"
(combine with lesson 05's `std::cin`).
