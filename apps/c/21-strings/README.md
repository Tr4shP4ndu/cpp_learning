# 21-strings (C)

The C version of [`apps/cpp/21-strings`](../../cpp/21-strings/). In C a "string"
is a raw `char` array ending in a `'\0'` byte — everything is manual.

## Line-by-line
- `char full[32];` — you choose the buffer size up front.
- `strcpy` / `strcat` (from `<string.h>`) — copy and append by hand; **you** must
  ensure the buffer is big enough. Overflowing it is a classic security bug.
- `strlen(full)` — counts characters up to the `'\0'` (O(n) each call).
- `strchr(full, 'L')` — returns a *pointer* into the string; subtract the base
  to get an index.

## Common pitfalls
- Buffer overflow (`strcat` past the end), forgetting the `'\0'`, and calling
  `strlen` repeatedly in a loop condition.

## Compared to C++
`std::string` grows automatically, concatenates with `+`, and frees itself —
none of the manual buffer management here. This is the clearest case for
preferring C++ objects; see the [C++ version](../../cpp/21-strings/).

## Build & run
```sh
make run-c app=21-strings
```
