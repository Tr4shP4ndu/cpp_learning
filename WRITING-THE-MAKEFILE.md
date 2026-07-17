# Writing the build, step by step

[MAKEFILE.md](MAKEFILE.md) shows how to *use* this workspace's build. This guide
shows how to **write** it — we rebuild the exact Makefile system this repo uses,
one small step at a time, learning each `make` idea as we reach for it.

By the end you'll understand every line of the root `Makefile`, the per-app
`Makefile`, and how they talk to each other.

> **The one rule that trips everyone up:** recipe lines are indented with a
> **TAB**, never spaces. `make` will say *"missing separator"* if you use
> spaces. Every indented command below is a real tab.

---

## Step 0 — the command make is hiding

Building a C++ program is really just one command:

```sh
c++ -std=c++23 -Wall -Wextra src/main.cpp -o hello && ./hello
```

`make` doesn't compile anything itself — it just *runs commands like this for
you*, and skips work that's already up to date. Everything below is about
teaching `make` to run that line.

## Step 1 — your first rule

A Makefile is a list of **rules**. A rule has three parts:

```make
target: prerequisites
	recipe        # <- indented with a TAB
```

- **target** — usually a file to produce (or just a name for a task).
- **prerequisites** — files the target depends on.
- **recipe** — the shell commands that build the target.

The simplest app Makefile:

```make
hello: src/main.cpp
	c++ -std=c++23 -Wall -Wextra src/main.cpp -o hello
```

Run `make hello` (or just `make` — the first target is the default). Run it
again and `make` says *"'hello' is up to date"*: because `hello` is newer than
its prerequisite `src/main.cpp`, there's nothing to do. Touch the source and it
rebuilds. **That freshness check is the whole point of make.**

## Step 2 — variables, so settings live in one spot

Hard-coding `c++` and the flags everywhere is brittle. Pull them into
**variables**:

```make
CXX      ?= c++
CXXFLAGS ?= -std=c++23 -Wall -Wextra

hello: src/main.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp -o hello
```

- `$(CXX)` expands the variable.
- `?=` means *"assign only if not already set"*, so a caller can override:
  `make CXX=g++` or `make CXXFLAGS=-std=c++98`. Variables set on the command
  line win over `?=`.

Already this is close to what each app in this repo contains.

## Step 3 — one owner for the flags: a root Makefile that `export`s

We have ~80 apps. We do **not** want the flags copied into 80 Makefiles. So one
**root** Makefile defines them once and hands them down. `make` passes a
variable to sub-processes only if you `export` it:

```make
# root Makefile
CXX      ?= c++
CXXFLAGS ?= -std=c++23 -Wall -Wextra
export CXX CXXFLAGS
```

Now an app's Makefile can *use* `$(CXX)`/`$(CXXFLAGS)` without defining them —
it inherits them from the root. The app Makefile shrinks to just the recipe:

```make
# apps/cpp/hello/Makefile
all:
	$(CXX) $(CXXFLAGS) -Iinclude -Isrc src/*.cpp -o $(BUILD_DIR)/$(OUTPUT_NAME)
```

Two new variables appear — `BUILD_DIR` and `OUTPUT_NAME`. The app doesn't set
them; the root will pass them in. That's the next step.

## Step 4 — the root drives the app: recursive make

`make` can invoke another `make` in a different directory with `-C`. Use the
built-in `$(MAKE)` variable so flags/jobs propagate correctly:

```make
# root Makefile — build the 'hello' C++ app
build-hello:
	@mkdir -p build/apps/cpp/hello
	$(MAKE) -C apps/cpp/hello \
		BUILD_DIR=$(abspath build/apps/cpp/hello) OUTPUT_NAME=hello
```

- `$(MAKE) -C <dir>` runs the Makefile in `<dir>`.
- The `VAR=value` arguments are passed **into** that sub-make — which is how the
  app's `all:` rule receives `BUILD_DIR` and `OUTPUT_NAME`.
- `$(abspath …)` turns a relative path absolute, so it's still correct after we
  `cd` into the app's folder.
- A leading `@` on a recipe line hides the command itself from the output (you
  see its result, not the line).

## Step 5 — put the NAME in the target: pattern rules

We don't want a `build-hello`, `build-calc`, `build-mandelbrot`… rule each. A
**pattern rule** uses `%` as a wildcard, and `$*` is whatever `%` matched (the
"stem"):

```make
build-cpp-app-%:
	@mkdir -p build/apps/cpp/$*
	$(MAKE) -C apps/cpp/$* \
		BUILD_DIR=$(abspath build/apps/cpp/$*) OUTPUT_NAME=$*
```

Now `make build-cpp-app-calc` sets `$* = calc` and builds `apps/cpp/calc`. One
rule builds every C++ app. **This is why the app name is part of the command** —
it's the stem of a pattern rule.

## Step 6 — don't repeat it for C: a reusable recipe

C needs the same rule with `c`/`CC`/`CFLAGS` instead of `cpp`/`CXX`/`CXXFLAGS`.
Rather than copy it, capture the recipe once with `define` and call it with an
argument:

