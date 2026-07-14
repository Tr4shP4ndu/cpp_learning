# 36-debugging

## Concept
Debugging is narrowing down *where reality diverges from your mental model*. The
cheapest tools are built into the language: `assert()` checks a runtime
invariant and aborts (naming the failing expression) if it's false;
`static_assert` checks a condition at **compile time**; and printing values with
context ("trace" output) shows you the actual flow. Interactive debuggers (lldb,
gdb) and sanitizers go further when those aren't enough.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `static_assert(sizeof(int) >= 2, "...")` — evaluated by the compiler; if false
  the program **doesn't build**. Zero runtime cost.
- `assert(n >= 0 && "...")` — a runtime precondition. In a debug build a false
  condition aborts and prints the expression, file, and line. The `&& "message"`
  trick makes the abort message readable.
- A **release build** (`-DNDEBUG`) compiles `assert` out entirely — asserts guard
  against *bugs*, they are not input validation.
- The `[trace]` loop — printf-style debugging: show the input and the result so
  you can follow what actually happened.

## Why this matters
Most bugs are found by shrinking the gap between "what I think runs" and "what
runs". Asserts document and enforce your assumptions; traces reveal where they're
wrong — often faster than reaching for a debugger.

## Common pitfalls
- Using `assert` for user input or recoverable errors — it vanishes in release
  builds and aborts the whole process. Use real error handling (lesson 61) there.
- Side effects inside `assert`: `assert(pop() == x)` stops popping in a release
  build. Keep assert conditions pure.
- Leaving noisy trace prints in committed code — gate them behind a flag or
  remove them.

## Build & run
```sh
make run app=36-debugging
make run app=36-debugging BUILD_TYPE=Release   # NDEBUG: asserts compiled out
```
Uncomment the `factorial(-1)` line and run the debug build to watch the assert
fire and name the failing condition.

## Try it yourself
Add `assert(result > 0)` after the loop, then call `factorial(20)` and watch it
trip when the multiplication overflows `int` — a real bug the assert surfaces.
