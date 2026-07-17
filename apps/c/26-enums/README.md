# 26-enums (C)

An `enum` gives names to a set of integer constants. They auto-increment from 0
unless you assign explicit values, and the underlying type is plain `int`.

## Line-by-line
- `enum Day { MON, TUE, ... };` — `MON` is 0, `TUE` is 1, and so on.
- `enum Http { OK = 200, NOT_FOUND = 404, ... };` — set values explicitly.
- `enum Day today = WED;` — a variable of the enum type holds one of the names.
- `printf("%d", today)` — an enum value is an `int`, so `%d` prints it.
- `switch (today) { case SAT: ... }` — enum constants make readable switch labels.

## Common pitfalls
- Enum names are global constants: two enums cannot reuse the same name.
- The type is not restricted — nothing stops you assigning an out-of-range int.
- Auto-incrementing resumes from the last explicit value (e.g. `A=5, B` makes B 6).

## Compared to C++
C++ keeps this C-style `enum` but adds `enum class`, which is scoped
(`Color::Red`) and does not implicitly convert to `int`, avoiding name clashes.
See the [C++ version](../../cpp/44-enums/).

## Build & run
```sh
make run-c-app-26-enums
```
