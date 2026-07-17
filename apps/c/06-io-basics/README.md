# 06-io-basics (C)

The C version of [`apps/cpp/05-io-basics`](../../cpp/05-io-basics/). Reads a
name and an age, prints a greeting.

## Line-by-line
- `printf(...)` — write to the screen.
- `fgets(name, sizeof name, stdin)` — read a whole line **safely** (bounded by
  the buffer size). Never use `gets` — it can overflow.
- `name[strcspn(name, "\n")] = '\0';` — `fgets` keeps the trailing newline;
  this overwrites it with a string terminator.
- `scanf("%d", &age)` — read an integer. The `&` (address-of) lets `scanf`
  write back into your variable — a preview of pointers (lesson 39).

## Common pitfalls
- Not checking the return values of `fgets`/`scanf` (both can fail).
- Buffer sizing — always pass `sizeof buf` to `fgets`.

## Compared to C++
C++ streams (`std::cin >> age`, `std::getline`) handle sizing and the address-of
for you. See the [C++ version](../../cpp/05-io-basics/).

## Build & run
This program is interactive; pipe input to run it non-interactively:
```sh
printf 'Ada\n35\n' | make run-c-app-06-io-basics
```
