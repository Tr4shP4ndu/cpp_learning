# 10-namespaces

## Concept
A namespace is a named region that groups identifiers. It prevents name
collisions: two libraries can both define `x` as long as each lives in its own
namespace. The standard library lives in the `std` namespace — that's why it's
`std::cout`.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `namespace first { int x = 1; }` and `namespace second { int x = 2; }` — two
  variables named `x` that coexist peacefully.
- `first::x` / `second::x` — the `::` scope-resolution operator picks which one.
- A local `int x = 0;` in `main` is a third, unqualified `x`.
- `using std::cout;` — a *using declaration* brings one name into scope so you
  can write `cout` instead of `std::cout`.

## Common pitfalls
- **Never put `using namespace std;` in a header** — it forces every file that
  includes the header to import all of `std`, causing collisions. In small
  `.cpp` files or function bodies it's tolerable; qualifying with `std::` is
  clearer.
- The `::name` form (leading `::`) refers to the global namespace explicitly.

## Handy features (for later)
- Namespace alias: `namespace fs = std::filesystem;` (used in lesson 23).
- Nested shorthand (C++17): `namespace company::util { ... }`.
- Anonymous namespace `namespace { ... }` — makes names file-local.

## Build & run
```sh
make run app=10-namespaces
```

## Try it yourself
Add a `namespace third { int x = 3; }` and print `third::x`.