```make
define build_app                 # $(1) is the language: cpp or c
	@mkdir -p build/apps/$(1)/$*
	$(MAKE) -C apps/$(1)/$* \
		BUILD_DIR=$(abspath build/apps/$(1)/$*) OUTPUT_NAME=$*
endef

build-cpp-app-%: ; $(call build_app,cpp)
build-c-app-%:   ; $(call build_app,c)
```

- `define … endef` stores a multi-line recipe.
- `$(call build_app,cpp)` expands it with `$(1)` = `cpp`.
- `$*` is still filled in per invocation, inside the expanded recipe.
- `target: ; recipe` is just a one-line way to write a rule.

(The app's own Makefile already picks the compiler per language: it uses `$(CC)
$(CFLAGS)` and `src/*.c` in the C template, `$(CXX) $(CXXFLAGS)` and `src/*.cpp`
in the C++ one.)

## Step 7 — `run` = build, then execute

We want `make run-cpp-app-calc` to build *and* run. Make one target depend on
another so the build happens first:

```make
define run_app                   # $(1) = lang
	@"./build/apps/$(1)/$*/$*" $(ARGS)
endef

run-cpp-app-%: build-cpp-app-% ; $(call run_app,cpp)
run-c-app-%:   build-c-app-%   ; $(call run_app,c)
```

`run-cpp-app-calc` lists `build-cpp-app-calc` as a prerequisite, so `make` builds
it first, then runs the recipe. `$(ARGS)` lets you pass arguments:
`make run-cpp-app-calc ARGS='1 + 2'`.

## Step 8 — the conveniences: build-all, list, clean, new, delete

**Find the apps** instead of hand-listing them. `$(shell …)` runs a shell
command; `$(patsubst)`/`$(notdir)` reshape the results:

```make
CPP_APPS := $(sort $(notdir $(patsubst %/src,%,$(shell find apps/cpp -type d -name src))))
```

That reads "every folder under `apps/cpp` that has a `src/`, keep just the app
name." Then **build everything** by depending on one build target per app —
`$(addprefix)` glues the prefix onto each name:

```make
build: $(addprefix build-cpp-app-,$(CPP_APPS)) $(addprefix build-c-app-,$(C_APPS))
	@echo "Built all apps."
```

**List, clean, scaffold, delete** are ordinary rules:

```make
list-cpp:
	@printf '    %s\n' $(CPP_APPS)

clean:
	@rm -rf build

new-cpp-app-%:
	@cp -R templates/cpp/. apps/cpp/$*/    # copy the starter template

delete-cpp-app-%:
	@rm -rf apps/cpp/$* build/apps/cpp/$*

.PHONY: build list-cpp clean            # names, not files — always run
```

`.PHONY` tells `make` these targets are task names, not files to check — so they
run even if a file named `clean` happens to exist.

## Step 9 — one refinement: prefer a repo-local compiler

The real root Makefile adds a small touch: if `make install` put a compiler in
`./toolchain/`, use it — unless the user named one explicitly.

```make
ifneq ($(wildcard toolchain/bin/clang++),)   # does the file exist?
  ifeq ($(origin CXX),default)               # user didn't set CXX?
    CXX := $(abspath toolchain/bin/clang++)
  endif
endif
```

- `$(wildcard path)` expands to the path if it exists, else nothing — a file
  test.
- `$(origin CXX)` reports *how* `CXX` got its value; `default` means "make's
  built-in default, nobody set it," so we're safe to override.

## You just wrote this repo's build

Put those pieces together and you have exactly what's in the repo:

- **`Makefile`** (root) — Steps 3–9: owns and `export`s the flags, and the
  `new/build/run/delete` pattern rules + `build`/`list`/`clean`.
- **`apps/<lang>/<name>/Makefile`** — Steps 1–2 shrunk by Step 3: one `all:`
  rule using the inherited flags.
- **`templates/{cpp,c}/`** — what `new-<lang>-app-%` copies (Step 8).

Open the root `Makefile` now and read it top to bottom — every construct in it
(`?=`, `:=`, `export`, `%`, `$*`, `define`/`call`, `$(MAKE) -C`, `$(shell)`,
`$(wildcard)`, `.PHONY`) is one you just built up here.

## Cheat-sheet

| Construct | Means |
|-----------|-------|
| `target: prereqs` / TAB recipe | a rule; run the recipe if a prereq is newer |
| `VAR ?= x` | set only if unset (overridable) |
| `VAR := x` | set now, plainly |
| `$(VAR)` | expand a variable |
| `export VAR` | pass VAR to sub-makes |
| `$(MAKE) -C dir V=x` | run make in `dir`, passing `V` |
| `foo-%:` … `$*` | pattern rule; `$*` is what `%` matched |
| `define f` … `endef` + `$(call f,arg)` | a reusable recipe; `$(1)` is the arg |
| `$(shell cmd)` | output of a shell command |
| `$(wildcard p)` | `p` if it exists, else empty |
| `$(patsubst a,b,text)` / `$(notdir p)` / `$(addprefix p,list)` | text surgery |
| `.PHONY: name` | `name` is a task, not a file |
| `@cmd` | run `cmd` without echoing the line |
