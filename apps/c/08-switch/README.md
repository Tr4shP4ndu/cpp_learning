# 08-switch (C)

`switch` picks one branch out of many by comparing a single integer value
against constant `case` labels. It works on any integer type (`int`, `char`,
`enum`) but not on strings.

## Line-by-line
- `switch (day)` — evaluates an int and jumps to the matching `case` label.
- `case 1:` ... `case 5:` stacked with one body — the empty cases fall through
  into the shared `printf`, a common way to group values.
- `break;` ends the switch. Case 6 omits it on purpose, so control falls
  through into case 7 and both messages print.
- `default:` runs when no case matches.
- The second switch uses a `char` (`'A'`, `'B'`), which is fine because `char`
  is an integer type; you still cannot `case "text"`.

## Common pitfalls
- Forgetting `break`, causing accidental fall-through into the next case.
- Trying to switch on a string — `switch (str)` / `case "x"` does not compile
  in C; use `if`/`strcmp` chains instead.
- Declaring a variable directly inside a `case` without a `{ }` block can trip
  compilers because case labels do not open a new scope.
- Using non-constant expressions as case labels; labels must be compile-time
  integer constants.

## Compared to C++
Same behavior; C++17 adds the `[[fallthrough]]` attribute to mark intentional
fall-through and silence warnings. See the [C++ version](../../cpp/08-switch/).

## Build & run
```sh
make run-c-app-08-switch
```
