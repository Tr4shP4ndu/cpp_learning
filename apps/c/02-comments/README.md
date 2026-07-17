# 02-comments (C)

Comments are notes for humans that the compiler strips out. C has two forms:
`/* ... */` block comments (any C version) and `// ...` line comments (added in
C99).

## Line-by-line
- The comment above `double_it` is a doc-style comment: a block comment placed
  right above a function to describe what it does.
- `return n * 2;  /* ... */` — a block comment can sit at the end of a line.
- The `/* ... */` block inside `main` spans multiple lines; C89 had only this
  form.
- `// ...` line comments run to the end of the line and are a C99 feature — a
  strict C89 compiler would reject them.

## Common pitfalls
- Block comments do not nest: `/* a /* b */ c */` ends at the first `*/`, and
  `c */` becomes a syntax error.
- Forgetting to close a `/*` swallows all following code until the next `*/`.
- Relying on `//` when your build targets pre-C99 compilers.

## Compared to C++
Identical syntax: C++ has both `//` and `/* */`, and `//` was always part of
C++ (C borrowed it back in C99). See the [C++ version](../../cpp/02-comments/).

## Build & run
```sh
make run-c-app-02-comments
```
