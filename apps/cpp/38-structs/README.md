# 38-structs

## Concept
A `struct` groups related values into one named type — a "point" is an `x` and a
`y`; a "person" is a name and an age. It's the first step from loose variables
toward objects, and the gateway to classes (lesson 40).

## Minimal example
See `src/main.cpp` (C++) and `src/main.c` (C).

## Line-by-line
- `struct Point { double x; double y; };` — declare a type with two fields.
- `Point p{3.0, 4.0};` — aggregate initialization fills fields in order.
- `p.x`, `alice.age` — the `.` operator accesses a member.
- Fields can be read and written like any variable.

## C vs C++ — the key divergence
| | C | C++ |
|---|---|---|
| Holds | data only | data **and** functions |
| Default access | (n/a) | members are **public** |
| Naming | `struct Point p;` or a typedef | just `Point p;` |

In C a struct is a pure data record. In C++ a `struct` is actually a **class**
whose members default to public — it can have member functions, constructors,
and everything a `class` has (lesson 40). Convention: use `struct` for plain
data bundles, `class` when you add behavior and want members private by default.

## Common pitfalls
- **C:** forgetting the `struct` keyword (`struct Point p;`), or the `char[]`
  size for string fields.
- Aggregate init order matters — `{3.0, 4.0}` sets `x` then `y`.
- Copying a struct copies every field (value semantics, lesson 32).

## Build & run
```sh
make run app=38-structs      # C++
make run-c app=38-structs    # C
```

## Try it yourself
Add a `struct Rectangle { Point topLeft; Point bottomRight; };` — structs can
nest. Print one of its corner coordinates.
