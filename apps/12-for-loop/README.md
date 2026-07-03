# 10-for-loop

A for loop repeats a block a known number of times. It has three parts:
- Initialization: runs once at the start
- Condition: checked before each iteration
- Increment/decrement: runs after each iteration

## Basic syntax

Counts from 0 to 4:

for (int i = 0; i < 5; ++i) { /* body */ }

Notes:
- Prefer `++i` in loops; same behavior here and avoids unnecessary temporaries.
- Use `size_t` for indexing containers.

## Iterating arrays and vectors

Raw array:
- `int a[3] = {1, 2, 3};`
- `for (size_t i = 0; i < 3; ++i) { /* a[i] */ }`

`std::vector`:
- `std::vector<int> v = {1, 2, 3};`
- `for (size_t i = 0; i < v.size(); ++i) { /* v[i] */ }`

## Range-based for

Cleaner iteration when you don’t need the index:
- `for (const auto& x : v) { /* use x */ }`

Use references to avoid copies:
- `auto& x` for modification
- `const auto& x` for read-only

## Control flow: break and continue

- `break`: exit the loop early
- `continue`: skip to the next iteration

## Common pitfalls

- Off-by-one errors: check `<` vs `<=` carefully
- Out-of-bounds: ensure index stays within size
- Modifying a container while iterating can invalidate iterators

## Examples

Sum of elements (range-based loop):
- `int sum = 0;`
- `for (const auto& x : v) sum += x;`

Find first even (indexed loop):
- `int found = -1;`
- `for (size_t i = 0; i < v.size(); ++i) { if (v[i] % 2 == 0) { found = i; break; } }`

## When to use which

- Classic `for`: when you need the index or a custom step
- Range-based `for`: when you only need the element
- Algorithms (`std::for_each`, `<ranges>`): prefer when possible for clarity and safety
## Build and run (from repository root)

Run these from the repository root:
  - make build app=10-for-loop
  - make run app=10-for-loop

Binary path: build/10-for-loop/bin/10-for-loop

Alternative (from inside this folder):
  - cd app/10-for-loop
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
