# cpp_learning

A tiny, vendor-free C++ workspace that stays out of your way.

- Single top-level `Makefile` to generate new apps and to build/run them cross‑platform.
- Each app has its own `Makefile` and `src/` with simple `main.cpp`.
- No submodules, no external build systems required.

See Makefile usage details in `Makefile.md`.

## Requirements

- macOS: Xcode Command Line Tools (for `clang++`). Install with `xcode-select --install`.
- Linux: `g++` or `clang++`.
- Windows: MSVC, MinGW, or Clang (optional; this repo still builds per‑app `Makefile`).

## Quick start

List apps:

```sh
make list
```

Create a new app scaffold:

```sh
make app NAME=hello
```

Build an app:

```sh
make build APP=hello
```

Run an app:

```sh
make run APP=hello
```

Interactive mode (when APP is omitted):

```sh
make build
make run
```

The generated app uses C++20 by default; adjust `CXXFLAGS` in the app's `Makefile` as you like.

## Notes

- Binaries live under `app/<name>/build/bin/<name>`.
- You can use either `clang++` or `g++`. Set `CXX=g++` (or `clang++`) when invoking make if you want to override.
- This setup is intentionally minimal to focus on learning C++ rather than toolchains.
