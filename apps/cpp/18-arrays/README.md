# 18-arrays

## Concept
An array is a fixed-size, contiguous run of values of the same type, accessed by
a zero-based index. This is the raw, C-style array — the low-level building
block. Lesson 19 shows `std::array`, the safer C++ wrapper around it.

## Minimal example
See `src/main.cpp`. The C version of this lesson lives in [`apps/c/18-arrays`](../../c/18-arrays/) (run it with `make run-c-app-17-arrays`).

## Line-by-line
- `int scores[5] = {10, 20, 30, 40, 50};` — five ints laid out back-to-back. The
  size `5` is part of the type.
- `scores[i]` — index access; valid indices are `0..4`.
- `sizeof(scores) / sizeof(scores[0])` — the classic way to compute the length,
  since a raw array doesn't store its own count.

## Common pitfalls
- **Out-of-bounds access** (`scores[5]`, `scores[-1]`) is undefined behavior —
  no automatic bounds check. AddressSanitizer (on in Debug) will catch it at
  runtime.
- **Array-to-pointer decay:** pass an array to a function and it becomes a bare
  pointer — the size is lost, and the `sizeof` trick breaks. This is *the*
  reason C++ prefers `std::array`/`std::vector`.
- Uninitialized array elements hold garbage; `int a[5] = {};` zeroes them all.

## C vs C++
Raw arrays are identical in C and C++ — C++ inherited them verbatim. The
difference is that C++ *adds better options on top* (`std::array`, next lesson).
In C, the raw array is all you get built in.

## Build & run
```sh
make run-cpp-app-18-arrays      # C++
make run-c-app-17-arrays    # C
```

## Try it yourself
Uncomment a `scores[5]` read and run the C++ version — watch AddressSanitizer
report the out-of-bounds access.
