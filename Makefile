# =============================================================
# Central build for the C / C++ learning workspace.
#
# Lessons under apps/c and apps/cpp build with NO per-lesson Makefile — the
# rules below compile src/ (+ optional include/) for you. When a lesson (or a
# project) needs more, drop a Makefile in its folder and this workspace hands
# the build off to it. Minimum by default; your own build when you outgrow it.
#
#   make run    app=01-hello-world      # C++  (apps/cpp/01-hello-world)
#   make run-c  app=01-hello-world      # C    (apps/c/01-hello-world)
#   make build  app=45-classes-basics   # build one C++ lesson
#   make build                          # build every lesson (both languages)
#   make list / make list-c             # list C++ / C lessons
#
# Switch standards to compare old vs modern:
#   make run app=20-strings STD=c++98      make run-c app=39-pointers CSTD=c99
#
# Your own projects (own folder name, own structure, own Makefile):
#   make new-project name=my-thing            # scaffold apps/my-thing
#   make project     name=my-thing            # build & run it
#   make list-projects
#
# Follow a tutorial in your OWN scratch copy (never touches the reference):
#   make practice app=01-hello-world          # -> practice/cpp/01-hello-world
#   make practice app=01-hello-world lang=c    # -> practice/c/01-hello-world
#   make run   app=01-hello-world PRACTICE=1   # build & run YOUR copy
#   make run-c app=01-hello-world PRACTICE=1
# =============================================================
APP_DIR      := apps
BUILD_DIR    := build
TEMPLATE_DIR := templates/app

# LLVM/Clang release fetched by `make install` on Linux/Windows (bump here, or
# override: make install LLVM_VERSION=20.1.0).
LLVM_VERSION ?= 19.1.7

# Where sources come from. PRACTICE=1 switches to your scratch copies under
# practice/ so `run`/`build` use your code instead of the reference lesson.
SRC_ROOT := $(if $(PRACTICE),practice,$(APP_DIR))

# Compiler: prefer a repo-local toolchain from `make install` (./toolchain) when
# present, otherwise make's built-in c++/cc. An explicit CXX=/CC= (command line
# or environment) always wins — note CXX/CC are make built-ins, so `?=` can't
# override them; we guard on $(origin ...)=default and assign with abspath so a
# delegated sub-Makefile finds the compiler from its own directory.
ifneq ($(wildcard toolchain/bin/clang++),)
  ifeq ($(origin CXX),default)
    CXX := $(abspath toolchain/bin/clang++)
  endif
  ifeq ($(origin CC),default)
    CC := $(abspath toolchain/bin/clang)
  endif
endif
STD  ?= c++23
CSTD ?= c17
BUILD_TYPE ?= Debug

WARN := -Wall -Wextra -Wpedantic
ifeq ($(BUILD_TYPE),Release)
  OPT := -O2 -DNDEBUG
  SAN :=
else
  OPT := -O0 -g
  SAN := -fsanitize=address,undefined
endif
# Each flag combination gets its own build dir, so switching STD never reuses
# objects compiled under a different standard.
SIG := $(BUILD_TYPE)-$(STD)-$(CSTD)

# Handed down to any sub-Makefile (a per-app override or a project) so it
# inherits the same compiler/standard/build type but can override them.
DELEGATE_VARS := CXX='$(CXX)' CC='$(CC)' STD='$(STD)' CSTD='$(CSTD)' BUILD_TYPE='$(BUILD_TYPE)'

