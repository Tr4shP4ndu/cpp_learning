# 51-casting

## Concept
A cast converts one type to another. C has one blunt tool — `(int)x` — that does
whatever it takes, silently. C++ splits that into **four named casts** so the
code says *which kind* of conversion you mean, and so the compiler can reject the
ones that don't make sense: `static_cast`, `dynamic_cast`, `const_cast`, and
`reinterpret_cast`.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `static_cast<int>(pi)` — ordinary, well-defined conversions (numeric
  narrowing, up-casts, `void*` back to a known type). Checked at compile time.
- `dynamic_cast<Dog*>(a)` — a **runtime-checked** down-cast within a polymorphic
  hierarchy (needs a `virtual` function). Returns `nullptr` when the object
  isn't really the target type — see the `&base` case.
- `const_cast<int*>(cp)` — the only cast that adds or removes `const`. Reading is
  fine; *writing* through it is UB unless the object was non-const to begin with.
- `reinterpret_cast<unsigned char*>(&value)` — reinterpret the raw bits (here,
  view an `int` byte by byte). No conversion happens; you're relabeling memory.
  Powerful, rarely correct, easy to misuse.

## Why this matters
Named casts are **greppable** and **intentional**: `dynamic_cast` says "I'm not
sure of the type," `const_cast` flags the one place const is being subverted. A
C-style `(T)x` hides which of these it's really doing.

## Common pitfalls
- `dynamic_cast` needs a **polymorphic** type (at least one `virtual` function,
  e.g. the virtual destructor here) or it won't compile.
- Writing through a `const_cast` to a genuinely `const` object is undefined
  behavior — `const_cast` is for interfacing with legacy APIs, not defeating const.
- `reinterpret_cast` results are largely implementation-defined; reach for it
  only for low-level byte work, and prefer `std::bit_cast` (C++20) for value
  reinterpretation.

## Build & run
```sh
make run app=51-casting
```

## Try it yourself
Add a second derived class `Cat : Animal`, store a `Cat*` in an `Animal*`, and
show that `dynamic_cast<Dog*>` returns `nullptr` while `dynamic_cast<Cat*>`
succeeds.
