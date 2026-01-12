# 20-function-overloading

- You can define multiple functions with the same name but different parameter lists (arity or parameter types). The compiler selects the best match at the call site.

## Overload rules (essentials)

- Signature differences must involve parameter types/arity or cv/ref qualifiers on member functions. Return type alone can't distinguish overloads.
- Default arguments are not part of the signature; avoid creating ambiguity with them.
- Implicit conversions are considered during overload resolution; the “best viable” function is chosen.
- For non-member overloads, place them in the same namespace as the types they operate on (ADL-friendly).

## Examples

// Different arity
int sum(int a, int b) { return a + b; }
int sum(int a, int b, int c) { return a + b + c; }

// Different parameter types
double area(double r) { return 3.14159 * r * r; }          // circle
double area(double w, double h) { return w * h; }           // rectangle

// Value vs reference vs const reference
void print(int x) { std::cout << "int: " << x << "\n"; }
void print(double x) { std::cout << "double: " << x << "\n"; }
void print(const std::string& s) { std::cout << s << "\n"; }

// Member function overloads with const
struct Box {
  int value{};
  int& get() { return value; }           // non-const object: mutable access
  const int& get() const { return value; } // const object: read-only access
};

int main() {
  std::cout << sum(2, 3) << "\n";       // calls sum(int,int)
  std::cout << sum(1, 2, 3) << "\n";   // calls sum(int,int,int)

  print(42);            // int
  print(3.14);          // double
  print(std::string("hi")); // string overload

  Box b; b.value = 5;
  b.get() = 10;         // non-const overload returns int&
  const Box cb{42};
  std::cout << cb.get() << "\n"; // const overload
}

## Overload resolution basics

- Exact match beats conversions; fewer or safer conversions win.
- Promotions (e.g., int -> long) are preferred over user-defined conversions.
- Ambiguity leads to a compile error; disambiguate via casts or different names.

## Pitfalls and tips

- Beware of implicit conversions causing ambiguous calls:
  - print(0) might match int or nullptr_t overloads if both exist.
  - Overloads for bool and integral types can conflict due to implicit conversions.
- Don't rely on return type to distinguish overloads; it doesn't participate.
- Default arguments can introduce ambiguity; prefer separate overloads or explicit calls.
- Overloading and templates: a templated overload may be a worse match than a non-template; constrain templates with concepts or std::enable_if to guide resolution.
- Prefer clear, minimal overload sets; too many overloads harm readability.

## Build and run (from repository root)

Run these from the repository root:
  - make build app=20-function-overloading
  - make run app=20-function-overloading

Binary path: build/20-function-overloading/bin/20-function-overloading

Alternative (from inside this folder):
  - cd app/20-function-overloading
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
