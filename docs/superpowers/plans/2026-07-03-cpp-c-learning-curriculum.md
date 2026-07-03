# C/C++ Learning Curriculum Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Grow this C++-only workspace into a staged C/C++ learning path (52 lessons + a from-scratch software-renderer capstone) that a total beginner can follow to advanced modern C++.

**Architecture:** Each lesson is a standalone folder built by one `make` command. A revised Makefile pair (top-level + per-app) adds C++/C standard switching, C counterparts, sanitizers, include-path + header-dependency support. Lessons 01–29 stay single-file; lesson 30 onward use `include/`+`src/`. The capstone is a real multi-file project. No external deps, no submodules, no CMake.

**Tech Stack:** C++23/C17 (switchable back to c++98), GNU Make, clang++/g++, clang/gcc. macOS + Linux.

## Global Constraints

- Vendor-free: no external dependencies, no git submodules, no CMake. Copied verbatim from spec.
- Compilers: `clang++`/`g++` (C++), `clang`/`gcc` (C). Default `CXX ?= g++`, `CC ?= gcc` (override on command line; macOS resolves these to clang).
- Standards: `STD=c++98|c++11|c++14|c++17|c++20|c++23` default `c++23`; `CSTD=c99|c11|c17` default `c17`.
- Warnings always on: `-Wall -Wextra -Wpedantic`.
- Debug (default) adds `-O0 -g -fsanitize=address,undefined`; Release uses `-O2`, no sanitizers.
- No `using namespace std;` in headers. `#pragma once` in headers (note the classic include-guard form in lesson 30).
- C counterpart binary suffix: `-c`. Dual binaries: `build/<app>/bin/<app>` and `build/<app>/bin/<app>-c`.
- Multi-file layout from lesson 30: `include/` for headers, `src/` for sources incl. `main`.
- `00-testing` is left untouched.
- Lesson README shape: Concept → Minimal example → Line-by-line → Common pitfalls → C vs C++ (where relevant) → Build & run → Try it yourself.
- Renumber map and full curriculum are in `docs/superpowers/specs/2026-07-03-cpp-c-learning-curriculum-design.md` — the source of truth for lesson numbers/names.

## Verification model (there is no unit-test framework here)

"Tests pass" for a lesson means all of:
1. `make build app=<app>` succeeds with **zero** warnings under default `-Wall -Wextra -Wpedantic`.
2. `make run app=<app>` (and `run-c` when a `.c` exists) runs and exits 0 with no ASan/UBSan report.
3. Any in-`main` `assert()` sanity checks pass.

The per-tier "definition of done" is: `make build` (all apps) is clean, and each new/changed app runs clean, then commit.

---

## Phase 1 — Foundation (detailed)

