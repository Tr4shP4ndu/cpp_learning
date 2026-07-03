# 06-operators

## Concept
Operators are the symbols that compute with values: `+ - * /`, comparisons,
logical `&&`/`||`, bit twiddling, and `++`/`--`. This lesson runs one of each
and checks the result with `assert`.

## Minimal example
See `src/main.cpp`.

## Categories
- **Arithmetic:** `+ - * / %` (`%` is remainder, integers only).
- **Assignment / compound:** `=`, `+= -= *= /= %=`, `&= |= ^= <<= >>=`.
- **Comparison:** `== != < <= > >=` → produce a `bool`.
- **Logical:** `&& || !` — short-circuit, left to right.
- **Bitwise:** `& | ^ ~ << >>` — operate on individual bits.
- **Increment/decrement:** `++ --` in prefix and postfix forms.
- **Special:** `sizeof`, `&` (address-of), `*` (dereference), `?:` (ternary).

## Line-by-line
- `a / b` with two `int`s truncates: `7 / 3 == 2`. Cast one side with
  `static_cast<double>` for real division.
- `p != nullptr && *p > 0` — because `&&` short-circuits, `*p` is only evaluated
  when `p` is non-null. This is the idiomatic safe null check.
- `flags |= 0b0010;` sets a bit; `flags &= ~0b0001u;` clears one.
- `++x` increments then yields the new value; `x++` yields the old value then
  increments.

## Common pitfalls
- **Integer division truncates.** `1/2 == 0`. Cast to floating point when you
  want a fraction.
- **Precedence surprises.** `a + b << 1` means `(a + b) << 1`. When unsure, add
  parentheses — clarity beats memorizing the table.
- **`&&` vs `&` and `||` vs `|`** are different: logical vs bitwise. Mixing them
  up compiles but misbehaves.
- Signed overflow and shifting negative numbers are undefined behavior (the
  sanitizer in Debug builds will flag them).

## Build & run
```sh
make run app=06-operators
```

## Try it yourself
Add a compound-assignment line (`a *= 2;`) and an `assert` that checks the new
value.
