# Debugging & tooling (the senior-dev toolbox)

The tools a working C/C++ developer reaches for beyond the compiler. You don't
need all of these at once — learn them as the need arises.

## Debuggers — `lldb` (macOS) / `gdb` (Linux)
Step through a program, inspect variables, and see where it crashed.
```sh
# Build with -g (this repo's Debug builds already do).
lldb ./build/<app>/Debug-c++23-c17/bin/<app>     # gdb ./... on Linux
(lldb) break set -n main      # gdb: break main
(lldb) run
(lldb) next / step / continue # over / into / resume
(lldb) frame variable         # gdb: info locals
(lldb) bt                     # backtrace: how did we get here?
```
When a program crashes, `bt` after the crash shows the call stack — usually the
fastest way to find the culprit.

## Sanitizers (compiler-built, runtime)
This repo's **Debug builds enable AddressSanitizer + UndefinedBehaviorSanitizer**
(`-fsanitize=address,undefined`). They catch, at runtime, with exact locations:
- **ASan:** out-of-bounds array/heap access, use-after-free, double-free.
- **UBSan:** signed overflow, invalid shifts, null-deref, bad casts.
- **TSan** (`-fsanitize=thread`, separate build): data races between threads.

If a lesson's program aborts with a colorful `==ERROR: AddressSanitizer` report,
read the top frame — that's your bug.

## valgrind (Linux; memcheck)
Finds leaks and invalid memory use without recompiling.
```sh
valgrind --leak-check=full ./build/<app>/.../bin/<app>
```
Slower than ASan but needs no special build. (On macOS, prefer the sanitizers.)

## Compiler Explorer — godbolt.org
Paste code and see the **assembly** it compiles to, side by side, for any
compiler/standard/flags. Great for "is this abstraction really zero-cost?" and
for understanding what the optimizer does.

## clang-tidy / clang-format
- **clang-format** — auto-format to a consistent style: `clang-format -i src/*.cpp`.
- **clang-tidy** — static analysis: flags bugs and non-idiomatic code before you
  run anything: `clang-tidy src/main.cpp -- -std=c++23 -Iinclude`.

## Reading errors
- **Compiler errors** name a file and line — fix the first one; later ones are
  often cascades. Template errors are long; look for the phrase "required from
  here" to find *your* line.
- **Linker errors** ("undefined reference", "duplicate symbol") mean the code
  compiled but definitions are missing or doubled — see lesson 32.

## This repo's knobs
```sh
make run   app=<name>                 # build + run (Debug, sanitizers on)
make build app=<name> BUILD_TYPE=Release   # optimized, no sanitizers
make build app=<name> STD=c++98            # try an older standard
make run   app=<name> CXX=clang++          # switch compiler
```
