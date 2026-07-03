# Debugging & tooling (the senior-dev toolbox)

The tools a working C/C++ developer reaches for. This is a stub — it will be
filled in during the capstone phase. Planned contents:

- **Debuggers** — `lldb` (macOS) / `gdb` (Linux): breakpoints, backtraces, watchpoints.
- **Sanitizers** — AddressSanitizer & UndefinedBehaviorSanitizer (on by default
  in this repo's Debug builds); ThreadSanitizer for data races.
- **valgrind** — memory-leak and invalid-access detection on Linux.
- **Compiler Explorer (godbolt.org)** — see the assembly your code compiles to.
- **clang-tidy / clang-format** — static analysis and consistent formatting.
- **Reading compiler errors** — how to parse template and linker errors.
