# 18-arrays (C)

The C version of [`apps/cpp/18-arrays`](../../cpp/18-arrays/). Raw arrays are
native C — C++ inherited them unchanged.

## Line-by-line
- `int scores[5] = {...};` — a fixed-size, contiguous block; size is part of the
  type.
- `scores[i]` — zero-based indexing; valid indices `0..4`.
- `sizeof(scores) / sizeof(scores[0])` — compute the length (a raw array doesn't
  store its own count).

## Common pitfalls
- **Out-of-bounds access is undefined behavior** — no bounds checking.
- **Array-to-pointer decay:** pass an array to a function and it becomes a bare
  pointer — the `sizeof` length trick then breaks (it measures the pointer).
  This is exactly why C++ adds `std::array`/`std::vector`.

## Compared to C++
C++ offers `std::array` (lesson 19) and `std::vector` (lesson 21) — safer,
size-aware wrappers. See the [C++ version](../../cpp/18-arrays/).

## Build & run
```sh
make run-c-app-18-arrays
```
