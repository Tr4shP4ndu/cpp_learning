# 10-while-loop (C)

A `while` loop repeats its body as long as a condition is true, checking the
condition *before* each pass. Here it counts a number's digits by repeatedly
dividing by 10.

## Line-by-line
- `int rest = n;` — a working copy so the original `n` survives for the printout.
- `while (rest > 0)` — keep looping until nothing is left.
- `rest /= 10;` — integer division shifts off the last digit (e.g. `90210` -> `9021`).
- `digits++;` — tally one digit per pass.

## Common pitfalls
- Forgetting to change the loop variable — the condition stays true and the loop
  never ends.
- Counting `0` as zero digits: this loop reports `0` for input `0`; handle that
  case separately if you need at least one digit.
- Using `float`/`double` and expecting `/= 10` to truncate — only integer
  division drops the remainder.

## Compared to C++
Same `while` semantics in both languages. See the
[C++ version](../../cpp/14-while-loop/).

## Build & run
```sh
make run-c-app-10-while-loop
```
