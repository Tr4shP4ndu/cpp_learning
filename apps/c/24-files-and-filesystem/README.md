# 24-files-and-filesystem (C)

The C version of [`apps/cpp/24-files-and-filesystem`](../../cpp/24-files-and-filesystem/).
Writes a file, reads it back — using `FILE*` handles.

## Line-by-line
- `FILE *out = fopen(path, "w");` — returns a handle or `NULL`; **always check**.
  `"w"` truncates/creates for writing.
- `fprintf(out, ...)` — like `printf`, but to the file.
- `fclose(out);` — required to flush and release. C has no RAII, so forgetting
  this can lose data.
- `fgets(line, sizeof line, in)` — read back a line safely.
- `remove(path)` — delete the file.

## Common pitfalls
- Not checking `fopen` for `NULL`.
- Forgetting `fclose` → buffered data never reaches disk.
- Text vs binary mode (`"wb"`/`"rb"`) matters on Windows.

## Compared to C++
C++ file streams close themselves on scope exit (RAII) and use
`std::filesystem::path` for paths. See the [C++ version](../../cpp/24-files-and-filesystem/).

## Build & run
```sh
make run-c app=24-files-and-filesystem
```
