# 02-comments

## Concept
Comments are text the compiler ignores. They document intent for the next
person to read the code (often future-you). C and C++ share the same two forms.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `// ...` — single-line comment; runs to the end of the line.
- `/* ... */` — block comment; can span multiple lines. Cannot be nested.
- A comment can follow code on the same line.

Documentation-generator tools (Doxygen) also read special forms like `///` and
`/** ... */`, but those are just conventions on top of the same two comment
kinds.

## Common pitfalls
- Trying to nest block comments: `/* outer /* inner */ */` ends at the **first**
  `*/`, leaving a trailing `*/` that is a syntax error.
- Commenting *what* the code does instead of *why*. `i++; // increment i` adds
  nothing. Explain the reason or the tricky bit instead.
- Leaving large blocks of commented-out code around — delete it; git remembers.
- Never put secrets or credentials in comments.

## C vs C++
Both forms (`//` and `/* */`) work identically in modern C and C++. The `//`
form began as a C++ feature but was adopted into C in the C99 standard, so it's
universal today.

## Build & run
```sh
make run app=02-comments
```

## Try it yourself
Add a block comment above `main` describing what the program does in one
sentence, then a trailing `//` comment explaining *why* `speed` is nudged up.
