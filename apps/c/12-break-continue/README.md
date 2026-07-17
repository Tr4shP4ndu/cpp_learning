# 12-break-continue (C)

`break` leaves the enclosing loop immediately; `continue` skips the rest of the
current pass and jumps to the next iteration. Here they combine to sum only the
even numbers from 2 to 10.

## Line-by-line
- `if (i % 2 != 0) continue;` — odd `i`: skip straight to the `i++` step, adding
  nothing.
- `if (i > 10) break;` — once `i` passes 10, abandon the loop for good.
- `sum += i;` — reached only by even `i` in `2..10`, giving `2+4+6+8+10 = 30`.

## Common pitfalls
- `break`/`continue` act on the **innermost** loop only — they can't exit two
  nested loops at once (C has no labeled break; people use a flag or `goto`).
- In a `for` loop, `continue` still runs the step (`i++`); in a `while` loop it
  does **not**, so you must update the counter yourself before `continue` or you
  loop forever.
- Confusing the two: `break` ends the loop, `continue` only ends the pass.

## Compared to C++
Identical keywords and behavior. See the
[C++ version](../../cpp/16-break-statement/) (and its companion
[continue lesson](../../cpp/17-continue-statement/)).

## Build & run
```sh
make run-c-app-12-break-continue
```
