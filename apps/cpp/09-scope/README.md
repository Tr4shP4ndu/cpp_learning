# 09-scope

## Concept
Scope is the region of code where a name is visible. A variable declared inside
a `{ }` block exists only within that block; when the block ends, the variable
is gone. This keeps names from clashing and ties an object's lifetime to where
it lives.

## Minimal example
See `src/main.cpp`.

## Line-by-line
- `int x = 10;` in `main` — visible for the rest of `main`, including nested
  blocks.
- The inner `{ ... }` introduces a new block scope; `y` lives only inside it.
- `x` is still visible inside the inner block (inner scopes can see outer names).
- The commented-out `std::cout << y` outside the block would not compile — `y`
  is out of scope there.

## Kinds of scope
- **Block scope** — inside `{ }` (including loop and `if` bodies).
- **Function scope** — parameters, for the whole function body.
- **Namespace/global scope** — declared at file level (lesson 10).
- **Class scope** — members of a struct/class (lesson 45).

## Lifetime vs scope
Local variables use *automatic storage*: created at their declaration,
destroyed at the closing `}`. This is the foundation of RAII (lesson 46) —
resources released automatically when their owner leaves scope. A `static`
local, by contrast, is created once and persists across calls.

## Common pitfalls
- **Shadowing:** an inner variable with the same name hides the outer one. Legal
  but confusing — avoid unless intentional.
- Declaring variables far from first use widens their scope needlessly. Declare
  them where you use them.
- Returning a pointer/reference to a local variable is a bug — it's destroyed at
  scope exit (the sanitizer catches this).

## Build & run
```sh
make run app=09-scope
```

## Try it yourself
Add a second nested block with its own `int x` and print it — observe that it
shadows the outer `x` only inside that block.
