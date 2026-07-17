# 25-unions (C)

A `union` overlays all its members in the same storage, so its size is that of
its largest member and only one member holds a valid value at a time. A *tagged
union* pairs a `union` with an `enum` tag so you always know which member is live.

## Line-by-line
- `union Number { int i; double d; };` — `i` and `d` share the same bytes.
- `n.i = 42;` then reading `n.i` is fine; `n.d = 3.14;` overwrites those bytes.
- `sizeof(union Number)` equals `sizeof(double)` here (the largest member).
- `struct Value { enum Kind kind; union Number data; };` — the tag plus the union.
- `{.i = 7}` / `{.d = 2.5}` — designated initializer picks which union member to set.
- `print_value` branches on `kind` so it only reads the member that is valid.

## Common pitfalls
- Reading a member you did not last write is undefined behavior in C.
- Forgetting the tag: without it you cannot tell which member is currently valid.
- Assuming all members are usable at once — a union is not a struct.

## Compared to C++
C++ has raw `union`s too, but prefers the type-safe `std::variant`, which tracks
the active alternative for you and throws (or is a compile error) on bad access —
no hand-written tag needed. See cpp/59-variant-tuple.

## Build & run
```sh
make run-c-app-25-unions
```
