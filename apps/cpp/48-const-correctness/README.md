# 48-const-correctness

## Concept
**Const-correctness** means marking everything that shouldn't change as `const`,
and letting the compiler enforce it. On a class, a **const member function**
(`double balance() const`) promises not to modify the object. That promise is
what makes `const Account&` parameters useful: the caller knows the function
can't mutate their object, and a `const` object can only call const methods.
It builds directly on encapsulation (lesson 47).

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `double balance() const` — the trailing `const` makes this a const member
  function: inside it, `this` is `const`, so it can read but not write members.
- `void deposit(double)` — no `const`, so it may mutate `balance_`, and may only
  be called on a non-const object.
- `void print_balance(const Account& a)` — `a` is const here, so only const
  methods (`balance()`) are callable; the commented `deposit` would fail to
  compile. That compile error is the feature.
- `const Account frozen(...)` — a const object; calling a non-const method on it
  is rejected by the compiler.

## Why this matters
- **Interfaces document intent:** a `const` reference parameter says "I only read
  this," verified by the compiler, not by trust.
- **It's viral in a good way:** const-correct getters let const objects and const
  references flow through your whole API.

## Common pitfalls
- Forgetting `const` on a getter means it can't be called on a `const` object or
  through a `const&` — a very common source of "why won't this compile?".
- `const` on the function applies to `*this`; it's separate from a `const`
  return type or `const` parameters.
- Genuinely need to change a cache/counter inside a const method? Mark that one
  member `mutable` — the deliberate escape hatch, not a default.

## Build & run
```sh
make run-cpp-app-48-const-correctness
```

## Try it yourself
Uncomment the `frozen.deposit(1.0);` line and read the compiler error. Then add a
`mutable int reads_;` member that `balance() const` increments, and confirm it
compiles even though `balance()` is const.
