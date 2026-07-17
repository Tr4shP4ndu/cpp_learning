# 24-structs (C)

The C version of [`apps/cpp/43-structs`](../../cpp/43-structs/). This is where C
and C++ start to diverge.

## Line-by-line
- `struct Point { double x, y; };` — in C a struct holds **data only** (no member
  functions).
- `struct Point p = {3.0, 4.0};` — note the `struct` keyword is required in C
  when naming the type...
- `typedef struct { ... } Person;` — ...unless you `typedef` it, letting you
  write `Person` directly.
- `p.x`, `alice.age` — access fields with `.`.

## Compared to C++
A C++ `struct` is a full class: it can have member functions, constructors, and
access control (members default to public). Convention: `struct` for plain data,
`class` for data + behavior. See the [C++ version](../../cpp/43-structs/) and,
for behavior, lesson 45 (classes).

## Build & run
```sh
make run-c-app-24-structs
```
