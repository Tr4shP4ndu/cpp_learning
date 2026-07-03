# 23-value-semantics

- Value semantics means objects behave like values: copying produces independent objects; assignment replaces the value; equality compares contents; no hidden sharing or aliasing unless explicit.

## Why value semantics

- Local reasoning: copies can be modified without affecting the original.
- Safety: fewer aliasing bugs and simpler lifetimes (RAII-managed resources).
- APIs: return-by-value is clear, and copy elision makes it efficient.

## Pass-by-value vs references

- Pass by value (T): the function gets its own copy.
  - Great for small/trivially copyable types (int, double, small structs).
  - Also useful with move semantics: take by value then std::move into fields.
- Pass by const reference (const T&): no copy, read-only view of large objects.
- Pass by non-const reference (T&): call intends to mutate the caller’s object.

## Return by value and copy elision

- Prefer returning by value. Compilers perform NRVO and copy elision, making it fast.
- Move semantics kicks in when elision doesn’t apply, avoiding deep copies.

## Small example

struct Point { int x{}; int y{}; };

Point translated(Point p, int dx, int dy) { // p is a copy
  p.x += dx; p.y += dy;                     // modify the copy
  return p;                                 // returned by value (elided/moved)
}

int main() {
  Point a{1,2};
  Point b = a;         // copy: independent value
  b.x = 10;            // changing b does not affect a
  std::cout << a.x << "," << a.y << "\n"; // 1,2

  Point c = translated(a, 5, -2);          // a unchanged, c is a new value
}

## Pitfalls and tips

- Expensive copies: for heavy objects, prefer const T& parameters; still return by value.
- Object slicing: copying a base subobject drops derived data; avoid passing polymorphic types by value.
- Hidden sharing: raw pointers/shared state break value semantics; prefer owning value types (std::string, std::vector) and RAII.
- Equality: define operator== to mean value equality when it makes sense (C++20 can use = default for aggregates).
- Move-aware APIs: for sink parameters, take by value and std::move into members to accept both lvalues (copy) and rvalues (move).

## Build and run (from repository root)

Run these from the repository root:
  - make build app=23-value-semantics
  - make run app=23-value-semantics

Binary path: build/23-value-semantics/bin/23-value-semantics

Alternative (from inside this folder):
  - cd app/23-value-semantics
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
