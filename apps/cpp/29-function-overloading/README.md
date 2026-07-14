# 29-function-overloading

## Concept
Overloading lets several functions share one name as long as their **parameter
lists differ**. The compiler picks the right one based on the argument types at
each call. It's how one clear name (`add`, `print`) can serve many types.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `int add(int x, int y)` and `double add(double x, double y)` — same name, two
  parameter signatures.
- `add(3, 4)` → the `int` version; `add(3.5, 4.5)` → the `double` version. This
  resolution happens at compile time (no runtime cost).

## The rule
Overloads must differ in the **number or types** of parameters. They **cannot**
differ only by return type — `int f()` vs `double f()` is an error, because the
call `f()` gives the compiler no way to choose.

## Common pitfalls
- **Ambiguous calls:** `add(3, 4.5)` (int + double) may not match either overload
  cleanly and can fail to compile. Provide an overload or cast explicitly.
- Adding overloads that do subtly different things for different types surprises
  readers — keep overloads behaving *consistently*.
- Overloading interacts with implicit conversions in ways that get tricky;
  templates (lesson 52) are often the better tool for "same logic, many types".

## Build & run
```sh
make run app=29-function-overloading
```

## Try it yourself
Add a third overload `std::string add(const std::string& a, const std::string&
b)` that concatenates, and call `add(std::string("foo"), std::string("bar"))`.
