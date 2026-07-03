# 46-templates

## Concept
Templates let you write code once that works for many types. Instead of a
separate `maximum` for `int`, `double`, and `string`, you write one template and
the compiler generates a concrete version for each type you actually use — with
no runtime cost. Templates are the foundation of the entire standard library
(`vector`, `map`, algorithms are all templates).

## Files
```
46-templates/
  include/generic.hpp   <- maximum<T>() and Box<T> (definitions live here)
  src/main.cpp          <- uses them with int/double/string
```

## Line-by-line
- `template <typename T> T maximum(const T& a, const T& b)` — `T` is a type
  placeholder. `maximum(3, 4)` deduces `T=int`; `maximum(1.5, 0.5)` deduces
  `T=double`.
- `template <typename T> class Box` — a class blueprint. `Box<int>` and
  `Box<std::string>` are separate, fully-typed classes.
- Everything is **defined in the header**: the compiler needs the full template
  definition wherever it's used, to stamp out the concrete version. This is why
  there's no `generic.cpp`.

## Common pitfalls
- **Putting template definitions in a `.cpp`** → "undefined reference" at link
  time, because no concrete version was generated. Keep them in headers.
- The requirements on `T` are implicit: `maximum` needs `operator>`. Call it with
  a type that lacks `>` and you get a long error *at the point of use*.
  **Concepts** (lesson 47) make those requirements explicit and the errors
  readable.
- Each distinct type instantiates its own copy — heavy template use grows binary
  size and compile time.

## Build & run
```sh
make run app=46-templates
```

## Try it yourself
Add a `template <typename T> T minimum(const T&, const T&)` to the header and
use it in `main`. Then try calling `maximum` on a type without `operator>` and
read the error.
