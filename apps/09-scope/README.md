# 05-scope

- Scope controls where names are visible and where objects live. In C++, scope and lifetime are related but not identical.

## Kinds of scope

- Block scope: names declared inside `{}` are visible only within that block.
- Function parameter scope: parameters are visible within the function body.
- Namespace (global) scope: names declared at file/namespace level.
- Class/struct scope: members are visible via `.`/`->` or qualified names.
- Enum scope: scoped enums (`enum class`) contain their enumerators.
- Statement init-scope (C++17): `if`, `switch`, and range-for allow an init statement with variables scoped to the statement.

## Shadowing (name hiding)

int x = 1;           // namespace/global
{
  int x = 2;         // shadows outer x within this block
  std::cout << x;    // 2
}
std::cout << ::x;    // 1 (use scope-resolution to access global)

Avoid shadowing when it harms readability.

## Lifetime vs scope

- Automatic storage (locals): created at declaration, destroyed when leaving scope.
- Static storage: lives for the entire program (globals, static locals, static data members).
- Dynamic storage: lifetime managed manually or by smart pointers (new/delete, unique_ptr, shared_ptr).

Static local example (persists across calls):

int counter() {
  static int c = 0; // initialized once
  return ++c;       // retains value between calls
}

## RAII and scope exit

- Objects are destroyed automatically at scope exit; use this for resource management (files, locks, memory).
- Prefer RAII types (std::unique_ptr, std::lock_guard, std::fstream) to tie lifetime to scope.

## for/if/switch scopes

for (int i = 0; i < 3; ++i) { /* i is only visible here */ }

if (auto it = someMap.find(k); it != someMap.end()) { /* it visible here */ }
else { /* it not visible here */ }

switch (int code = getCode(); code) {
  case 0: /* code visible in switch */ break;
}

## Lambdas and capturing

- Captures bring outer-scope variables into the lambda’s body.
- `[=]` copies by value; `[&]` captures by reference (mind lifetimes!).
- Prefer minimal explicit captures: `[&x, y]`.

## Best practices

- Minimize scope: declare variables at first use.
- Avoid global variables; prefer dependency injection and function parameters.
- Don’t shadow names from outer scopes unless there’s clear benefit.
- Use braces to create limited scopes for temporaries and to control destruction order.

## Build and run (from repository root)

Run these from the repository root:
  - make build app=05-scope
  - make run app=05-scope

Binary path: build/05-scope/bin/05-scope

Alternative (from inside this folder):
  - cd app/05-scope
  - make run

This uses the per-app Makefile and still outputs to the centralized top-level build/ folder.
