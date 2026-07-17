# 34-error-handling (C)

C has no exceptions. A function that can fail tells you by its return value
(often `NULL`, `-1`, or a non-zero code) and sets the global `errno` to say why.
You check the return, then report using `perror` or `strerror(errno)`.

## Line-by-line
- `#include <errno.h>` — declares `errno`, the thread-local error code.
- `errno = 0;` — clear it first; a function only sets it on failure, it never
  clears it on success.
- `fopen(..., "r")` returns `NULL` because the file is missing.
- `perror("fopen")` — prints `fopen: ` followed by the system's message for the
  current `errno`.
- `strerror(errno)` — returns that same message as a string you can format
  yourself.
- `EXIT_SUCCESS` / `EXIT_FAILURE` — portable exit codes (`0` = success, non-zero
  = failure to the shell). A real tool would `return EXIT_FAILURE` on the error
  path; this lesson reports the error and returns success so `make run` stays
  clean.

## Common pitfalls
- Not checking the return value at all, then using a `NULL` handle.
- Reading `errno` after a call that *succeeded* — it is only meaningful right
  after a call that reported failure.
- Calling another library function before reading `errno` — it may overwrite it.
- Returning `1`/`0` from `main` with reversed meaning; `0` is success.

## Compared to C++
C++ keeps codes+`errno` but adds exceptions (`try`/`catch`, `throw`) and modern
value-based error types like `std::expected`. See the
[C++ version](../../cpp/61-error-handling/).

## Build & run
```sh
make run-c-app-34-error-handling
```
