# 21-pointer-arithmetic (C)

Adding to a pointer moves it by whole elements, not bytes: `p + 1` steps over
`sizeof(*p)` bytes. Arrays and pointers are deeply linked, which is why
indexing and pointer math are two views of the same thing.

## Line-by-line
- `int *base = arr;` — an array name *decays* to a pointer to its first element,
  so this is the same as `&arr[0]`.
- `arr[2]` is exactly `*(arr + 2)`, and `&arr[2]` is exactly `arr + 2`.
- `&arr[2] - arr` is `2` (an element count), not `8` — subtraction also scales.
- `base + 1` prints an address `sizeof(int)` bytes higher than `base`.
- `for (int *p = arr; p < arr + n; p++)` — walk the array with a moving pointer;
  `arr + n` is the one-past-the-end marker (legal to compare, not to deref).

## Common pitfalls
- Thinking `p + 1` adds one byte — it adds one *element*.
- Dereferencing `arr + n` (one past the end) — undefined behavior; ASan flags it.
- Confusing the element count from pointer subtraction with a byte count.

## Compared to C++
Identical mechanics in C++, but you'd usually iterate with `std::vector` and
range-`for` or iterators rather than raw pointer math. See the
[C++ version](../../cpp/39-pointers/).

## Build & run
```sh
make run-c-app-21-pointer-arithmetic
```
