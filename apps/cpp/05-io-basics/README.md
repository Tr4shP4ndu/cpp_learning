# 05-io-basics

## Concept
Programs talk to the user through *standard input* (the keyboard) and *standard
output* (the screen). This lesson reads a name and an age, then prints a
greeting. It's your first *interactive* program.

## Minimal example
See `src/main.cpp`. The C version of this lesson lives in [`apps/c/05-io-basics`](../../c/05-io-basics/) (run it with `make run-c-app-05-io-basics`).

## Line-by-line
- `std::cout << ...` — write to the screen.
- `std::getline(std::cin, name);` — read one whole line (including spaces) into
  `name`.
- `std::cin >> age;` — read a single whitespace-separated token and convert it
  to `int`.
- `age + 1` — you can compute inside an output statement.

## Common pitfalls
- **Mixing `>>` and `getline`.** `std::cin >> age;` leaves the newline in the
  buffer; a following `getline` then reads an empty line. Read the line *first*
  (as here), or clear the leftover newline.
- `std::cin >> age` on non-numeric input fails and leaves `age` unchanged —
  real programs check `if (std::cin >> age)`.
- In C, never use `gets` (removed from the language). Use `fgets` with a size,
  as shown, to avoid buffer overflows.

## C vs C++
| | C | C++ |
|---|---|---|
| Print | `printf("...", args)` | `std::cout << ...` |
| Read a line | `fgets(buf, size, stdin)` | `std::getline(std::cin, s)` |
| Read a number | `scanf("%d", &age)` | `std::cin >> age` |

C's `scanf` needs the `&` (address-of) so it can write back into your variable —
a preview of pointers (lesson 39). C++ streams handle that for you.

## Build & run
This program is **interactive** — it waits for keyboard input. Run it and type:
```sh
make run-cpp-app-05-io-basics
```
To feed input non-interactively (e.g. for a quick test), pipe it in:
```sh
printf 'Ada\n36\n' | make run-cpp-app-05-io-basics
```

## Try it yourself
Ask for the user's favorite number and print its square.
