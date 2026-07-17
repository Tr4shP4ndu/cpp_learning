# 37-type-conversions (C)

C quietly converts between types in arithmetic and comparisons. Knowing the
rules — integer promotion, the usual arithmetic conversions, and the
signed/unsigned trap — saves you from surprising results. Explicit casts let you
take control when you need to.

## Line-by-line
- `c + c` — `char` operands are promoted to `int` first, so the result type is
  `int` and prints with `%d`.
- `(unsigned)a < b` with `int a = -1` and `unsigned b = 1u` — a bare `a < b`
  does this implicitly; the usual arithmetic conversions turn `-1` into a large
  unsigned value, so the test is **false**. (Compilers flag the bare form with
  `-Wsign-compare`, which is why we write the cast out.)
- `(unsigned)a` — shows what `-1` becomes when reinterpreted as unsigned.
- `(int)d` — float-to-int truncates toward zero (`3.9` becomes `3`), it does not
  round.
- `x / y` is integer division (`3`); `(double)x / y` casts one operand so the
  division is done in floating point (`3.5`).

## Common pitfalls
- Comparing signed and unsigned values (e.g. a loop `int i` against a `size_t`
  count) — the signed side converts and negatives become huge.
- Expecting `(int)3.9` to round; it truncates.
- Writing `(double)(x / y)` — the integer division already happened; cast an
  operand *before* dividing: `(double)x / y`.

## Compared to C++
C++ has the same implicit conversion rules but adds named casts —
`static_cast`, `reinterpret_cast`, `const_cast`, `dynamic_cast` — that make
intent explicit and are easier to search for than C's `(type)` casts. See the
[C++ version](../../cpp/52-casting/).

## Build & run
```sh
make run-c-app-37-type-conversions
```
