# 48-move-semantics

## Concept
Copying an object that owns a big resource (a vector, a string, a file handle)
duplicates that resource — expensive. *Moving* instead **transfers ownership**
from a temporary or soon-to-die object, leaving the source empty. It's usually
just a few pointer swaps. Move semantics (C++11) are why returning a big
`std::vector` by value is cheap.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `Widget(const Widget&)` — the **copy** constructor: duplicates `data_`.
- `Widget(Widget&& other) noexcept` — the **move** constructor: `T&&` binds to
  rvalues; `std::move(other.data_)` steals the buffer rather than copying it.
- `Widget b = a;` — `a` is a named lvalue → the **copy** ctor runs.
- `Widget c = std::move(a);` — `std::move` casts `a` to an rvalue → the **move**
  ctor runs. Afterward `a` is valid but unspecified (don't read its contents).

## Key ideas
- `std::move` **doesn't move anything** — it's just a cast to `T&&` that says
  "you may pillage me." The actual work is in the move constructor/assignment.
- Mark move operations `noexcept` — standard containers (like `vector` growing)
  only use your move ctor if it can't throw; otherwise they fall back to copying.
- The compiler often **elides** copies/moves entirely (RVO) when returning
  locals, making value returns essentially free.

## The rule of five
If your class needs a custom destructor, copy ctor, copy assignment, move ctor,
or move assignment, it usually needs to consider **all five** (compare lesson
41, which `= delete`d copies to stay safe). Best of all: use members that
already handle this (`vector`, `string`, `unique_ptr`) and you get correct
copy/move **for free** — the rule of *zero*.

## Common pitfalls
- Using a moved-from object as if it still holds its data.
- Forgetting `noexcept` on move operations → silent fallback to copying.

## Build & run
```sh
make run app=48-move-semantics
```
Note "copy A (1000 elems)" for `b` vs "move A (stole the buffer)" for `c`.

## Try it yourself
Add a function `Widget makeWidget()` that returns a `Widget` by value and
construct one from it. Observe how few (if any) copies/moves happen thanks to
elision.
