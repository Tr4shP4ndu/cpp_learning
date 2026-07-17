# 22-vectors

## Concept
`std::vector<T>` is the workhorse container: a dynamic array that grows and
shrinks at run time and manages its own heap memory. It's what you reach for
whenever you need "a list of things" and don't know the count up front. It's the
run-time-sized counterpart to the fixed `std::array` (lesson 19).

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `std::vector<int> nums{10, 20, 30};` — create with initial elements.
- `nums.push_back(40);` — append; the vector reallocates automatically if it
  runs out of capacity.
- `nums.size()`, `nums.front()`, `nums.back()` — count and end elements.
- `nums.pop_back();` — remove the last element.
- `nums.at(1)` — bounds-checked access; `nums[1]` is the unchecked, faster form.

## How it grows
A vector keeps a *capacity* ≥ its *size*. When you `push_back` past capacity it
allocates a bigger block (typically 2×) and moves the elements. Amortized, each
push is O(1). If you know the final size, `nums.reserve(n)` avoids repeated
reallocations.

## Common pitfalls
- **Iterator/reference invalidation:** a `push_back` that reallocates invalidates
  any pointers, references, or iterators into the vector. Don't hold them across
  a modification.
- `[]` past the end is undefined behavior; use `.at()` when unsure.
- Copying a large vector copies every element — pass by `const&` to functions.

## Build & run
```sh
make run-cpp-app-22-vectors
make run-cpp-app-22-vectors STD=c++03   # brace-init list is C++11 — fails on older
```

## Try it yourself
Build a vector by looping `push_back(i * i)` for `i` in `0..9`, then print it
with a range-for (lesson 13).
