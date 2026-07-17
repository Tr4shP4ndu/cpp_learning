# 33-command-line-arguments (C)

`int main(int argc, char *argv[])` receives the words typed on the command line.
`argc` is how many there are; `argv` is the array of them, with `argv[0]` being
the program's own name. Everything arrives as text.

## Line-by-line
- `int main(int argc, char *argv[])` — the standard signature for a program that
  reads arguments.
- The loop prints every `argv[i]` from `0` to `argc-1`; `argv[argc]` is `NULL`.
- `strtol(argv[1], &end, 10)` — converts the first argument to a `long` in base
  10; `end` points at the first unconverted character.
- `*end == '\0'` confirms the whole string was a valid number.

## Common pitfalls
- Reading `argv[1]` without first checking `argc > 1` — out-of-bounds access.
- Treating arguments as numbers directly; they are strings, convert with
  `strtol`/`atoi` (`strtol` reports errors, `atoi` does not).
- Forgetting `argv[0]` is the program name, so real arguments start at index 1.

## Compared to C++
C++ uses the exact same `main(int argc, char *argv[])` signature; higher-level
code often wraps it in a `std::vector<std::string_view>` for convenience, but the
entry point is identical.

Our runner forwards arguments after `ARGS=`:

```sh
make run-c-app-33-command-line-arguments ARGS='alpha 42 beta'
```

## Build & run
```sh
make run-c-app-33-command-line-arguments
```
