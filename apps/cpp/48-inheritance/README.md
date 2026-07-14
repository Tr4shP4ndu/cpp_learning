# 48-inheritance

## Concept
Inheritance lets a new class (the *derived* class) build on an existing one (the
*base* class), reusing its data and behavior and adding more. It models an
"is-a" relationship: a `Dog` **is an** `Animal`.

## Files
```
48-inheritance/
  include/animals.hpp   <- Animal (base) and Dog : public Animal (derived)
  src/animals.cpp       <- their method definitions
  src/main.cpp          <- a Dog uses both inherited and its own methods
```

## Line-by-line
- `class Dog : public Animal` — Dog publicly inherits from Animal, so a Dog *is*
  an Animal and gets `name()`, `eat()`, and `name_`.
- `Dog::Dog(std::string name) : Animal(std::move(name))` — the derived
  constructor forwards to the **base constructor** to initialize the inherited
  part.
- `bark()` uses `name_` directly because it's `protected` (accessible to
  subclasses).
- In `main`, `d.eat()` is inherited; `d.bark()` is Dog's own.

## `public` / `protected` / `private` members
- `public` — everyone.
- `protected` — this class and its derived classes.
- `private` — only this class (not even subclasses).

## Common pitfalls
- **Prefer composition when it's not truly "is-a".** A `Car` *has an* `Engine`
  (a member), it isn't an engine. Overusing inheritance creates rigid designs.
- Deep inheritance hierarchies get hard to follow — keep them shallow.
- Inheritance alone doesn't give you runtime polymorphism; calling a base
  pointer's method still calls the base version unless it's `virtual` — that's
  lesson 49.

## Build it step by step
1. **The base.** Write `Animal` (`name_`, `name()`, `eat()`), define its methods,
   and use an `Animal` directly in `main`.
2. **Derive.** Add `class Dog : public Animal` with a constructor that forwards
   the name to `Animal(...)`. A `Dog` already has `eat()` for free — call it.
3. **Extend.** Add `Dog::bark()` (it can use the `protected` `name_`) and call
   both `eat()` and `bark()` on a `Dog`.

## Build & run
```sh
make run app=48-inheritance
```

## Try it yourself
Add a `Cat : public Animal` with a `meow()` method. Notice how little code it
takes — `name()`/`eat()` come for free.
