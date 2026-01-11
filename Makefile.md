# Makefile guide

This workspace uses a single, vendor‑free Makefile to scaffold, build, and run small C++ apps consistently across OSes.

## Layout
- Apps live under `app/<name>/` with their own `src/` and per‑app `Makefile`.
- All build artifacts are centralized under `build/<name>/...` (outside each app folder).
  - Binary path: `build/<name>/bin/<name>`

## Common variables
- `APP_DIR` (top‑level): defaults to `app`.
- `CXX`: C++ compiler (`clang++` or `g++`). Can be overridden: `make build APP=foo CXX=g++`.
- `CXXFLAGS`: defaults to `-std=c++20 -O2 -Wall -Wextra -Wpedantic`.
- `LDFLAGS`: linker flags (empty by default).
- `DETECTED_OS`: computed (`Windows`, `Darwin`, or Linux uname).

## Top‑level targets
- `help`
  - Prints a summary and the detected OS.

- `install`
  - Verifies a minimal C++ toolchain is available.
  - macOS: checks for `clang++` (install Xcode Command Line Tools if missing).
  - Linux: checks for `g++` or `clang++`.
  - Windows: prints checks for `cl`, `g++`, `clang++`, etc.

- `app NAME=<name>`
  - Scaffolds a new app at `app/<name>/`:
    - `src/main.cpp` with a minimal hello world.
    - A per‑app `Makefile` configured to output into `build/<name>/`.

- `list`
  - Lists subdirectories under `app/` (your available apps).

- `build [APP=<name>]`
  - Builds the given app.
  - If `APP` is omitted, an interactive picker lists apps and stores your selection in `build/.selected_app`, then builds that app.

- `run [APP=<name>]`
  - Builds then runs the app using the per‑app `Makefile` (same output binary in `build/<name>/bin/<name>`).
  - If `APP` is omitted, uses the same interactive picker as `build`.

- `run-from-build APP=<name>`
  - Runs `build/<name>/bin/<name>` directly from the top‑level build directory.

### Internal helper targets
- `pick-app`
  - Interactive selection. On macOS/Linux it prints a numbered list and reads the choice; on Windows it uses PowerShell. Writes the chosen name to `build/.selected_app`.

- `selected-app`
  - Prints the content of `build/.selected_app`.

## Per‑app Makefile (template)
Each app gets a generated `Makefile` like this (simplified):

- Variables
  - `APP_NAME`: the app name.
  - `SRC_DIR`: usually `src`.
  - `BUILD_ROOT`: top‑level `build` directory (computed from app dir).
  - `BUILD_DIR`: `$(BUILD_ROOT)/$(APP_NAME)`.
  - `BIN_DIR`: `$(BUILD_DIR)/bin`.

- Targets
  - `all` (default) → `build`.
  - `build` → compiles `src/*.cpp` to `$(BUILD_DIR)/*.o`, links to `$(BIN_DIR)/$(APP_NAME)`.
  - `run` → runs the linked binary. On Windows it appends `.exe`, on others runs the POSIX binary.
  - `clean` → removes `$(BUILD_DIR)` for that app.

This keeps the app folder clean and puts all intermediates and artifacts in the centralized `build/` tree.

## Examples
- Create then run a new app:
```sh
make app NAME=hello
make run APP=hello
```

- Build using g++ explicitly:
```sh
make build APP=hello CXX=g++
```

- Interactive build/run (omit APP):
```sh
make build
make run
```

- Run from centralized build folder:
```sh
make run-from-build APP=hello
```

## Troubleshooting
- macOS: “clang++ not found” → install Command Line Tools: `xcode-select --install`.
- Compiler errors about missing `build` dirs should be handled by `mkdir -p` rules; if you see them, run the `build` target again and share the exact error.
- If a new app doesn’t appear in `make list`, confirm it exists under `app/<name>/` and is a directory.

## Notes
- No vendors/submodules are used; this is intentionally minimal.
- You can tweak warning levels or standard version in the per‑app `Makefile` (`CXXFLAGS`).
