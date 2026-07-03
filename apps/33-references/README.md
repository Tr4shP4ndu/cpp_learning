# 24-references

- References in C++ are aliases to existing objects. They are initialized to refer to an object and cannot be reseated.
- Use them to avoid copies, express mutation intent, and enable efficient APIs.

## Kinds of references

- Lvalue reference (T&): binds to lvalues; allows modification.
- Const lvalue reference (const T&): binds to lvalues and temporaries; read-only.
- Rvalue reference (T&&): binds to rvalues (temporaries); used to move from or perfect-forward.

## Binding rules (essentials)

- T& cannot bind to a temporary.
- const T& can bind to a temporary, extending the temporary’s lifetime to the reference’s scope.
- T&& binds only to rvalues; use std::move(x) to treat x as an rvalue.
- Forwarding references (template <class U> void f(U&& u)) bind to both lvalues and rvalues; use std::forward<U>(u).

## References vs pointers

- References must be initialized and cannot be null or reseated.
- Pointers can be null, reseated, and require * and & for access/binding.

## Examples

// Mutating via lvalue reference
void increment(int& x) { x += 1; }

// Read-only via const reference (avoids copy of large object)
void print(const std::string& s) { std::cout << s << "\n"; }

// Move-taking overload using rvalue reference
void setName(std::string&& dst, std::string src) { /* bad: dst is a copy */ }

struct Person {
  std::string name;
  void setName(std::string n) { name = std::move(n); }            // by value + move-in
  void setNameRef(std::string&& n) { name = std::move(n); }       // rvalue ref overload
};

// Perfect forwarding
template <class Fn, class... Args>
decltype(auto) call(Fn&& fn, Args&&... args) {
  return std::forward<Fn>(fn)(std::forward<Args>(args)...);
}

int main() {
  int v = 10; increment(v); // v becomes 11
  print(std::string("hello")); // binds temporary to const ref

  Person p; std::string s = "Ada";
  p.setName(s);            // copies from lvalue
  p.setName("Grace"s);     // moves from temporary (C++14 suffix)
}

## Lifetime extension

- A temporary bound to a const T& lives as long as the reference.
- No extension for binding to T&& stored in a named variable; beware of dangling when moving out of temporaries.

## Reference collapsing (advanced)

- In templates and auto, & and && collapse following rules: & + & -> &, & + && -> &, && + && -> &&.
- auto&& often models “forwarding references” and binds to either value-category.

## Pitfalls and tips

- Don’t return references to local variables (dangling references).
- Avoid binding non-const T& to temporaries (not allowed).
- When storing references as members, ensure the referred-to object outlives the reference.
- Prefer const T& for read-only large inputs; T& for intended mutation; T&& for move sinks; or take by value and std::move when appropriate.

## Build and run (from repository root)

Run these from the repository root:
  - make build app=24-references
  - make run app=24-references

Binary path: build/24-references/bin/24-references

Alternative (from inside this folder):
  - cd app/24-references
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
