# 27-bitfields (C)

Struct bit-fields let you say exactly how many bits a member uses, packing
several small values into one word. Separately, the bitwise operators let you
set, clear, and test individual flags inside an ordinary integer mask.

## Line-by-line
- `unsigned bold : 1;` — a field one bit wide (holds 0 or 1).
- `unsigned level : 3;` — three bits, so it can hold 0..7.
- `(1u << 0)`, `(1u << 1)`, `(1u << 2)` — `<<` builds a mask with one bit set.
- `mask |= READ;` — OR sets the flag's bit.
- `mask &= ~WRITE;` — AND with the inverted mask clears just that bit.
- `mask & READ` — AND tests whether the bit is set (nonzero if so).

## Common pitfalls
- You cannot take the address of a bit-field (`&f.bold` is illegal).
- Signedness and layout of bit-fields are implementation-defined; prefer `unsigned`.
- Use `&` (bitwise) not `&&` (logical) when testing masks.
- Assigning a value too big for the field silently truncates it.

## Compared to C++
C++ shares this feature unchanged: the same bit-field syntax and the same
bitwise operators behave identically.

## Build & run
```sh
make run-c-app-27-bitfields
```
