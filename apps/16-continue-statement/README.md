# 16-continue-statement

The `continue` statement skips the rest of the current loop body and proceeds directly to the next iteration (re-evaluating the loop condition as appropriate).

## When to use
- Skip processing for elements that don’t meet a condition (e.g., filter odd/even)
- Ignore invalid inputs and move on
- Keep nesting shallow by early-skipping rather than piling up `if` blocks

## Examples

Skip odd numbers in a classic `for` loop:
```cpp
for (int i = 0; i < 10; ++i) {
  if (i % 2 != 0) continue; // skip odds
  // only evens reach here
}
```

Process only positive values in a range-based loop:
```cpp
for (const auto& x : values) {
  if (x <= 0) continue;
  // process positive x
}
```

In `while` loops, ensure the state moves forward to avoid infinite loops:
```cpp
int i = 0;
while (i < 10) {
  ++i;              // advance first to avoid stalling
  if (i % 3 == 0) continue;
  // work with i when not divisible by 3
}
```

## Pitfalls
- Overuse can harm readability; consider restructuring logic or using algorithms.
- In `while`/`do-while`, forgetting to update loop variables before `continue` can create infinite loops.
- `continue` affects only the nearest loop.

## Build and run (from repository root)

Run these from the repository root:
  - make build app=16-continue-statement
  - make run app=16-continue-statement

Binary path: build/16-continue-statement/bin/16-continue-statement

Alternative (from inside this folder):
  - cd app/16-continue-statement
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