CPP_APPS := $(notdir $(wildcard $(APP_DIR)/cpp/*))
C_APPS   := $(notdir $(wildcard $(APP_DIR)/c/*))
# Your own projects live under apps/ next to the c/ and cpp/ lesson trees: any
# folder name except 'c'/'cpp', any structure, its own Makefile. They are the
# apps/ subdirs that aren't the two lesson trees. (Shell listing because GNU
# make 3.81's $(wildcard */) doesn't filter to directories reliably.)
PROJECTS := $(filter-out c cpp,$(shell ls -d $(APP_DIR)/*/ 2>/dev/null | sed 's:/*$$::;s:.*/::'))

# Resolve the language from the goal: run-c/build-c build C, everything else C++.
ifneq ($(filter run-c build-c,$(MAKECMDGOALS)),)
  TREE     := c
  EXT      := c
  COMPILER := $(CC)
  STDFLAG  := -std=$(CSTD)
else
  TREE     := cpp
  EXT      := cpp
  COMPILER := $(CXX)
  STDFLAG  := -std=$(STD)
endif

define require_app
$(if $(app),,$(error Usage: make $(1) app=<name>))
endef

define require_name
$(if $(name),,$(error Usage: make $(1) name=<project>))
endef

.PHONY: help install list list-c list-projects build build-c build-all run run-c \
        clean delete-app app practice project project-build new-project

help:
	@echo "Targets:"
	@echo "  make install                  Put a C/C++ compiler in ./toolchain (or verify it)"
	@echo "  make run    app=<name>        Build & run a C++ lesson (apps/cpp/<name>)"
	@echo "  make run-c  app=<name>        Build & run a C lesson   (apps/c/<name>)"
	@echo "  make build [app=<name>]       Build one C++ lesson, or ALL lessons if omitted"
	@echo "  make build-c app=<name>       Build one C lesson"
	@echo "  make list / make list-c       List C++ / C lessons"
	@echo "  make app app=<name> [lang=c]  Scaffold a new lesson"
	@echo "  make practice app=<name> [lang=c]  Make a scratch copy to follow a tutorial"
	@echo "  make clean                    Remove all build output"
	@echo "  make delete-app app=<name>    Delete a lesson (both trees)"
	@echo ""
	@echo "Your own projects (own name, own structure, own Makefile):"
	@echo "  make new-project name=<name>  Scaffold apps/<name> (own Makefile + src/)"
	@echo "  make project     name=<name>  Build & run a project"
	@echo "  make project-build name=<name>  Build a project"
	@echo "  make list-projects            List your projects"
	@echo ""
	@echo "A lesson may ALSO carry its own Makefile (apps/<tree>/<name>/Makefile);"
	@echo "if present, the workspace delegates build/run to it (needs build + run targets)."
	@echo ""
	@echo "Variables: STD=c++98..c++23 (def c++23)  CSTD=c99|c11|c17"
	@echo "           BUILD_TYPE=Debug|Release  CXX=g++|clang++  CC=gcc|clang"
	@echo "           PRACTICE=1  build/run your scratch copy under practice/"

# Put a self-contained compiler in ./toolchain (git-ignored) so the workspace
# builds without a system-wide install. macOS verifies the Xcode Command Line
# Tools instead (the compiler needs Apple's SDK, which can't live in the repo).
# Linux/Windows(MSYS) download a prebuilt LLVM release and extract it.
install:
	@set -e; os="$$(uname -s)"; arch="$$(uname -m)"; dest="toolchain"; ver="$(LLVM_VERSION)"; \
	case "$$os" in \
	  Darwin) \
	    echo "macOS detected."; \
	    if xcode-select -p >/dev/null 2>&1; then \
	      echo "Xcode Command Line Tools present."; \
	      echo "Using system clang: $$(clang++ --version 2>/dev/null | head -1)"; \
	      echo; \
	      echo "On macOS the compiler relies on Apple's SDK, so there is nothing to"; \
	      echo "put in ./$$dest — you are ready to build:  make run app=01-hello-world"; \
	    else \
	      echo "Xcode Command Line Tools NOT found. Install once (clang + macOS SDK):"; \
	      echo "    xcode-select --install"; exit 1; \
	    fi ;; \
	  Linux|MINGW*|MSYS*|CYGWIN*) \
	    if [ -x "$$dest/bin/clang++" ]; then \
	      echo "Toolchain already present: $$("$$dest/bin/clang++" --version | head -1)"; exit 0; \
	    fi; \
	    case "$$os" in Linux) plat="Linux" ;; *) plat="Windows" ;; esac; \
	    case "$$arch" in \
	      x86_64|amd64)  a="X64" ;; \
	      aarch64|arm64) a="ARM64" ;; \
	      *) echo "Unsupported arch '$$arch' — see the README (Installing a compiler)"; exit 1 ;; \
	    esac; \
	    asset="LLVM-$$ver-$$plat-$$a.tar.xz"; \
	    url="https://github.com/llvm/llvm-project/releases/download/llvmorg-$$ver/$$asset"; \
	    echo "Fetching repo-local LLVM/Clang $$ver ($$plat-$$a):"; echo "  $$url"; \
	    mkdir -p "$$dest"; tmp="$$(mktemp -d)"; \
	    if command -v curl >/dev/null 2>&1; then curl -fL "$$url" -o "$$tmp/llvm.tar.xz"; \
	    elif command -v wget >/dev/null 2>&1; then wget -O "$$tmp/llvm.tar.xz" "$$url"; \
	    else echo "Need 'curl' or 'wget' to download."; rm -rf "$$tmp"; exit 1; fi; \
	    echo "Extracting into ./$$dest ..."; \
	    tar -xJf "$$tmp/llvm.tar.xz" -C "$$dest" --strip-components=1; rm -rf "$$tmp"; \
	    if [ -x "$$dest/bin/clang++" ]; then \
	      echo "Installed: $$("$$dest/bin/clang++" --version | head -1)"; \
	      echo "The workspace now prefers ./$$dest/bin/clang++ automatically."; \
	    else \
	      echo "Downloaded, but $$dest/bin/clang++ not found — asset layout may differ."; \
	      echo "See the README (Installing a compiler) for the native-install fallback."; exit 1; \
	    fi ;; \
	  *) echo "Unrecognized OS '$$os'. See the README (Installing a compiler) for manual setup."; exit 1 ;; \
	esac

