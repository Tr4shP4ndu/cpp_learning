# 45-operator-overloading

## Concept
Operator overloading lets you define what `+`, `==`, `<<`, etc. mean for your own
types, so a `Vec2` can be added with `a + b` and printed with `std::cout << v`
just like a built-in type. Used tastefully, it makes math-like types read
naturally.

## Files
```
45-operator-overloading/
  include/vec2.hpp   <- declares the operators for Vec2
  src/vec2.cpp       <- defines them
  src/main.cpp       <- uses +, +=, ==, << on Vec2 (with assert checks)
```

## Line-by-line
- `Vec2& operator+=(const Vec2& rhs)` — a **member** operator; `return *this;`
  lets you chain.
- `Vec2 operator+(const Vec2& a, const Vec2& b)` — a **non-member** (free)
  function; symmetric binary operators read best this way. It reuses `+=` rather
  than duplicating the arithmetic.
- `operator==` — enables `a == b`.
- `std::ostream& operator<<(std::ostream&, const Vec2&)` — teaches `std::cout`
  how to print a `Vec2`. Must be a free function (the left operand is the
  stream).
- The `assert`s double as a self-check.

## Member vs non-member
- Operators that modify the left operand (`+=`, `++`) are usually **members**.
- Symmetric operators (`+`, `==`, `<<`) are usually **non-members**, so both
  sides get equal treatment (and conversions apply symmetrically).

## Common pitfalls
- **Don't surprise the reader.** Overload operators only when the meaning is
  obvious (`+` for vector addition — yes; `+` to send a network packet — no).
- Implement related operators consistently (if you define `==`, users expect
  `!=` too; C++20 can generate it from `==`).
- Define `+` in terms of `+=` to avoid duplicated logic (shown here).

## Build it step by step
1. **Print it.** Define `struct Vec2 { double x, y; };` and `operator<<` so
   `std::cout << v` works. Verify it prints.
2. **Add them.** Add the free function `operator+`; use `a + b` and print the
   result.
3. **More operators.** Add `operator+=` (member, `return *this;`) and
   `operator==`, then add `assert`s that check the results.

## Build & run
```sh
make run app=45-operator-overloading
```

## Try it yourself
Add `operator-` (subtraction) and a `operator*` for scaling by a scalar
(`Vec2 operator*(const Vec2& v, double s)`).
