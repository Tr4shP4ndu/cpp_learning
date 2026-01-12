# 06-namespaces

-  A namespace is a declarative region that provides a scope to the identifiers (names of types, functions, variables, etc.) inside it. Namespaces help avoid name conflicts in large projects by allowing entities with the same name to coexist as long as they are in different namespaces.

## Why use namespaces

- Avoid name collisions between libraries and modules.
- Group related APIs for clarity (e.g., math::, net::, io::).
- Enable argument-dependent lookup (ADL) for free functions associated with a type.

## Declaring and using

// Basic declaration and definition
namespace math {
  int add(int a, int b);
}

int math::add(int a, int b) { return a + b; }

// Nested namespaces (C++17 shorthand)
namespace company::util {
  void log(const char*);
}

// Use with scope resolution
int s = math::add(2, 3);

## using declarations vs using namespace

- using declaration imports one name: `using math::add;` (prefer local scope).
- using directive imports all names: `using namespace math;` (avoid in headers; limit to small function scope if used).

## Namespace aliases

// Shorten long names
namespace fs = std::filesystem;
fs::path p{"/tmp"};

## Anonymous (unnamed) namespaces

- `namespace { ... }` gives internal linkage to its contents (visible only in the current translation unit).
- Good for file-local helpers in .cpp files.
- Avoid in headers for non-inline definitions; it creates a separate entity in each translation unit.

## Inline namespaces (versioning)

// Default-visible version
namespace api {
  inline namespace v2 { int parse(const std::string&); } // preferred
  namespace v1 { int parse(const char*); }               // legacy
}

// Unqualified lookup finds api::v2::parse; older remains accessible via api::v1::parse.

## ADL (Argument-Dependent Lookup)

- Free functions are found based on the namespaces of their arguments.
- Put non-member operators and helper functions in the same namespace as the type they operate on.

// Example: operator<< for a user type in its namespace
namespace model {
  struct Point { int x{}, y{}; };
  inline std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << p.x << "," << p.y;
  }
}

// std::cout << model::Point{1,2}; // finds operator<< via ADL

## Best practices and pitfalls

- Do not write `using namespace` in headers; it pollutes consumers' namespaces.
- Never add symbols to `std`; alias or wrap instead (undefined behavior to specialize many std entities incorrectly).
- Wrap your project in a root namespace (and possibly `detail` for non-API internals).
- Prefer `using` declarations in function scope over directives at global scope.
- In headers, prefer declarations; put definitions in .cpp to avoid ODR issues. For header-only, use `inline` where needed.

## Build and run (from repository root)

Run these from the repository root:
  - make build app=06-namespaces
  - make run app=06-namespaces

Binary path: build/06-namespaces/bin/06-namespaces

Alternative (from inside this folder):
  - cd app/06-namespaces
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
