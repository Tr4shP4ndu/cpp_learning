# 07-typedef-aliases

- Typedefs and type aliases let you give readable names to complex types. Modern C++ prefers `using` over `typedef`, especially for templates.

## `typedef` vs `using`

- `typedef ExistingType Alias;`
- `using Alias = ExistingType;` (clearer and works with templates)

Examples:

typedef unsigned long ulong_t; // C-style
using index_t = std::size_t;   // modern alias

## Pointer, reference, and function pointer aliases

// Pointer and reference
using IntPtr = int*;     // pointer to int
using IntRef = int&;     // reference to int
using CStr  = const char*; // C string

// Function pointer: returns int, takes (double, int)
using FuncPtr = int(*)(double, int);

// Using the aliases
int foo(double, int);
FuncPtr p = &foo;
int r = p(3.14, 7);

## Template aliases

// Alias a template with fixed parameters
template <class T>
using Vec = std::vector<T>;

Vec<int> vi;        // std::vector<int>

// Partial application via alias
template <class T>
using HashSet = std::unordered_set<T>;

## Namespace aliases

namespace fs = std::filesystem;
fs::path p{"/tmp"};

## Benefits

- Readability: shorten verbose types (iterators, function signatures).
- Maintainability: change the underlying type in one place.
- Abstraction: expose intent (e.g., UserId, Milliseconds).

## Pitfalls and tips

- Be cautious with pointer/reference aliases: IntPtr a, b; declares two pointers? No—only `a` is a pointer; prefer `using` for clarity and repeat the alias: `IntPtr a; IntPtr b;`.
- Aliases don’t create new types; they’re synonyms. You can’t overload on alias differences.
- Prefer `using` with templates; `typedef` can get unreadable with complex function pointers or templates.
- Don’t over-alias trivial types; meaningful names beat unnecessary indirection.

## Build and run (from repository root)

Run these from the repository root:
  - make build app=07-typedef-aliases
  - make run app=07-typedef-aliases

Binary path: build/07-typedef-aliases/bin/07-typedef-aliases

Alternative (from inside this folder):
  - cd app/07-typedef-aliases
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
