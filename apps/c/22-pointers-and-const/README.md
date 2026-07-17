# 22-pointers-and-const (C)

`const` freezes different things depending on which side of the `*` it sits on.
Read the declaration right-to-left. A pointer-to-pointer (`int **`) adds one
more level of indirection, used for out-parameters and arrays of strings.

## Line-by-line
- `const int *p` — *pointer to const int*: you cannot write `*p`, but you can
  re-point `p` (here `p = &b;`).
- `int *const q` — *const pointer to int*: you cannot re-point `q`, but you can
  write through it (`*q = 42;`).
- `int **out` — a pointer to a pointer; `*out = arr;` sets the caller's pointer,
  so `pick(&chosen, nums)` fills `chosen` (a classic out-parameter).
- `const char **w = words;` — the same `char**` shape as `argv`: a pointer to
  the first of several string pointers.

## Common pitfalls
- Mixing up the two `const` positions (right-to-left reading resolves it).
- Passing `&chosen` vs `chosen` to an out-parameter — you need the address of
  the pointer so the callee can modify it.
- Forgetting the extra `*` / `&` level when working with `char **`.

## Compared to C++
C++ has the identical `const`-pointer rules, but usually replaces `int**`
out-parameters with references (see cpp/38-references) — cleaner call sites and
no `&` at the call. See the [C++ version](../../cpp/39-pointers/).

## Build & run
```sh
make run-c-app-22-pointers-and-const
```
