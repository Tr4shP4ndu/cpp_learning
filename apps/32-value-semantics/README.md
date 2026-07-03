# 32-value-semantics

## Concept
By default, C and C++ have *value semantics*: assigning a variable, or passing
it to a function, makes an independent **copy**. Changing the copy leaves the
original untouched. This is the baseline behavior that references (33) and
pointers (34) deliberately break.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `int b = a;` — `b` is a separate copy; `b = 7;` doesn't affect `a`.
- `tryToChange(int n)` — `n` is a copy of the argument. Setting `n = 100`
  changes only the local copy.
- After the call, `a` is still 42 — the function never had access to the
  original.

## Why it matters
Value semantics make code easy to reason about: a function can't secretly
mutate your variables. The cost is copying — cheap for an `int`, expensive for a
big `std::vector`. That trade-off is exactly why lesson 27 introduced
`const&` (see without copying) and why 33/34 give you references and pointers to
*share* rather than copy.

## Common pitfalls
- Expecting a function to modify a value passed by value — it can't; it only
  sees a copy. Return the new value, or pass by reference/pointer.
- Copying large objects in a loop or hot path silently wastes time.

## Build & run
```sh
make run app=32-value-semantics
```

## Try it yourself
Add `int c = a; c += 1;` and print all three. Confirm `a` is unchanged. Then
change `tryToChange` to take `int& n` (a reference) and watch `a` change —
previewing lesson 33.