list:
	@printf "%s\n" $(CPP_APPS)

list-c:
	@printf "%s\n" $(C_APPS)

list-projects:
	@printf "%s\n" $(PROJECTS)

clean:
	@echo "Cleaning $(BUILD_DIR)/*"
	@rm -rf "$(BUILD_DIR)"/* 2>/dev/null || true

delete-app:
	$(call require_app,delete-app)
	@rm -rf "$(APP_DIR)/cpp/$(app)" "$(APP_DIR)/c/$(app)" \
	        "$(BUILD_DIR)/$(APP_DIR)/cpp/$(app)" "$(BUILD_DIR)/$(APP_DIR)/c/$(app)" 2>/dev/null || true
	@echo "Deleted: $(app)"

app:
	$(call require_app,app)
	$(eval T := $(if $(filter c,$(lang)),c,cpp))
	@mkdir -p "$(APP_DIR)/$(T)/$(app)/src"
	@for tpl in $(wildcard $(TEMPLATE_DIR)/*.tpl); do \
		out=$$(basename "$$tpl" .tpl); \
		case "$$out" in \
			main.cpp) [ "$(T)" = c ] && continue; dest="$(APP_DIR)/$(T)/$(app)/src/main.cpp" ;; \
			main.c)   [ "$(T)" = c ] || continue; dest="$(APP_DIR)/$(T)/$(app)/src/main.c" ;; \
			*)        dest="$(APP_DIR)/$(T)/$(app)/$$out" ;; \
		esac; \
		sed 's/__APP_NAME__/$(app)/g' "$$tpl" > "$$dest"; \
	done
	@echo "Created $(APP_DIR)/$(T)/$(app)"

# Make a personal scratch copy to follow a tutorial in. Writes a starter file
# ONLY if one isn't there yet, so re-running never clobbers your work.
practice:
	$(call require_app,practice)
	$(eval T := $(if $(filter c,$(lang)),c,cpp))
	$(eval E := $(if $(filter c,$(lang)),c,cpp))
	@mkdir -p "practice/$(T)/$(app)/src"
	@dest="practice/$(T)/$(app)/src/main.$(E)"; \
	 if [ -e "$$dest" ]; then \
		echo "Kept existing $$dest (not overwritten)"; \
	 elif [ "$(E)" = c ]; then \
		printf '/* Practice: %s\n   Follow the tutorial: apps/c/%s/README.md\n   Build & run YOUR copy: make run-c app=%s PRACTICE=1 */\n#include <stdio.h>\n\nint main(void) {\n    /* TODO: follow the README and build it up step by step. */\n    printf("practice: %s\\n");\n    return 0;\n}\n' "$(app)" "$(app)" "$(app)" "$(app)" > "$$dest"; \
		echo "Created $$dest"; \
	 else \
		printf '// Practice: %s\n// Follow the tutorial: apps/cpp/%s/README.md\n// Build & run YOUR copy: make run app=%s PRACTICE=1\n#include <iostream>\n\nint main() {\n    // TODO: follow the README and build it up step by step.\n    std::cout << "practice: %s\\n";\n    return 0;\n}\n' "$(app)" "$(app)" "$(app)" "$(app)" > "$$dest"; \
		echo "Created $$dest"; \
	 fi
	@echo "Now: make run$(if $(filter c,$(lang)),-c,) app=$(app) PRACTICE=1"

