# 33-references

## Concept
A reference is a second **name** for an existing variable — an alias. Operations
on the reference act on the original object; no copy is made. References are how
C++ lets a function work on the caller's data (the flip side of value semantics,
lesson 32).

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `int& ref = value;` — `ref` is an alias for `value`. The `&` in a *type* means
  "reference to".
- `ref = 43;` — writes through the alias, so `value` becomes 43.
- `&value == &ref` prints `1` — they are literally the same object at the same
  address.
- `addBang(std::string& text)` — the parameter is a reference, so the function
  modifies the caller's `message` in place.

## Rules that make references safe
- A reference **must be initialized** when declared (no "null reference").
- Once bound, it **can't be re-seated** to refer to a different object.
- These two rules are why references are safer than pointers (lesson 34) when
  you just need an alias.

## References vs pointers (preview)
| | Reference | Pointer |
|---|---|---|
| Can be null? | No | Yes (`nullptr`) |
| Re-point later? | No | Yes |
| Syntax to use | like the variable | needs `*` to dereference |

Use a reference when you always have a valid object and won't need to rebind;
use a pointer when you need "maybe nothing" or reassignment.

## Common pitfalls
- **Dangling reference:** returning a reference to a local variable — it's
  destroyed at scope exit (the sanitizer catches use-after-return).
- `const std::string& text` is the read-only form (lesson 27); drop `const` only
  when you intend to modify.

## Build & run
```sh
make run app=33-references
```

## Try it yourself
Make a `const int& cref = value;` and try `cref = 5;` — it won't compile,
because a const reference is read-only.
