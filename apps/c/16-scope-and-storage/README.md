# 16-scope-and-storage (C)

*Scope* is where a name is visible; *storage* is how long its value lives. Locals
default to automatic storage (born and destroyed with their block), while
`static` and `extern` change lifetime and linkage.

## Line-by-line
- `static int file_private = 100;` — file scope with *internal linkage*: private
  to this source file.
- `int shared = 7;` — file scope with external linkage; another file can reach it.
- `extern int shared;` — a declaration only; it names something defined elsewhere.
- `static int counter` inside `next_id` — a static *local*: initialized once and
  persists across calls (why the ids count 1, 2, 3).
- `int local` / `int inner` — automatic storage; `inner` exists only inside its `{}`.

## Common pitfalls
- Expecting a plain local to remember its value between calls — it doesn't; use
  `static` for that.
- Confusing the two meanings of `static`: file-scope = internal linkage, local =
  persistent lifetime.
- Returning a pointer to an automatic local — it dies when the function returns.

## Compared to C++
C++ shares these rules (see the [C++ version](../../cpp/09-scope/)) and adds
`static` for class members and function-local statics with lazy init; C++'s
`static`-storage lesson is [cpp/42-static-storage](../../cpp/42-static-storage/).

## Build & run
```sh
make run-c-app-16-scope-and-storage
```
