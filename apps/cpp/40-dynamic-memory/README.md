# 41-dynamic-memory

## Concept
Sometimes you don't know how much memory you need until run time, or you need
data to outlive the scope that created it. *Dynamic allocation* asks the system
for memory from the **heap**. The catch: you must return it exactly once, or you
leak (never freed) or crash (freed twice / used after free).

## Minimal example
See `src/main.cpp`. The C version of this lesson lives in [`apps/c/41-dynamic-memory`](../../c/41-dynamic-memory/) (run it with `make run-c app=41-dynamic-memory`).

## Line-by-line (C++)
- `int* p = new int(42);` — allocate one int on the heap, initialized to 42.
- `delete p;` — release it. Exactly once.
- `p = nullptr;` — defensive: a null pointer can't be accidentally reused.
- `new int[n]` / `delete[] arr` — arrays use the `[]` forms. **Mismatching**
  `new[]` with `delete` (no brackets) is undefined behavior.

## Line-by-line (C)
- `malloc(sizeof *p)` — asks for a number of **bytes**; returns `void*` or
  `NULL`. `sizeof *p` sizes it to whatever `p` points at.
- Always **check for `NULL`** before using the result.
- `free(p)` — release. One `free` per `malloc`.

## The bugs this creates
- **Leak:** you `new`/`malloc` but never `delete`/`free`. Memory piles up.
- **Double free:** releasing the same block twice → corruption.
- **Use-after-free / dangling pointer:** using memory after releasing it.
- **Wrong form:** `delete` vs `delete[]`.

AddressSanitizer (on in Debug builds) catches double-free and use-after-free at
runtime.

## C vs C++ — and why you rarely do this by hand
| | C | C++ |
|---|---|---|
| Allocate | `malloc(n)` | `new T` / `new T[n]` |
| Free | `free(p)` | `delete p` / `delete[] p` |

In modern C++ you almost never write raw `new`/`delete`. Instead you use
**smart pointers** (lesson 41) and containers (`std::vector`), which free memory
automatically via RAII — the ownership is tied to a scope, so it's impossible to
forget. This lesson shows the manual mechanism so you understand what the smart
pointers are doing for you.

## Build & run
```sh
make run app=41-dynamic-memory      # C++
make run-c app=41-dynamic-memory    # C
```

## Try it yourself
Comment out one `delete[]` and build with `BUILD_TYPE=Release` on Linux under
`valgrind ./binary` — watch it report the leak. (See
`references/debugging-and-tooling.md`.)
