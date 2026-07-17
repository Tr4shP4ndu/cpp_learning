# 25-iterators

## Concept
An **iterator** is a generalized pointer: an object that points at an element of
a container and can move to the next one. Every STL container hands out
iterators via `begin()` and `end()`, and every STL algorithm is written in terms
of them. Understanding iterators is what turns "containers" and "algorithms"
into one connected system.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `v.begin()` — an iterator to the first element; `v.end()` — one **past** the
  last. The range `[begin, end)` is half-open, so `end()` is never dereferenced.
- `*it` — dereference, exactly like a pointer, to get the element.
- `++it` — advance to the next element.
- `for (int x : v)` — the range-based for loop is pure sugar over
  `begin()`/`end()`; now you can see what it expands to.
- `std::find(v.begin(), v.end(), 30)` — algorithms consume an iterator range and
  return an iterator; comparing it to `end()` tells you "not found".
- `found - v.begin()` — random-access iterators (like a vector's) support
  arithmetic, so subtracting gives the index.
- `v.rbegin()/v.rend()` — reverse iterators walk back to front.

## Iterator categories
Not all iterators can do everything. From weakest to strongest: **input/output**
(single pass) → **forward** → **bidirectional** (`std::list`, can `--`) →
**random-access** (`std::vector`, `std::array`, supports `it + n`). An algorithm
requires a minimum category; that's why `std::sort` needs random access but
`std::find` works on anything forward.

## Common pitfalls
- **Iterator invalidation:** modifying a container (e.g. `push_back` that
  reallocates a vector) can invalidate existing iterators — using them
  afterward is undefined behavior.
- Dereferencing `end()` is UB — it's a "past the last" marker, not an element.
- Mixing iterators from two different containers is UB.

## Build & run
```sh
make run-cpp-app-25-iterators
```

## Try it yourself
Erase every element equal to 20 using the erase–remove idiom:
`v.erase(std::remove(v.begin(), v.end(), 20), v.end());` and print the result.
Then note how any iterator you held before the erase is now invalid.