Goal: new Makefiles, templates, root README learning-path, `references/` stubs, and the full folder renumber — with everything still building. **No lesson content is rewritten in this phase** (that's Phase 2+).

### Task 1: New per-app Makefile template

**Files:**
- Modify: `templates/app/Makefile.tpl`

**Interfaces:**
- Produces: a per-app Makefile that supports `STD`, `CSTD`, `BUILD_TYPE`, dual C/C++ binaries, `-Iinclude -Isrc`, `-MMD -MP` dep tracking, targets `build run run-c clean`.

- [ ] **Step 1: Write the new per-app Makefile template**

```make
# Per-app Makefile — one lesson, one command.
APP_NAME := __APP_NAME__
SRC_DIR  := src
INC_DIR  := include

BUILD_ROOT ?= $(abspath $(CURDIR)/../../build)
BUILD_DIR  := $(BUILD_ROOT)/$(APP_NAME)
BIN_DIR    := $(BUILD_DIR)/bin

# Toolchain (override on the command line, e.g. CXX=clang++)
CXX ?= g++
CC  ?= gcc

# Standards — switch to see old vs modern behavior:
#   make run STD=c++98      make run-c CSTD=c99
STD  ?= c++23
CSTD ?= c17

BUILD_TYPE ?= Debug
WARN       := -Wall -Wextra -Wpedantic
INC        := $(addprefix -I,$(wildcard $(INC_DIR))) -I$(SRC_DIR)
DEPFLAGS   := -MMD -MP

ifeq ($(BUILD_TYPE),Release)
  OPT := -O2
  SAN :=
else
  OPT := -O0 -g
  SAN := -fsanitize=address,undefined
endif

CXXFLAGS ?= -std=$(STD)  $(WARN) $(OPT) $(SAN) $(INC) $(DEPFLAGS)
CFLAGS   ?= -std=$(CSTD) $(WARN) $(OPT) $(SAN) $(INC) $(DEPFLAGS)
LDFLAGS  ?= $(SAN)

# C++ sources -> the main binary; C sources -> the "-c" binary.
CXX_SRCS := $(wildcard $(SRC_DIR)/*.cpp)
C_SRCS   := $(wildcard $(SRC_DIR)/*.c)
CXX_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CXX_SRCS))
C_OBJS   := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.c.o,$(C_SRCS))

BIN   := $(BIN_DIR)/$(APP_NAME)
BIN_C := $(BIN_DIR)/$(APP_NAME)-c

.PHONY: all build run run-c clean
all: build

$(BIN_DIR):
	@mkdir -p "$(BIN_DIR)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

build: $(if $(CXX_SRCS),$(BIN)) $(if $(C_SRCS),$(BIN_C))

$(BIN): $(CXX_OBJS)
	$(CXX) $(CXX_OBJS) -o "$@" $(LDFLAGS)

$(BIN_C): $(C_OBJS)
	$(CC) $(C_OBJS) -o "$@" $(LDFLAGS)

run: build
	@"$(BIN)"

run-c: build
	@if [ -x "$(BIN_C)" ]; then "$(BIN_C)"; else echo "No C version for $(APP_NAME)"; fi

clean:
	@rm -rf "$(BUILD_DIR)"

-include $(CXX_OBJS:.o=.d) $(C_OBJS:.o=.d)
```

- [ ] **Step 2: Verify it builds a sample app**

Run: copy this template into an existing lesson temporarily and `make build`.
Expected: builds clean, `.d` files appear alongside `.o`.

- [ ] **Step 3: Commit**

```bash
git add templates/app/Makefile.tpl
git commit -m "feat(make): per-app Makefile with STD/C/sanitizer/-I/dep support"
```

### Task 2: Update the other templates

**Files:**
- Modify: `templates/app/main.cpp.tpl`, `templates/app/README.md.tpl`
- Create: `templates/app/main.c.tpl`

**Interfaces:**
- Consumes: nothing.
- Produces: scaffold sources matching the standard lesson shape.

- [ ] **Step 1: main.cpp.tpl**

```cpp
// __APP_NAME__ — one-line concept goes here.
#include <iostream>

int main() {
    std::cout << "__APP_NAME__\n";
    return 0;
}
```

- [ ] **Step 2: main.c.tpl**

```c
/* __APP_NAME__ (C) — one-line concept goes here. */
#include <stdio.h>

int main(void) {
    printf("__APP_NAME__\n");
    return 0;
}
```

- [ ] **Step 3: README.md.tpl** — the 7-section shape

```markdown
# __APP_NAME__

## Concept
<!-- Plain-English: what this is and why it exists. -->

## Minimal example
See `src/main.cpp`.

## Line-by-line
<!-- Annotate the non-obvious lines. -->

## Common pitfalls
<!-- The mistakes beginners actually hit. -->

## C vs C++
<!-- Only if a C counterpart exists; otherwise delete this section. -->

## Build & run
```sh
make run app=__APP_NAME__            # C++ (default c++23)
make run app=__APP_NAME__ STD=c++98  # try an older standard
make run-c app=__APP_NAME__          # C version, if present
```

## Try it yourself
<!-- One small exercise. -->
```

- [ ] **Step 4: Commit**

```bash
git add templates/app
git commit -m "feat(templates): standard lesson shape + C scaffold"
```

### Task 3: Rewrite the top-level Makefile

**Files:**
- Modify: `Makefile`

**Interfaces:**
- Produces: `list build run run-c app clean delete-app help`, forwarding `STD CSTD CXX CC BUILD_TYPE` to per-app Makefiles; `app=` scaffolds from templates with optional `lang=c`.

- [ ] **Step 1: Write the top-level Makefile**

```make
# Top-level workspace Make — scaffold, build, run lessons.
APP_DIR      := apps
BUILD_DIR    := build
TEMPLATE_DIR := templates/app

CXX ?= g++
CC  ?= gcc
STD ?= c++23
CSTD ?= c17
BUILD_TYPE ?= Debug
export CXX CC STD CSTD BUILD_TYPE BUILD_DIR

APPS := $(notdir $(wildcard $(APP_DIR)/*))
TEMPLATES := $(wildcard $(TEMPLATE_DIR)/*.tpl)

define require_app
$(if $(app),,$(error Usage: make $(1) app=<name>))
endef

.PHONY: help list app build run run-c clean delete-app

help:
	@echo "Targets:"
	@echo "  make app app=<name> [lang=c]   Scaffold a new lesson"
	@echo "  make list                      List lessons"
	@echo "  make build [app=<name>]        Build one lesson (or all)"
	@echo "  make run app=<name>            Run the C++ binary"
	@echo "  make run-c app=<name>          Run the C binary (if present)"
	@echo "  make clean                     Remove build outputs"
	@echo "  make delete-app app=<name>     Delete a lesson"
	@echo ""
	@echo "Variables: STD=c++98..c++23 (def c++23)  CSTD=c99|c11|c17"
	@echo "           BUILD_TYPE=Debug|Release  CXX=g++|clang++  CC=gcc|clang"

list:
	@printf "%s\n" $(APPS)

app:
	$(call require_app,$@)
	@mkdir -p "$(APP_DIR)/$(app)/src"
	@for tpl in $(TEMPLATES); do \
		out=$$(basename "$$tpl" .tpl); \
		case "$$out" in \
			main.cpp) [ "$(lang)" = "c" ] && continue; dest="$(APP_DIR)/$(app)/src/main.cpp" ;; \
			main.c)   [ "$(lang)" = "c" ] || continue; dest="$(APP_DIR)/$(app)/src/main.c" ;; \
			*)        dest="$(APP_DIR)/$(app)/$$out" ;; \
		esac; \
		sed 's/__APP_NAME__/$(app)/g' "$$tpl" > "$$dest"; \
	done
	@echo "Created app: $(APP_DIR)/$(app)"

build:
ifdef app
	@$(MAKE) -C "$(APP_DIR)/$(app)" build
else
	@for a in $(APPS); do $(MAKE) -C "$(APP_DIR)/$$a" build || exit 1; done
endif

run:
	$(call require_app,$@)
	@$(MAKE) -C "$(APP_DIR)/$(app)" run

run-c:
	$(call require_app,$@)
	@$(MAKE) -C "$(APP_DIR)/$(app)" run-c

clean:
	@echo "Cleaning $(BUILD_DIR)/*"
	@rm -rf "$(BUILD_DIR)"/* 2>/dev/null || true

delete-app:
	$(call require_app,$@)
	@rm -rf "$(APP_DIR)/$(app)" "$(BUILD_DIR)/$(app)" 2>/dev/null || true
	@echo "Deleted: $(app)"
```

- [ ] **Step 2: Verify**

Run: `make list` then `make app app=zz-probe && make run app=zz-probe && make run-c app=zz-probe && make delete-app app=zz-probe`
Expected: scaffolds, builds clean, C++ runs, `run-c` prints "No C version", deletes.

- [ ] **Step 3: Commit**

```bash
git add Makefile
git commit -m "feat(make): top-level STD/C switching, run-c, lang=c scaffold"
```

### Task 4: Renumber existing lesson folders

**Files:** rename per the renumber map in the spec (use `git mv`). Two folders also lose the `::` (`09-std::arrays`→`19-std-array`, `14-std::fill`→`20-std-fill`).

- [ ] **Step 1: Rename in dependency-safe order**

Because numbers overlap between old and new, rename to temp names first, then to finals. Script:

```bash
cd apps
# stage every rename to a temp prefix to avoid collisions
git mv 05-scope _t-09-scope
git mv 06-namespaces _t-10-namespaces
git mv 07-typedef-aliases _t-11-typedef-aliases
git mv 08-arrays _t-18-arrays
git mv 09-std::arrays _t-19-std-array
git mv 10-for-loop _t-12-for-loop
git mv 11-range-for-loop _t-13-range-for-loop
git mv 12-while-loop _t-14-while-loop
git mv 13-do-while-loop _t-15-do-while-loop
git mv 14-std::fill _t-20-std-fill
git mv 15-break-statement _t-16-break-statement
git mv 16-continue-statement _t-17-continue-statement
git mv 17-functions _t-25-functions
git mv 18-void-functions _t-26-void-functions
git mv 19-functions-with-parameters _t-27-functions-with-parameters
git mv 20-function-overloading _t-28-function-overloading
git mv 21-recursive-function _t-29-recursive-function
git mv 22-operators _t-06-operators
git mv 23-value-semantics _t-32-value-semantics
git mv 24-references _t-33-references
git mv 25-dereferencing _t-35-dereferencing
# 01-04 keep their numbers; leave them.
# now strip the temp prefix
for d in _t-*; do git mv "$d" "${d#_t-}"; done
cd ..
```

- [ ] **Step 2: Fix APP_NAME inside each renamed per-app Makefile**

Each renamed folder's `Makefile` still has the old `APP_NAME`. Replace with the new folder name (and swap in the new template while we're here, done in Phase 2 per-lesson; for now just correct APP_NAME so builds work):

