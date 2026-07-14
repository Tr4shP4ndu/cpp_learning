# 39-pointers

## Concept
A pointer is a variable whose value is a **memory address** — it "points at"
another object. Pointers are the low-level tool behind arrays, dynamic memory,
and data structures. This lesson covers what a pointer *is*; lesson 40 focuses
on using the `*` operator to read/write through it.

## Minimal example
See `src/main.cpp`. The C version of this lesson lives in [`apps/c/39-pointers`](../../c/39-pointers/) (run it with `make run-c app=39-pointers`).

## Line-by-line
- `int* ptr = &value;` — `&value` is the **address-of** operator; `int*` is the
  type "pointer to int". `ptr` now holds where `value` lives.
- Printing `ptr` shows an address (e.g. `0x16d…`), the same as `&value`.
- `*ptr` **dereferences** — follows the address to the value (lesson 40).
- `int* nothing = nullptr;` — a pointer that points at nothing. Always check for
  null before dereferencing.
- `ptr = &other;` — a pointer can be re-pointed; a reference (lesson 38) cannot.

## Pointer vs reference
- A **reference** must always alias a valid object and can't be re-seated — safer
  when that's all you need.
- A **pointer** can be null and can be reassigned — use it when you need "maybe
  nothing" or to walk through memory.

## Common pitfalls
- **Dereferencing null or uninitialized pointers** is undefined behavior (a
  crash, if you're lucky; the sanitizer flags it). Initialize to `nullptr` and
  check.
- A **dangling pointer** points at memory that's already been freed or gone out
  of scope. Using it is UB.
- The `*` symbol is overloaded: in a *declaration* (`int* p`) it means "pointer";
  in an *expression* (`*p`) it means "dereference".

## C vs C++
Pointer syntax is identical in C and C++. The only cosmetic difference: C uses
`NULL`, modern C++ uses `nullptr` (type-safe). C++ also gives you *smart
pointers* (lesson 42) so you rarely handle raw owning pointers by hand.

## Build & run
```sh
make run app=39-pointers      # C++
make run-c app=39-pointers    # C
```

## Try it yourself
Make a `int** pp = &ptr;` (a pointer to a pointer) and print `**pp`. This is how
multi-level indirection works.