# --- your own projects: each owns its Makefile; we just delegate ---
new-project:
	$(call require_name,new-project)
	@if [ "$(name)" = "c" ] || [ "$(name)" = "cpp" ]; then echo "'c' and 'cpp' are the lesson trees — choose another project name"; exit 1; fi
	@if [ -e "$(APP_DIR)/$(name)" ]; then echo "$(APP_DIR)/$(name) already exists — not overwritten"; exit 1; fi
	@mkdir -p "$(APP_DIR)/$(name)/src"
	@sed 's/__PROJECT_NAME__/$(name)/g' templates/project/Makefile.tpl  > "$(APP_DIR)/$(name)/Makefile"
	@sed 's/__PROJECT_NAME__/$(name)/g' templates/project/main.cpp.tpl  > "$(APP_DIR)/$(name)/src/main.cpp"
	@sed 's/__PROJECT_NAME__/$(name)/g' templates/project/README.md.tpl > "$(APP_DIR)/$(name)/README.md"
	@echo "Created $(APP_DIR)/$(name) — build & run: make project name=$(name)"

project:
	$(call require_name,project)
	@test -f "$(APP_DIR)/$(name)/Makefile" || { echo "No $(APP_DIR)/$(name)/Makefile — a project owns its build. Scaffold one: make new-project name=$(name)"; exit 1; }
	@$(MAKE) --no-print-directory -C "$(APP_DIR)/$(name)" run $(DELEGATE_VARS) ARGS='$(ARGS)'

project-build:
	$(call require_name,project-build)
	@test -f "$(APP_DIR)/$(name)/Makefile" || { echo "No $(APP_DIR)/$(name)/Makefile — a project owns its build. Scaffold one: make new-project name=$(name)"; exit 1; }
	@$(MAKE) --no-print-directory -C "$(APP_DIR)/$(name)" build $(DELEGATE_VARS)

build-all:
	@for a in $(CPP_APPS); do $(MAKE) --no-print-directory build   app=$$a || exit 1; done
	@for a in $(C_APPS);   do $(MAKE) --no-print-directory build-c app=$$a || exit 1; done
	@echo "Built all lessons."

# --- single-lesson build (only when app= is set) ---
ifdef app
LDIR := $(SRC_ROOT)/$(TREE)/$(app)

ifeq ($(wildcard $(LDIR)/Makefile),)
# Default built-in build: compile src/ (+ optional include/).
SRCS     := $(wildcard $(LDIR)/src/*.$(EXT))
OBJDIR   := $(BUILD_DIR)/$(SRC_ROOT)/$(TREE)/$(app)/$(SIG)
BINDIR   := $(OBJDIR)/bin
BIN      := $(BINDIR)/$(app)
OBJS     := $(patsubst $(LDIR)/src/%.$(EXT),$(OBJDIR)/%.o,$(SRCS))
INC      := $(addprefix -I,$(wildcard $(LDIR)/include)) -I$(LDIR)/src
ALLFLAGS := $(STDFLAG) $(WARN) $(OPT) $(SAN) $(INC) -MMD -MP

$(BINDIR):
	@mkdir -p "$(BINDIR)"

$(OBJDIR)/%.o: $(LDIR)/src/%.$(EXT) | $(BINDIR)
	$(COMPILER) $(ALLFLAGS) -c $< -o $@

$(BIN): $(OBJS)
	$(COMPILER) $(OBJS) -o "$@" $(SAN)

build:   $(BIN)
build-c: $(BIN)

run: $(BIN)
	@"$(BIN)" $(ARGS)

run-c: $(BIN)
	@"$(BIN)" $(ARGS)

-include $(OBJS:.o=.d)
else
# This lesson brings its own Makefile — hand the build off to it (recursive
# make). Contract: that Makefile provides `build` and `run` targets.
build build-c:
	@$(MAKE) --no-print-directory -C "$(LDIR)" build $(DELEGATE_VARS)

run run-c:
	@$(MAKE) --no-print-directory -C "$(LDIR)" run $(DELEGATE_VARS) ARGS='$(ARGS)'
endif

else
build: build-all
run:
	$(call require_app,run)
run-c:
	$(call require_app,run-c)
endif
