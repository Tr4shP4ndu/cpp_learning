# 31-header-source-separation

## Concept
Real programs are split across many files. The convention: **headers** (`.hpp`
/ `.h`) declare *what* exists (the interface); **source files** (`.cpp` / `.c`)
define *how* it works (the implementation). Other files `#include` the header to
use the code without seeing its internals. This is the first lesson with a real
`include/` + `src/` layout.

## Files
```
31-header-source-separation/
  include/greeter.hpp   <- declaration (interface)
  src/greeter.cpp       <- definition  (implementation)
  src/main.cpp          <- consumer    (uses the interface)
```

## Line-by-line
- **`greeter.hpp`** declares `std::string greet(const std::string&);` — signature
  only, no body.
- **`greeter.cpp`** `#include`s its own header and provides the one definition.
- **`main.cpp`** `#include`s the header and calls `greet()`. It never sees
  `greeter.cpp` — the two are joined later by the *linker* (lesson 32).
- `#pragma once` at the top of the header prevents it from being pasted in twice
  within one file.

## Why split at all
- **Faster builds:** change `greeter.cpp` and only it recompiles; every file that
  includes the header need not.
- **Encapsulation:** consumers depend on the stable interface, not the details.
- **Reuse:** many `.cpp` files can share one header.

## The One Definition Rule (ODR)
A function/variable may be *declared* many times but **defined exactly once**
across the whole program. Put declarations in headers (included widely) and
definitions in a single `.cpp`. Defining a function *in* a header that's
included by multiple files violates the ODR (fix: mark it `inline`, or move it
to a `.cpp`).

## Include guards
`#pragma once` (widely supported) or the classic form:
```cpp
#ifndef GREETER_HPP
#define GREETER_HPP
// ...declarations...
#endif
```
Both stop double-inclusion. Without a guard, including a header twice re-pastes
its contents and causes "redefinition" errors.

## `#include "..."` vs `#include <...>`
- `"greeter.hpp"` — your project's headers (searched near the file first, then
  the `-I` paths).
- `<iostream>` — system/library headers (searched in the compiler's include
  paths).

The per-app Makefile adds `-Iinclude` so `#include "greeter.hpp"` resolves.

## Build & run
```sh
make run app=31-header-source-separation
```
The Makefile compiles `main.cpp` and `greeter.cpp` separately, then links them.

## Try it yourself
Add a second function `int add(int, int)` — declare it in `greeter.hpp`, define
it in `greeter.cpp`, and call it from `main.cpp`. Notice `main.cpp` didn't need
to change how it's built.
