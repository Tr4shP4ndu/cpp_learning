# 24-files-and-filesystem

## Concept
Real programs read and write files. C++ uses *file streams* (`std::ofstream` /
`std::ifstream`) that work just like `std::cout`/`std::cin`, plus
`std::filesystem` for paths and file metadata. C uses `FILE*` handles with
`fopen`/`fprintf`/`fgets` and treats paths as plain strings.

## Minimal example
See `src/main.cpp`. The C version of this lesson lives in [`apps/c/24-files-and-filesystem`](../../c/24-files-and-filesystem/) (run it with `make run-c app=24-files-and-filesystem`).

## Line-by-line (C++)
- `fs::path file = fs::temp_directory_path() / "...";` — `path` joins segments
  with the right separator for the OS; `/` is overloaded for joining.
- `std::ofstream out(file);` — open for writing (creates the file). Writing uses
  the same `<<` you already know.
- The `{ }` block around the stream closes it via **RAII** — leaving scope
  flushes and saves the file automatically. No explicit `close()` needed.
- `std::getline(in, line)` — read back one line at a time.
- `fs::exists`, `fs::file_size`, `file.filename()` — filesystem queries.

## Line-by-line (C)
- `FILE *out = fopen(path, "w");` — returns a handle or `NULL`; **always check**.
- `fprintf(out, ...)` — like `printf` but to the file.
- `fclose(out);` — required to flush and release. C has no RAII, so forgetting
  this can lose data.
- `fgets(line, sizeof line, in)` — read a line safely with a size bound.

## Common pitfalls
- **Not checking that the file opened.** A missing file or permission error
  gives a failed stream / `NULL` handle; writing to it silently does nothing.
- **C:** forgetting `fclose` → buffered data never hits disk.
- Text vs binary mode matters on Windows (`"wb"`/`"rb"` for binary).
- `std::filesystem` needs C++17.

## C vs C++
| | C | C++ |
|---|---|---|
| Open | `fopen(path,"w")` → check `NULL` | `std::ofstream out(path)` |
| Write | `fprintf(f, ...)` | `out << ...` |
| Close | manual `fclose` | automatic (RAII, scope exit) |
| Paths | raw strings | `std::filesystem::path` |

RAII is the headline: the C++ stream closes itself when it leaves scope, so you
can't forget. This is the same idea you'll formalize in lesson 46.

## Build & run
```sh
make run app=24-files-and-filesystem      # C++
make run-c app=24-files-and-filesystem    # C
```

## Try it yourself
Open the file in append mode instead of overwrite: `std::ofstream out(file,
std::ios::app);` (C++) or `fopen(path, "a")` (C). Run twice and watch it grow.