```bash
cd apps
for d in */; do d=${d%/}; sed -i.bak "s/^APP_NAME := .*/APP_NAME := $d/" "$d/Makefile" && rm -f "$d/Makefile.bak"; done
cd ..
```

- [ ] **Step 3: Verify everything still builds**

Run: `make clean && make build`
Expected: all lessons compile (warnings from old code are acceptable *for now*; hard errors are not).

- [ ] **Step 4: Commit**

```bash
git add -A apps
git commit -m "refactor(apps): renumber lessons into single sequence; drop '::' in names"
```

### Task 5: Root README learning-path + references stubs

**Files:**
- Modify: `README.md`, `Guide-to-Install-C-CPP.md`
- Create: `references/debugging-and-tooling.md`, `references/going-further-gpu.md`

- [ ] **Step 1:** Add a "Learning path" section to `README.md` — the curriculum table from the spec (numbers + names, grouped by tier), a "how to use this repo" paragraph, and `STD=`/`run-c` usage. Keep the existing quick-start.
- [ ] **Step 2:** Add a one-line C-compiler check to `Guide-to-Install-C-CPP.md` (`cc --version` / `gcc --version`).
- [ ] **Step 3:** Create the two `references/` files as short stubs with headings and a one-paragraph intro each (filled in Phase 9).
- [ ] **Step 4: Commit**

