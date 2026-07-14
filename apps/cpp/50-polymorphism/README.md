# 49-polymorphism

## Concept
Polymorphism lets one interface stand for many types. Through a base-class
pointer or reference, a call like `shape->area()` runs the *derived* type's
version — decided at **runtime**. This is how you write code that works with
"any Shape" without knowing which concrete shape it holds.

## Files
```
49-polymorphism/
  include/shapes.hpp   <- abstract Shape + Circle/Square overrides
  src/shapes.cpp       <- the concrete area()/name() implementations
  src/main.cpp         <- iterate base pointers; correct version runs
```

## Line-by-line
- `virtual double area() const = 0;` — `virtual` enables runtime dispatch; `= 0`
  makes it **pure virtual**, so `Shape` is **abstract** (you can't instantiate a
  bare `Shape`, only its concrete subclasses).
- `override` on the derived methods asks the compiler to verify they really
  override a base virtual — catches typos in the signature.
- `virtual ~Shape() = default;` — a polymorphic base **must** have a virtual
  destructor, or `delete`ing through a base pointer won't call the derived
  destructor (a leak/UB).
- In `main`, a `vector<unique_ptr<Shape>>` holds mixed shapes; the loop calls the
  same methods and each object responds with its own behavior.

## Why virtual matters
Without `virtual`, `shape->area()` would call `Shape::area` based on the pointer
*type*, not the object. `virtual` flips that: the call resolves to the actual
object's type via a vtable lookup — a tiny runtime cost for huge flexibility.

## Common pitfalls
- **Forgetting the virtual destructor** on a base you delete polymorphically.
- **Object slicing:** storing derived objects *by value* in a `Shape`
  (`std::vector<Shape>`) chops off the derived part. Use pointers/references
  (here, `unique_ptr<Shape>`).
- Marking overrides — always use `override`; it turns silent mistakes into
  compile errors.

## Build it step by step
1. **Abstract base.** In `shapes.hpp` write `Shape` with a **virtual destructor**
   and two **pure-virtual** methods (`= 0`). You can't instantiate it yet — that's
   the point.
2. **One concrete shape.** Add `Circle : public Shape`, implement `area()`/`name()`
   with `override`, and print one circle's area.
3. **Many, uniformly.** Add `Square`, then put both in a
   `std::vector<std::unique_ptr<Shape>>` and loop calling `area()`/`name()` — the
   same call runs the right version per object.

## Build & run
```sh
make run app=49-polymorphism
```

## Try it yourself
Add a `Triangle : public Shape` (`area = 0.5 * base * height`) and push one into
the vector. The loop needs **no changes** — that's the payoff of polymorphism.
