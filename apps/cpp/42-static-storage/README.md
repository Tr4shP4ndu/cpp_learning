# 42-static-storage

## Concept
`static` is one keyword with a few related meanings, all about **lifetime and
sharing** rather than a single call or a single object:
- a **static local** lives for the whole program, initialized once, keeping its
  value between calls;
- a **static member** is shared by every instance of a class — one variable, not
  one per object.

It's the natural place to pin down the three storage regions: **automatic**
(stack), **dynamic** (heap), and **static**.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `static int counter = 0;` inside `next_id()` — a static local: initialized the
  first time control reaches it, then it persists, so successive calls return
  1, 2, 3.
- `static int living;` in `Widget` — a static member: declared in the class...
- `int Widget::living = 0;` — ...and **defined once** outside it (a static data
  member is a single shared object that needs storage somewhere).
- `Widget::living` — accessed on the class, not an instance.
- `int stack_var` (automatic/stack) vs `new int` (dynamic/heap, lesson 40) —
  the two lifetimes you already know, contrasted with static.

## The three storage durations
| Where | Lifetime | Example |
|-------|----------|---------|
| **Automatic** (stack) | until the enclosing scope ends | `int x;` |
| **Dynamic** (heap) | until you `delete` (or a smart pointer does) | `new int` |
| **Static** | whole program run | `static int`, globals, static members |

## Common pitfalls
- Forgetting the out-of-line definition of a static member → a linker error
  ("undefined reference"). (C++17 `inline static` avoids this.)
- Static locals initialize on first use, which is **not thread-safe to rely on**
  before C++11; since C++11 the initialization itself is thread-safe, but the
  shared state still needs synchronization (lesson 62).
- Hidden shared mutable state via statics makes code hard to test — use
  deliberately.

## Build & run
```sh
make run app=42-static-storage
```

## Try it yourself
Add a `static int` counter to `Widget`'s constructor that only ever increments
(total ever created) and print it alongside `living` — note how one goes down
on destruction and the other never does.
