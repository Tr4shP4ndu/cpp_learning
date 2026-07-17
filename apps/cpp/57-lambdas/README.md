# 55-lambdas

## Concept
A lambda is an anonymous function you define inline, right where you use it.
They're the natural way to pass behavior to standard algorithms (`sort`,
`count_if`, `for_each`) without writing a separate named function.

## Minimal example
See `src/main.cpp`.

## Syntax
```
[captures](parameters) -> ReturnType { body }
```
The return type is usually deduced, so you omit `-> ReturnType`.

## Line-by-line
- `auto square = [](int x) { return x * x; };` — a lambda with no captures,
  stored in a variable and called like a function.
- `[threshold](int x) { return x > threshold; }` — **captures** `threshold` by
  value (a copy). The lambda can now use an outer variable.
- `std::count_if(begin, end, over)` — pass the lambda as the predicate.
- `[](int a, int b){ return a > b; }` — an inline comparator for `std::sort`
  (descending).
- `[&total](int x){ total += x; }` — capture **by reference** to modify an outer
  variable.

## Captures
- `[x]` — copy `x` in.
- `[&x]` — refer to `x` (changes affect the original).
- `[=]` / `[&]` — capture everything used, by value / by reference.
- Prefer naming exactly what you need; `[&]` with dangling references is a
  common bug.

## Common pitfalls
- **Dangling captures:** a lambda that captures a reference (or `[&]`) and
  outlives the referenced variable reads freed memory.
- Capturing a big object by value copies it every time the lambda is created.
- Lambdas are **C++11**; generic lambdas (`auto` params) are C++14.

## Build & run
```sh
make run-cpp-app-55-lambdas
```

## Try it yourself
Use `std::transform` with a lambda to build a new vector of each element
doubled, then print it.
