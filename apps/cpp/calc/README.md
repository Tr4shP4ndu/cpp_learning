# calc

Evaluates arithmetic expressions with `+ - * /` and parentheses, respecting
precedence — a **recursive-descent parser**, where each grammar rule
(`expr`, `term`, `factor`) is a function that calls the others. The clearest
real example of why recursion matters.

**Builds on:** recursion (30), strings (20), error-handling/exceptions (61).

## Run
Pipe the expression on stdin — cleanest, since the shell won't split it or
expand `*` as a glob:
```sh
echo '1 + 2 * 3'    | make run-cpp-app-calc   # 7
echo '(1 + 2) * 3'  | make run-cpp-app-calc   # 9
echo '2 * -3.5'     | make run-cpp-app-calc   # -7
echo '10 / (2 - 2)' | make run-cpp-app-calc   # error: division by zero
```
Passing it as an argument also works if you **quote it as one string** (so `*`
and spaces survive the shell):
```sh
make run-cpp-app-calc ARGS='"1 + 2 * 3"'      # 7
```

## Try extending it
- Add `^` for exponentiation (right-associative — a good grammar puzzle).
- Add named constants (`pi`, `e`) or functions (`sqrt`, `sin`).
- Return a `std::variant<double, std::string>` instead of throwing (lesson 59).
