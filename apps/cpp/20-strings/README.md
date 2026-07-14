# 21-strings

## Concept
Text handling is where C and C++ diverge most. C++'s `std::string` is a
self-managing text object: it grows as needed, concatenates with `+`, knows its
length, and frees its own memory. In C, a "string" is just a raw array of `char`
terminated by a `'\0'` byte that *you* manage.

## Minimal example
See `src/main.cpp`. The C version of this lesson lives in [`apps/c/21-strings`](../../c/21-strings/) (run it with `make run-c app=21-strings`).

## Line-by-line (C++)
- `std::string full = first + " " + last;` — concatenation just works; the
  string allocates whatever space it needs.
- `full.size()` — length in O(1); no scanning.
- `full[0]` — index like an array.
- `full += "!";` — append in place.
- `full.substr(0, 3)`, `full.find("Love")` — rich built-in operations.

## Line-by-line (C)
- `char full[32];` — you pick the buffer size up front.
- `strcpy` / `strcat` — copy and append by hand; **you** must ensure the buffer
  is big enough. Overflowing it is a real security vulnerability.
- `strlen(full)` — counts characters up to the `'\0'` (O(n)).
- `strchr` returns a *pointer* into the string; subtract the base to get an
  index.

## Common pitfalls
- **C:** buffer overflow (`strcat` past the end), forgetting the `'\0'`, and
  using `strlen` in a loop condition (re-scans every time).
- **C++:** taking `char* c = str.c_str()` and keeping it after the string
  changes — the pointer dangles.

## C vs C++
`std::string` exists precisely to eliminate the C footguns above: no fixed
buffer, no manual `'\0'`, no overflow. Use it in C++ unless you're interfacing
with a C API. This is the clearest example of "C++ gives you a safe object where
C gives you raw memory."

## Build & run
```sh
make run app=21-strings      # C++
make run-c app=21-strings    # C — note the manual buffer management
```

## Try it yourself
In the C++ version, read a word with `std::cin >> word` and append it to `full`.
Then try to do the same safely in C — notice how much more you have to think
about.
