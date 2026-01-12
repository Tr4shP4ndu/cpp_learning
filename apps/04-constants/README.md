# 04-constants

- Constants express intent that a value won’t change and enable optimizations and safety.

## const (read-only)

- `const` makes an object read-only through that name. It can still be copied or moved.
- At namespace scope, `const` has internal linkage by default; prefer `inline constexpr` (C++17) for header constants.

// Examples
const double Pi = 3.141592653589793;
const std::string AppName = "demo";

## constexpr (compile-time constant)

- `constexpr` means the value can be evaluated at compile time and used where a constant expression is required (array bounds, non-type template args, case labels).
- Functions can be `constexpr` if they can be evaluated at compile time given constant arguments.

// Examples
constexpr int BufSize = 1024;
constexpr int square(int x) { return x * x; }
static_assert(square(5) == 25);

## consteval (immediate) and constinit (initialization)

- `consteval` forces compile-time evaluation (function must return a constant expression).
- `constinit` guarantees initialization at compile time for static storage duration but does not imply constness.

// Examples
consteval int id(int x) { return x; }
constexpr int ID = id(42);

constinit int global_counter = 0; // not const; just initialized at compile-time

## Pointers and const

- `const T*` (pointer to const T): cannot modify the pointed-to object via this pointer.
- `T* const` (const pointer): cannot reseat the pointer; can modify the object.
- `const T* const`: neither reseat nor modify through the pointer.

int v = 1;
const int* pc = &v;   // pointer to const
int* const cp = &v;   // const pointer
const int* const cpc = &v; // both

## const member functions and fields

- Mark member functions `const` when they don’t modify observable state.
- Data members can be `const`; for lazily updated caches, use `mutable` to allow modification in const functions (use sparingly).

struct Point {
  int x{}, y{};
  int sum() const { return x + y; } // does not modify this
};

## Literals and suffixes

- Integer: 42u (unsigned), 42l/42ll (long/long long).
- Floating: 1.0f (float), 1.0 (double), 1.0l (long double).
- Char/String prefixes: u8"...", u"...", U"...", L"..."; also use std::string_view for lightweight constant strings.

## Where constant expressions are required

- Array bounds: `int a[BufSize];`
- Case labels in switch
- Non-type template parameters: `std::array<int, BufSize>`
- Enum values and bit-fields

## Pitfalls and tips

- Casting away const and then modifying a truly-const object is undefined behavior; avoid `const_cast` for mutation.
- Prefer `constexpr` over macros for compile-time constants; type-safe and scoped.
- For header constants, prefer `inline constexpr` to avoid multiple definitions.
- `const` does not imply thread-safety by itself; synchronize shared data.

## Build and run (from repository root)

Run these from the repository root:
  - make build app=04-constants
  - make run app=04-constants

Binary path: build/04-constants/bin/04-constants

Alternative (from inside this folder):
  - cd app/04-constants
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
