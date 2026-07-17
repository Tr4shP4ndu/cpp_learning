# 05-operators (C)

Operators are the symbols that compute with values: arithmetic, relational,
logical, bitwise, compound assignment, `++`/`--`, and the ternary `?:`. C's set
is the same as C++'s.

## Line-by-line
- `a / b` with two ints truncates toward zero (`7 / 2` is `3`); make one side a
  `double` (`7.0 / 2`) for real division. `%` is the integer remainder.
- Relational operators (`>`, `==`, ...) yield the `int` `1` for true or `0` for
  false — C has no distinct bool by default.
- `&&`, `||`, `!` short-circuit: the right operand is not evaluated once the
  result is known, which makes guarded checks safe.
- Bitwise: `|` sets bits, `& ~mask` clears them, `^` is XOR, `<<`/`>>` shift.
- `x += 3; x *= 2;` — compound assignment updates in place. `++x` increments
  then reads; each done in its own statement.
- `a > b ? a : b` — the ternary is an expression that produces one of two
  values.

## Common pitfalls
- Modifying a variable more than once between sequence points (e.g.
  `printf("%d %d", ++x, x--)`) is undefined behavior — keep such updates in
  separate statements.
- Expecting `5 / 2` to be `2.5`; integer division discards the fraction.
- Confusing `=` (assign) with `==` (compare) inside conditions.
- `%` requires integer operands; it does not work on `double`.

## Compared to C++
The operators behave identically; C++ additionally lets you overload most of
them for your own types. See the [C++ version](../../cpp/06-operators/).

## Build & run
```sh
make run-c-app-05-operators
```
