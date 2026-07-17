# Using the Makefile — a step-by-step guide

This workspace builds C and C++ apps with plain `make`. There is **no magic you
have to memorise**: every app has its own tiny `Makefile`, and one shared file
(`rules.mk`) holds the actual compiler command. This guide walks you through it
by *doing* — the same steps used to set the workspace up.

> **First time?** Set up a compiler once: `make install`. On macOS that just
> checks you have the Xcode Command Line Tools; on Linux/Windows it downloads a
> local Clang into `./toolchain/`.

---

## 1. Create your first app

From the repo root:

```sh
make app app=hello-me
```

That scaffolds a folder under the C++ tree:

```
apps/cpp/hello-me/
├── Makefile        # two lines — see below
├── README.md       # notes template
└── src/
    └── main.cpp    # a "hello" starter
```

Open `apps/cpp/hello-me/Makefile` — it is only this:

```make
APP := hello-me
include $(firstword $(wildcard ../../rules.mk ../../../rules.mk ...))
```

It names the app and pulls in the shared build recipe. That's the whole thing.

## 2. Build and run it

Two equivalent ways. **From the repo root, by name:**

```sh
make run app=hello-me
```

**Or go into the folder** and use its own Makefile directly:

```sh
cd apps/cpp/hello-me
make run          # build + run
make              # build only
make clean        # delete this app's build/
```

The compiled program lands in the app's own `build/` folder
(`apps/cpp/hello-me/build/hello-me`), which is git-ignored.

Pass arguments to your program with `ARGS`:

```sh
make run app=hello-me ARGS='one two three'
```

## 3. Put related apps in a group

Apps don't have to sit directly under `apps/cpp/`. Give `make app` a `group=`
and it nests the app in a named folder:

```sh
make app app=spinner lang=cpp group=graphics
#            -> apps/cpp/graphics/spinner
```

Nothing else changes — apps are found by **name**, at any depth:

```sh
make run app=spinner        # still just works
```

Groups are pure organisation; use them to keep, say, all your graphics demos
together.

## 4. Switch the C++ standard or build type

Any of these variables pass straight through to the app:

```sh
make run app=hello-me STD=c++98            # compile as older C++
make run app=hello-me BUILD_TYPE=Release   # -O2, asserts off, no sanitizers
```

By default apps build **Debug**: `-O0 -g` plus AddressSanitizer and
UndefinedBehaviorSanitizer, so memory bugs and undefined behaviour are caught
while you learn. Use `BUILD_TYPE=Release` when you want speed (e.g. the
`tinyraytracer` render loop).

## 5. Write a C app

Add `lang=c` and you get a `main.c` instead:

```sh
make app app=ctool lang=c        # -> apps/c/ctool/src/main.c
make run app=ctool               # C vs C++ is auto-detected from the sources
```

(`make run-c app=ctool` also works — it's the same thing, kept so the C
lessons' READMEs read naturally.)

## 6. When you outgrow the defaults

The shared rules compile everything in `src/` (plus an optional `include/`) with
one command. If an app needs something special — extra libraries, a code
generator, multiple build steps — **replace its two-line Makefile with a real
one of your own.** As long as it has `build` and `run` targets, the launcher
still finds it by name and calls it:

```sh
make run app=my-fancy-app        # runs YOUR Makefile's `run` target
```

So you start simple and only take over the build when you actually need to.

## 7. Everyday commands

```sh
make list            # every C++ app
make list-c          # every C app
make build app=NAME  # build one app
make build           # build ALL apps
make clean           # remove every app's build/ output
make delete-app app=NAME   # delete an app folder
make help            # this list, from the Makefile itself
```

## 8. How it works (the design)

Three small pieces, each with one job:

| File | Job |
|------|-----|
| **`apps/<lang>/<name>/Makefile`** | Names the app (`APP := …`) and includes the shared rules. Two lines. |
| **`rules.mk`** (repo root) | The one real build recipe: picks the compiler (C++ if there are `.cpp` files, else C), prefers `./toolchain`, sets flags, compiles `src/` → `build/<name>`. |
| **`Makefile`** (repo root) | A convenience launcher. `make run app=X` **finds** the folder named `X` and runs *its* Makefile for you. Also does `install`, `list`, `app`, `clean`. |

A few details worth knowing:

- **Finding `rules.mk`.** Each app's `include` line searches a few directory
  levels up (`../../rules.mk`, `../../../rules.mk`, …) and uses the first one
  that exists. That's why an app works whether it sits directly under a language
  or inside a group folder — the path to the root is found automatically.
- **C vs C++.** `rules.mk` looks for `src/*.cpp`; if there are none it treats
  the app as C and uses `src/*.c`. You never declare the language.
- **The root launcher owns no build logic.** It only locates an app by name and
  runs `make -C <that folder>`. All the compiling lives in `rules.mk`, in one
  place, so there's a single spot to read or change how things build.

That's the entire system. If you can read the two-line app Makefile and
`rules.mk`, you understand the whole build.
