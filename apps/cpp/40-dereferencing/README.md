# 40-dereferencing

## Concept
Dereferencing means following a pointer to the object it points at, using the
`*` operator. It's how you read and write the *value* through a pointer, rather
than manipulating the address itself.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `int* ptr = &num;` — `ptr` holds `num`'s address (lesson 39).
- `ptr` alone → the address. `*ptr` → the value stored there (10).
- `*ptr = 20;` — write *through* the pointer; because `ptr` points at `num`,
  `num` becomes 20.

## The two hats of `*`
- In a **declaration**: `int* ptr` — "ptr is a pointer to int".
- In an **expression**: `*ptr` — "the value ptr points to".
Same symbol, different jobs. This trips up every beginner once.

## Common pitfalls
- **Dereferencing null/invalid pointers** is undefined behavior. Always ensure
  the pointer points at a live object first (AddressSanitizer, on in Debug,
  catches many of these).
- `*ptr + 1` vs `*(ptr + 1)`: the first adds 1 to the value; the second does
  *pointer arithmetic* (move to the next int) then dereferences. Parenthesize.
- Confusing `ptr` (address) with `*ptr` (value) in output.

## Build & run
```sh
make run app=40-dereferencing
```

## Try it yourself
Point a second pointer at the same `num` and modify through one — observe both
`*ptr` and the original `num` reflect the change (they're the same object).
