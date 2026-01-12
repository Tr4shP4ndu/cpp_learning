# 25-dereferencing

- Dereferencing a pointer reads or writes the object it points to. Use the unary `*` operator (or `->` for member access) to reach the pointed-to object.

## Basics

- Address-of: `&x` gives the address of `x` (a pointer).
- Dereference: `*p` gives the object referred to by `p`.
- Member access through pointer: `p->m` is `(*p).m`.
- Array subscripting: `p[i]` is syntactic sugar for `*(p + i)`.

## Examples

// Integers
int x = 10; int* p = &x; *p = 20; // x becomes 20

// Struct member access
struct S { int a; } s{1}; S* sp = &s;
(*sp).a = 2; // explicit deref
sp->a = 3;   // equivalent and idiomatic

// Arrays and pointer arithmetic
int arr[3] = {1,2,3}; int* pa = arr; // decay to pointer to first element
int second = *(pa + 1);   // 2
int third  = pa[2];       // 3

// Smart pointers (preferred for ownership)
auto up = std::make_unique<int>(42);
int v = *up;              // dereference unique_ptr

// Iterators use * too
std::vector<int> vec{1,2,3};
int first = *vec.begin(); // 1

## Const correctness with pointers

- `const T*` (pointer to const): you cannot modify `*p` through this pointer.
- `T* const` (const pointer): you cannot reseat `p`, but you can modify `*p`.
- `const T* const`: neither reseat nor modify through `p`.

// Example
int y = 5; const int* pc = &y; // pointer to const int
//*pc = 7; // error: cannot modify through pointer-to-const
int* const cp = &y; *cp = 7;   // ok: const pointer to non-const int

## Operator precedence tips

- `*p.member` is parsed as `*(p.member)`; use `(*p).member` or `p->member`.
- `*p++` dereferences the original pointer, then increments the pointer.
- `(*p)++` increments the referred-to value.
- Prefer parentheses to make intent explicit.

## Safety and pitfalls

- Never dereference a null, uninitialized, or dangling pointer.
  - Initialize pointers upon declaration; set to `nullptr` when empty.
  - Check `if (p)` before dereferencing when null is possible.
- Ensure lifetime: the pointed-to object must outlive all dereferences.
- Avoid pointer arithmetic except within arrays you own; it’s easy to go out of bounds (UB).
- Prefer smart pointers (`std::unique_ptr`, `std::shared_ptr`) for ownership and automatic deletion.
- For non-owning references, prefer references (`T&`, `const T&`) or `std::span`/`std::string_view` for ranges/views.

## Build and run (from repository root)

Run these from the repository root:
  - make build app=25-dereferencing
  - make run app=25-dereferencing

Binary path: build/25-dereferencing/bin/25-dereferencing

Alternative (from inside this folder):
  - cd app/25-dereferencing
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
