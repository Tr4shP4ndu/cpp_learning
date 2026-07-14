# 58-format

## Concept
`std::format` (C++20) is type-safe, Python-style string formatting: a format
string with `{}` placeholders and arguments filled in order. It replaces both
`printf` (which is not type-safe — the wrong `%` specifier is undefined behavior)
and long `<<` chains, and it **returns a `std::string`** you can store, not just
print.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `std::format("hello, {}!\n", who)` — `{}` is a placeholder filled by the next
  argument; the type is deduced, so there's no `%s`/`%d` to get wrong.
- `"{0} + {0} = {1}"` — **positional** indices let you reuse and reorder
  arguments.
- The part after `:` is the **format spec**:
  - `{:>8}` / `{:<8}` — right- / left-align within width 8,
  - `{:.3f}` — fixed-point with 3 decimals,
  - `{:#x}` — hexadecimal with a `0x` prefix,
  - `{:04d}` — integer zero-padded to width 4.
- `std::string line = std::format(...)` — the result is a value you can keep,
  log, or return.

## Why this matters
One consistent, type-safe syntax for both stream output and building strings.
Mistakes that were runtime UB with `printf` become **compile-time** errors: the
format string is checked against the argument types.

## Common pitfalls
- Needs **C++20** and a standard library that ships `<format>` (this repo's
  clang/libc++ does).
- A literal brace must be doubled: `{{` and `}}`.
- A format spec that doesn't match the argument type is a compile-time error —
  which is the feature, not a bug.

## Build & run
```sh
make run app=58-format
make build app=58-format STD=c++17   # <format> is C++20 — fails
```

## Try it yourself
Format a small table: loop over a few `{name, score}` pairs and print each with
`std::format("{:<10}{:>5}\n", name, score)` so the columns line up.