```bash
git add README.md Guide-to-Install-C-CPP.md references
git commit -m "docs: learning-path README, C-compiler note, references stubs"
```

**Phase 1 done when:** `make clean && make build` is clean of hard errors, scaffold round-trips, renumber committed.

---

## Per-lesson recipe (Phases 2–8)

Apply this to every lesson in a tier. A "lesson" task = one folder.

- [ ] **1. Swap in the current template Makefile**, set `APP_NAME`.
- [ ] **2. Write `src/main.cpp`** (and `src/main.c` if the spec marks it `C`) demonstrating exactly the lesson's concept — small, warning-clean, with an `assert()` sanity check if there's real logic.
- [ ] **3. Write `README.md`** in the 7-section shape. The "C vs C++" section only if a `.c` exists.
- [ ] **4. `make run app=<app>`** (and `run-c`) — verify clean build, zero warnings, exit 0, no sanitizer report.
- [ ] **5. For a modern feature, also `make build app=<app> STD=c++98`** and note in the README whether it compiles (teaches the standard difference).
- [ ] **6. Commit** `feat(lesson): NN-<name>`.

Tier task = do the recipe for every lesson in the tier, then one `make build` (all) + tier commit. Existing lessons are *deepened/standardized*; new lessons are *authored*.

## Phase 2 — Beginner (01–11)
Deepen: 01,02,03,04,06,09,10,11. Author: 05 io-basics (+C), 07 conditionals, 08 switch. C parts: 01,03,05.
Note per lesson: 01 add `main.c` (printf); 03 add `main.c` (C types, `<stdint.h>`); 05 cin/cout vs printf/scanf; 07 if/else/ternary; 08 switch/fallthrough/`[[fallthrough]]`.

