# 39-pointers (C)

The C version of [`apps/cpp/39-pointers`](../../cpp/39-pointers/). Pointers are
native C; the syntax is identical to C++.

## Line-by-line
- `int *ptr = &value;` — `&value` is the address; `int*` is "pointer to int".
- `%p` prints a pointer; `*ptr` dereferences (reads the pointed-to value).
- `int *nothing = NULL;` — C uses `NULL` where C++ prefers `nullptr`.
- `ptr = &other;` — a pointer can be re-pointed.

## Common pitfalls
- Dereferencing a `NULL` or uninitialized pointer is undefined behavior.
- Dangling pointers (pointing at freed/out-of-scope memory).

## Compared to C++
Identical mechanics; C++ adds `nullptr` (type-safe) and smart pointers
(lesson 41) so you rarely manage raw owning pointers. See the
[C++ version](../../cpp/39-pointers/).

## Build & run
```sh
make run-c-app-39-pointers
```
