# 40-classes-basics

## Concept
A class is a blueprint that bundles **data** (member variables) with **behavior**
(member functions / methods) into one type. An *object* is a concrete instance
of that class. This is the core idea of object-oriented programming.

## Files
```
40-classes-basics/
  include/rectangle.hpp   <- the class declaration
  src/rectangle.cpp       <- the method definitions
  src/main.cpp            <- creates and uses objects
```

## Line-by-line
- The header declares `class Rectangle` with two data members and two method
  *declarations*.
- `double Rectangle::area() const { ... }` — the `Rectangle::` prefix ties the
  definition to the class. Inside, `width`/`height` mean *this object's*
  members.
- `const` after a method means it won't modify the object — mark every method
  that only reads.
- In `main`, `Rectangle r;` creates an object; `r.area()` calls a method on it.

## Class vs object
The class is the type (written once); each object is a separate instance with
its own copy of the members. Two `Rectangle`s have independent `width`/`height`.

## Common pitfalls
- Forgetting `const` on read-only methods — you then can't call them on a
  `const Rectangle&`.
- Defining a method body in the header without `inline` and including that
  header in multiple `.cpp`s violates the ODR (lesson 30) — keep definitions in
  the `.cpp`.
- Here members are `public` for simplicity; lesson 42 (encapsulation) makes them
  `private` and explains why.

## Build & run
```sh
make run app=40-classes-basics
```

## Try it yourself
Add a `double perimeter() const;` — declare it in the header, define it in the
`.cpp`, and call it from `main`.