## Phase 3 — Core (12–24)
Deepen: 12–20. Author: 21 strings (+C: `char[]`+`<string.h>` vs `std::string`), 22 vectors, 23 containers-map-set (`std::map`,`unordered_map`,`set`), 24 files-and-filesystem (+C: `FILE*`; C++: `fstream`+`std::filesystem::path`).

## Phase 4 — Functions (25–29)
Deepen all five (25–29). Add const-ref parameter note to 27.

## Phase 5 — Project structure & pipeline (30–31)
Author 30 header-source-separation: first multi-file lesson — `include/greeter.hpp`+`src/greeter.cpp`+`src/main.cpp`, `#pragma once` + classic guard note, ODR one-liner. Author 31 build-pipeline: a README-heavy lesson showing `-E`/`-S`/`-c`, object files, linking, `-I`; a small 2-file program whose README walks each stage's command and output.

## Phase 6 — Memory (32–37)
Deepen: 32,33,35. Author: 34 pointers (+C), 36 dynamic-memory (+C: `malloc/free` vs `new/delete`, and "why we prefer RAII/smart pointers"), 37 smart-pointers (`unique_ptr`/`shared_ptr`/`make_*`).

## Phase 7 — OOP (38–45, multi-file)
Author all, `include/`+`src/` layout: 38 structs (+C), 39 enums (`enum class`), 40 classes-basics, 41 constructors-destructors (RAII), 42 encapsulation, 43 inheritance, 44 polymorphism (`virtual`/override/abstract), 45 operator-overloading.

## Phase 8 — Advanced (46–51)
Author: 46 templates, 47 concepts (C++20; README notes `STD=c++17` fails), 48 move-semantics, 49 lambdas, 50 ranges-and-algorithms, 51 error-handling (`optional`/`expected`(C++23)/exceptions).

## Phase 9 — Capstone + references (52)
Author `52-mini-renderer` per the spec's milestones (PPM/TGA image → vec math → line → wireframe → triangle rasterizer → z-buffer → shading), `include/`+`src/`, assets tiny/procedural. Fill `references/debugging-and-tooling.md` and `references/going-further-gpu.md`. Update the root README learning-path links.

---

## Self-Review

- **Spec coverage:** every spec lesson (01–52), both reference docs, capstone, template/Makefile/README changes, renumber map, and C-counterpart set map to a phase/task. ✓
- **Placeholders:** Phase 1 tasks carry full code. Phases 2–9 intentionally carry a *recipe + per-lesson scope* rather than full source — this is deliberate staged detail (fleshed out at execution per your "gradually" instruction), not a hidden TODO. The recipe itself is fully specified.
- **Type consistency:** Makefile variable/target names (`build`,`run`,`run-c`,`STD`,`CSTD`,`CXX`,`CC`,`BIN`,`BIN_C`) are consistent between the top-level and per-app Makefiles and the README usage.
