# 19-string-library (C)

`<string.h>` gives you the classic C string functions and `<ctype.h>` the
per-character helpers. None of them allocate — you supply every buffer, and
sizing it correctly is your job.

## Line-by-line
- `strcpy(buf, src)` — copies `src` (plus its `'\0'`) into `buf`; unbounded.
- `strncpy(safe, src, sizeof safe - 1)` — bounded, so it can't overflow, but it
  may leave `safe` *unterminated* when `src` is too long — hence
  `safe[sizeof safe - 1] = '\0';`. That is why `strncpy` is the safer choice.
- `strcat(buf, "!")` — appends onto the existing `'\0'`; `buf` must have room.
- `strcmp(a, b)` — returns `<0`, `0`, or `>0` by lexicographic order.
- `strchr(buf, ' ')` / `strstr(buf, "Love")` — return a *pointer* into the
  string (or `NULL`); subtract the base to get an index.
- `toupper` / `isdigit` — take an `int`; pass `(unsigned char)` to avoid UB on
  negative `char` values.

## Common pitfalls
- Assuming `strncpy` always terminates — it does not; terminate it yourself.
- `strcpy`/`strcat` past the end of the buffer (a classic overflow; ASan catches it).
- Passing a raw `char` to `<ctype.h>` functions instead of `(unsigned char)`.

## Compared to C++
`std::string` owns and resizes its own buffer, compares with `==`/`<`, appends
with `+`, and searches with `.find()` — no manual sizing or `'\0'` bookkeeping.
See the [C++ version](../../cpp/20-strings/).

## Build & run
```sh
make run-c-app-19-string-library
```
