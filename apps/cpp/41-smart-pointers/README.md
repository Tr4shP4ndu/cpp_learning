# 42-smart-pointers

## Concept
Smart pointers are objects that own a heap allocation and **free it
automatically** when they go out of scope. They turn the error-prone
manual `new`/`delete` of lesson 40 into something you can't forget — this is
RAII (Resource Acquisition Is Initialization) applied to memory. In modern C++,
prefer them over raw owning pointers.

## Minimal example
See `src/main.cpp`.

## The two you need
- **`std::unique_ptr<T>`** — a *single* owner. Cannot be copied (that would mean
  two owners); ownership is **moved** with `std::move`. Zero overhead over a raw
  pointer. Your default.
- **`std::shared_ptr<T>`** — *shared* ownership via a reference count. Copies
  bump the count; the object is freed when the **last** owner is destroyed. Use
  only when ownership genuinely is shared.

## Line-by-line
- `std::make_unique<int>(42)` — allocate + wrap in one step. Prefer it over
  `std::unique_ptr<int>(new int(42))` (exception-safe, less typing).
- `*a` — dereference like a normal pointer.
- `std::unique_ptr<int> b = std::move(a);` — transfer ownership; `a` becomes
  null afterward.
- `auto s2 = s1;` — copying a `shared_ptr` adds an owner; `s1.use_count()` is 2.
- No `delete` anywhere — each smart pointer releases on scope exit.

## Common pitfalls
- **Trying to copy a `unique_ptr`** is a compile error — that's the design.
  `std::move` it instead.
- **Overusing `shared_ptr`:** the reference counting isn't free, and cyclic
  `shared_ptr`s leak (break cycles with `std::weak_ptr`). Default to
  `unique_ptr`.
- Don't wrap the same raw pointer in two smart pointers — double free.

## Build & run
```sh
make run app=42-smart-pointers
```
`std::make_unique` was standardized in C++14 (`make_shared` in C++11). Some
standard libraries provide `make_unique` earlier as an extension, so building
with `STD=c++11` may or may not fail depending on your compiler — either way,
use C++14+ to be safe.

## Try it yourself
Wrap a small `struct Point { int x, y; };` in a `std::unique_ptr` via
`make_unique<Point>(1, 2)` and access `p->x`. Notice you never call `delete`.
