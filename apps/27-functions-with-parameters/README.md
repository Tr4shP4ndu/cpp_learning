# 19-functions-with-parameters

- Functions take parameters to operate on input and optionally return results. Parameters define the function's interface and how data is passed.

## Basics

- Declaration: return_type name(parameter_list);
- Definition: provides the function body.
- Call: supply arguments matching the parameter list (type and order).

Example:

int add(int a, int b) { return a + b; }
int s = add(3, 4); // s == 7

## Passing strategies

- Pass by value: copies the argument.
  - Safe and simple; modifications don't affect the caller.
  - Use for small, trivially copyable types (int, double, small structs).
- Pass by reference (T&): no copy; refers to the caller's object.
  - Allows modifying the caller's object; efficient for large types.
  - Be explicit in intent; name parameters clearly.
- Pass by const reference (const T&): no copy; read-only access.
  - Best for large types when you don't need to modify.
- Pass by pointer (T*): can be null and requires dereferencing.
  - Use only when nullability is meaningful; check for nullptr.

Examples:

void increment(int& x) { x += 1; }
void print(const std::string& s) { std::cout << s << "\n"; }
void maybeSet(int* p, int v) { if (p) *p = v; }

## Default arguments

- Provide defaults in the declaration to make parameters optional.
- Defaults must be specified once (commonly in the header) and trailing.

int repeat(std::string s, int times = 2) { /* ... */ }

## Overloading

- Multiple functions with the same name but different parameter types/arity.
- Prefer explicitness; avoid ambiguous overloads.

int sum(int a, int b);
double sum(double a, double b);

## const correctness

- Mark parameters and member functions const when they should not modify state.
- Prefer const T& for large read-only inputs; copy small types.

## Pitfalls and tips

- Avoid unnecessary copies; use references for heavy types.
- Don't return references to local variables (dangling).
- Validate pointer parameters before dereferencing; prefer references if non-null.
- For output parameters, consider returning a value (or struct) instead of mutating inputs; if mutation is needed, use T& and name clearly (e.g., outValue).
- Use std::span for array-like inputs when appropriate (C++20).

## Small example

// Adds two values (by value)
int add(int a, int b) { return a + b; }

// Appends suffix without copying the whole string (const reference)
void showWith(const std::string& base, const std::string& suffix) {
  std::cout << base << suffix << "\n";
}

// Mutates the passed value (reference)
void scale(int& value, int factor) { value *= factor; }

// Optional parameter via default arg
int clamp(int x, int lo = 0, int hi = 100) {
  return std::min(std::max(x, lo), hi);
}

int main() {
  int s = add(2, 5);
  std::cout << "sum: " << s << "\n";

  std::string name = "Alice";
  showWith(name, "!"); // name is not modified

  int v = 10;
  scale(v, 3); // v becomes 30
  std::cout << "scaled: " << v << "\n";

  std::cout << clamp(120) << "\n";    // 100
  std::cout << clamp(-5, -10, 10) << "\n"; // -5
}

## Build and run (from repository root)

Run these from the repository root:
  - make build app=19-functions-with-parameters
  - make run app=19-functions-with-parameters

Binary path: build/19-functions-with-parameters/bin/19-functions-with-parameters

Alternative (from inside this folder):
  - cd app/19-functions-with-parameters
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
