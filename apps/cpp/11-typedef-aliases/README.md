# 11-typedef-aliases

## Concept
A type alias gives a second, readable name to an existing type. It doesn't
create a new type — just a synonym — but it shortens verbose types and lets you
change an underlying type in one place.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `typedef std::string text_t;` — the classic C form: `typedef <type> <alias>;`.
- `using number_t = int;` — the modern C++ form. Reads naturally left-to-right
  and, unlike `typedef`, works with templates.
- `using pair_list_t = std::vector<std::pair<std::string, int>>;` — the payoff:
  a long type becomes one short name.
- `for (const auto& [subject, score] : scores)` — structured bindings unpack
  each pair (a preview of ranged loops, lesson 13).

## `typedef` vs `using`
They're equivalent for simple cases. Prefer `using` in modern C++:
- It reads like an assignment: `using Alias = Type;`.
- It supports *alias templates*: `template<class T> using Vec = std::vector<T>;`.
- Function-pointer aliases are far more readable with `using`.

## Common pitfalls
- An alias is **not** a new distinct type — you can't overload two functions
  that differ only by alias vs underlying type.
- Don't alias trivial types just to alias them; a good name earns its keep
  (`using UserId = int;` documents intent; `using MyInt = int;` doesn't).

## Build & run
```sh
make run-cpp-app-11-typedef-aliases
```

## Try it yourself
Add `using Celsius = double;` and a variable of that type. Note it behaves
exactly like a `double`.
