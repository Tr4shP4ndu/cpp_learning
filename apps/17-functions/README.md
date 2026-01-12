# 17-functions

- Functions are reusable blocks of code that perform specific tasks. Functions help to structure your program, avoid repetition, and make code more modular and readable.
- Components of a Function:
  - Return type: The data type of the value the function returns (e.g., int, double, void).
  - Function name: A unique identifier to call the function.
  - Parameters: Optional inputs that the function accepts (also called arguments).
  - Function body: The block of code that defines what the function does.
  - Return statement: Specifies the value to return (if applicable).

## Declaring vs defining

- Declaration (prototype): tells the compiler the function’s signature.
- Definition: provides the actual implementation.

```cpp
int add(int a, int b);        // declaration
int add(int a, int b) {       // definition
  return a + b;
}
```

## Passing arguments

- Pass-by-value (copy):
```cpp
void foo(int x) { /* x is a copy */ }
```

- Pass-by-reference (modify caller’s object):
```cpp
void inc(int& x) { ++x; }
```

- Pass-by-const-reference (read-only, no copy):
```cpp
int sum(const std::vector<int>& v) {
  int s = 0; for (auto x : v) s += x; return s;
}
```

## Overloading

Same name, different parameter types or counts:
```cpp
int print(int x);
double print(double x);
```

## Default arguments

```cpp
int power(int base, int exp = 2) { return /* base^exp */; }
```

## Inline and headers

- Put declarations in headers (`.hpp` or `.h`), definitions in `.cpp`.
- Small, frequently-used functions can be marked `inline` (or defined in headers) to allow ODR-safe multiple translation units.

## Return values and void

- `void f()` means no return value.
- Use `return;` to exit early from `void` functions.
- Prefer returning values over output-parameters where practical.

## Small example

```cpp
#include <iostream>

int add(int a, int b) { return a + b; }
void greet(const std::string& name) { std::cout << "Hello, " << name << "\n"; }

int main() {
  std::cout << add(2, 3) << "\n";   // 5
  greet("Ada");
}
```

## Build and run (from repository root)

Run these from the repository root:
  - make build app=17-functions
  - make run app=17-functions

Binary path: build/17-functions/bin/17-functions

Alternative (from inside this folder):
  - cd app/17-functions
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
