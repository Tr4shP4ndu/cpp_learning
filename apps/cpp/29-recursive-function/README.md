# 29-recursive-function

## Concept
A recursive function calls *itself* on a smaller version of the problem until it
reaches a case simple enough to answer directly. It's a natural fit for problems
defined in terms of themselves (factorials, tree traversal, divide-and-conquer).

## Minimal example
See `src/main.cpp`.

## The two required parts
1. **Base case** — the stopping condition (`n <= 1 → return 1`). Without it the
   recursion never ends.
2. **Recursive case** — call yourself on a *smaller* input (`factorial(n - 1)`)
   and combine the result. Each step must move toward the base case.

## Line-by-line
- `factorial(5)` expands to `5 * factorial(4)` → `5 * 4 * factorial(3)` → … →
  reaches `factorial(1) == 1`, then multiplies back up: `1, 2, 6, 24, 120`.
- The `assert` lines double as a self-check (they'll abort the program if the
  logic is wrong).
- Return type is `long long` — factorials overflow `int` fast (13! already
  exceeds a 32-bit int).

## Common pitfalls
- **Missing/incorrect base case** → infinite recursion → stack overflow (crash).
- **Not shrinking the input** (e.g. calling `factorial(n)` instead of `n - 1`) →
  same crash.
- **Deep recursion** can exhaust the call stack; some recursions are better
  rewritten as loops (this one easily could be).
- **Overflow:** the *math* overflows long before the recursion is deep.

## Build & run
```sh
make run app=29-recursive-function
```

## Try it yourself
Write a recursive `int fib(int n)` for Fibonacci (`fib(0)=0`, `fib(1)=1`,
`fib(n)=fib(n-1)+fib(n-2)`) and `assert(fib(7) == 13);`.
