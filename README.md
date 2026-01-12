# cpp_learning

A tiny, vendor-free C++ workspace that stays out of your way.

- Single top-level `Makefile` to scaffold apps and build/run them.
- Each app lives under `apps/<name>` with its own `Makefile` and `src/main.cpp`.
- No submodules, no external build systems required.

See Makefile usage details in `Makefile.md`.

## Requirements

- macOS: Xcode Command Line Tools (for `clang++`). Install with `xcode-select --install`.
- Linux: `g++` or `clang++`.
- Windows: MSVC, MinGW, or Clang (optional; this repo still builds per‑app `Makefile`).

## Quick start

- List apps

	```sh
	make list
	```

- Scaffold a new app (uses templates in `templates/app/`)

	```sh
	make app app=my-new-app
	```

- Build an app

	```sh
	make build app=my-new-app
	```

- Run an app

	```sh
	make run app=my-new-app
	```

- Clean build artifacts (keeps the `build/` directory)

	```sh
	make clean
	```

- Delete an app and its build output

	```sh
	make delete-app app=my-new-app
	```

Tips:
- `make help` prints available targets and brief usage.
- App names should use letters, digits, `-` or `_` (no spaces/shell specials).
- Generated apps target C++20; tweak `CXXFLAGS` in the app `Makefile` if needed.

## Notes

- Source lives in `apps/<name>/src/`.
- Binaries are placed centrally under `build/<name>/bin/<name>`.
- Use either `clang++` or `g++`; override with `CXX=g++` or `CXX=clang++` on the make command line.
- Minimal by design—focus on C++ concepts, not tooling.
