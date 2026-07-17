# Using the Makefile — a step-by-step guide

This workspace builds C and C++ apps with plain `make`. The design is simple and
predictable:

- **One place owns the flags** — the root `Makefile`. It sets the compiler and
  warning/optimisation/sanitizer flags and `export`s them.
- **Each app has a one-line Makefile** — a single `all:` rule that compiles
  `src/` using those exported flags. Nothing to configure per app.
- **The app's NAME is part of the command** — `make run-cpp-app-calc`. There's
  no `app=…` variable to remember, and tab-completion works.

> **First time?** Run `make install` once. On macOS it just checks you have the
> Xcode Command Line Tools; on Linux/Windows it downloads a local Clang into
> `./toolchain/`.

---

## 1. Create your first app

```sh
make new-cpp-app-hello-me
```

That copies the C++ template into a new folder:

```
apps/cpp/hello-me/
├── Makefile      # one line — see §6
├── README.md
└── src/
    └── main.cpp  # prints "hello"
```

(For C, use `make new-c-app-hello-me` → `apps/c/hello-me/` with `main.c`.)

## 2. Build and run it

```sh
make run-cpp-app-hello-me
```

`run-…` builds first, then runs. To only build:

```sh
make build-cpp-app-hello-me
```

The program lands in `build/apps/cpp/hello-me/hello-me` (the whole `build/`
folder is git-ignored). Pass arguments with `ARGS`:

```sh
make run-cpp-app-hello-me ARGS='one two three'
```

## 3. C or C++ — same commands, different word

The `cpp` / `c` in the target picks the language and the tree:

```sh
make run-cpp-app-calc      # apps/cpp/calc   (C++)
make run-c-app-shell       # apps/c/shell    (C)
```

We spell out the language because the same lesson name can exist in **both**
trees (there's an `01-hello-world` in C *and* C++), so `run-cpp-app-…` /
`run-c-app-…` is unambiguous.

## 4. Switch the C++ standard or build type

Any of these variables pass straight through to the compile:

```sh
make run-cpp-app-hello-me STD=c++98          # older C++ standard
make run-c-app-shell      CSTD=c99           # older C standard
make run-cpp-app-mandelbrot BUILD_TYPE=Release   # -O2, asserts off, no sanitizers
```

By default apps build **Debug**: `-O0 -g` plus AddressSanitizer and
UndefinedBehaviorSanitizer, so memory bugs and undefined behaviour surface while
you learn. Use `BUILD_TYPE=Release` when you want speed (e.g. the
`tinyraytracer` render loop).

## 5. Everyday commands

```sh
make list                     # all apps (list-cpp / list-c for one language)
make build                    # build every app
make build-cpp-app-NAME       # build one
make clean                    # remove all build output
make delete-cpp-app-NAME      # delete an app (source + build)
make help                     # this target list, from the Makefile itself
```

## 6. What an app's Makefile looks like

Open `apps/cpp/calc/Makefile` — this is the whole file:

```make
all:
	$(CXX) $(CXXFLAGS) -Iinclude -Isrc src/*.cpp -o $(BUILD_DIR)/$(OUTPUT_NAME)
```

`$(CXX)` and `$(CXXFLAGS)` are **exported by the root Makefile**; `$(BUILD_DIR)`
and `$(OUTPUT_NAME)` are **passed in** when the root runs `make -C` on the app.
So the app never repeats the flags — it just says "compile my sources." Change
how everything builds by editing the flags in **one** place: the root Makefile.

**Need something special** for one app (an extra library, a code generator,
several build steps)? Replace that one-line `all:` rule with whatever commands
you need — it still receives the exported flags and `BUILD_DIR`/`OUTPUT_NAME`,
and `make build-cpp-app-NAME` keeps working.

## 7. How it fits together

| Piece | Job |
|-------|-----|
| **`Makefile`** (root) | Owns & exports the flags. Turns `run-cpp-app-NAME` into "compile `apps/cpp/NAME`, then run `build/apps/cpp/NAME/NAME`." |
| **`apps/<lang>/<name>/Makefile`** | One `all:` rule that compiles `src/` with the exported flags. |
| **`templates/<lang>/`** | What `make new-<lang>-app-NAME` copies to start a new app. |

That's the whole system. If you can read the root Makefile's flags and an app's
one-line `all:` rule, you understand the build.
