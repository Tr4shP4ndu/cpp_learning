# 46-constructors-destructors

## Concept
A **constructor** runs automatically when an object is created — it sets up the
object's initial state and acquires any resources. A **destructor** runs
automatically when the object's lifetime ends — it releases those resources.
Tying a resource's lifetime to an object's is the single most important C++
idiom: **RAII** (Resource Acquisition Is Initialization).

## Files
```
46-constructors-destructors/
  include/buffer.hpp   <- class owning a heap buffer
  src/buffer.cpp       <- constructor acquires, destructor releases
  src/main.cpp         <- watch construction/destruction happen with scope
```

## Line-by-line
- `Buffer(std::size_t size);` — the constructor; `~Buffer();` — the destructor
  (same name with `~`).
- `: size_(size), data_(new int[size]{})` — the **member initializer list**
  initializes members *before* the constructor body. Prefer it over assigning in
  the body.
- The destructor `delete[] data_;` — releases what the constructor allocated.
- In `main`, the inner `{ }` block bounds the object's life: the constructor
  prints when `b` is created, the destructor prints the moment control leaves
  the block. You never call `delete` — the language does it for you.

## Why RAII matters
Because the destructor runs on *every* exit path — normal return, early return,
even a thrown exception — resources can't leak. This is why C++ file streams
(lesson 24) and smart pointers (lesson 42) clean up automatically: they're RAII
types.

## Common pitfalls
- **The rule of three/five:** a class that manages a raw resource needs to handle
  copying/moving too, or a copy will double-free. Here we `= delete` the copy
  operations to keep it safe; the full rules come with move semantics (lesson
  48). In real code, prefer a `std::vector`/`unique_ptr` member so you don't
  manage raw memory at all.
- Initializing members in the body instead of the init list means they're
  default-constructed first, then reassigned — wasteful, sometimes impossible
  (references, `const` members).

## Build it step by step
1. **Skeleton.** In `buffer.hpp` declare `Buffer` with a constructor, a
   destructor, and the `size_`/`data_` members; `= delete` the copy operations.
2. **Acquire & release.** In `buffer.cpp`, make the constructor allocate and
   print "acquired", the destructor `delete[]` and print "released". In `main`,
   create a `Buffer` inside a `{ }` block and watch *when* each message prints.
3. **Use it.** Add `set`/`get` (with bounds checks) and read/write a few
   elements before the block ends.

## Build & run
```sh
make run app=46-constructors-destructors
```
Watch the order: "acquired" → usage → "released" → "left the block".

## Try it yourself
Create two `Buffer`s of different sizes in the same scope and observe that
destructors run in **reverse** order of construction (last created, first
destroyed).
