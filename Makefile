# =============================================================
# Central build for the C / C++ learning workspace.
#
# Apps under apps/c and apps/cpp build with NO per-app Makefile — the rules
# below compile src/ (+ optional include/) for you. When an app needs more,
# drop a Makefile next to its src/ and this workspace hands the build off to
# it. Minimum by default; your own build when you outgrow it.
#
#   make run    app=01-hello-world      # C++  (apps/cpp/01-hello-world)
#   make run-c  app=01-hello-world      # C    (apps/c/01-hello-world)
#   make build  app=45-classes-basics   # build one C++ app
#   make build                          # build every app (both languages)
#   make list / make list-c             # list C++ / C apps
#
# Switch standards to compare old vs modern:
#   make run app=20-strings STD=c++98      make run-c app=39-pointers CSTD=c99
#
# An app can sit directly under a language, OR inside a named group folder —
# both are found by name, so the run/build commands don't change:
#   apps/cpp/mandelbrot                 apps/cpp/graphics/mandelbrot
#   make app app=my-thing lang=cpp                 # apps/cpp/my-thing
#   make app app=my-thing lang=cpp group=graphics  # apps/cpp/graphics/my-thing
#   make run app=my-thing                          # resolves it at any depth
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

# Every app owns a src/ dir. Apps live directly under a language (apps/cpp/foo)
# or nested one level in a named group (apps/cpp/graphics/foo); either way we
# find them by locating their src/ dir, so groups need no bookkeeping here.
CPP_APPS := $(sort $(notdir $(patsubst %/src,%,$(shell find $(APP_DIR)/cpp -type d -name src 2>/dev/null))))
C_APPS   := $(sort $(notdir $(patsubst %/src,%,$(shell find $(APP_DIR)/c   -type d -name src 2>/dev/null))))

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

.PHONY: help install list list-c build build-c build-all run run-c \
        clean delete-app app practice

help:
	@echo "Targets:"
	@echo "  make install                  Put a C/C++ compiler in ./toolchain (or verify it)"
	@echo "  make run    app=<name>        Build & run a C++ app (apps/cpp/<name>)"
	@echo "  make run-c  app=<name>        Build & run a C app   (apps/c/<name>)"
	@echo "  make build [app=<name>]       Build one C++ app, or ALL apps if omitted"
	@echo "  make build-c app=<name>       Build one C app"
	@echo "  make list / make list-c       List C++ / C apps"
	@echo "  make app app=<name> [lang=c] [group=<g>]  Scaffold a new app"
	@echo "  make practice app=<name> [lang=c]  Make a scratch copy to follow a tutorial"
	@echo "  make clean                    Remove all build output"
	@echo "  make delete-app app=<name>    Delete an app (both trees)"
	@echo ""
	@echo "Apps sit under apps/<cpp|c>/, optionally inside a named group folder"
	@echo "(apps/cpp/graphics/<name>). They are found by name at any depth, so the"
	@echo "run/build commands are the same wherever an app lives."
	@echo ""
	@echo "An app may ALSO carry its own Makefile (next to its src/); if present,"
	@echo "the workspace delegates build/run to it (needs build + run targets)."
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

clean:
	@echo "Cleaning $(BUILD_DIR)/*"
	@rm -rf "$(BUILD_DIR)"/* 2>/dev/null || true

delete-app:
	$(call require_app,delete-app)
	@for t in cpp c; do \
	  d=$$(find "$(APP_DIR)/$$t" -type d -name '$(app)' 2>/dev/null | head -n1); \
	  if [ -n "$$d" ]; then rm -rf "$$d"; echo "Removed $$d"; fi; \
	  rm -rf "$(BUILD_DIR)/$(APP_DIR)/$$t/$(app)"; \
	done
	@echo "Deleted: $(app)"

# Scaffold a new app under apps/<lang>/, or inside a named group folder when
# group=<g> is given (apps/<lang>/<group>/<app>).
app:
	$(call require_app,app)
	$(eval T   := $(if $(filter c,$(lang)),c,cpp))
	$(eval REL := $(if $(group),$(group)/$(app),$(app)))
	@mkdir -p "$(APP_DIR)/$(T)/$(REL)/src"
	@for tpl in $(wildcard $(TEMPLATE_DIR)/*.tpl); do \
		out=$$(basename "$$tpl" .tpl); \
		case "$$out" in \
			main.cpp) [ "$(T)" = c ] && continue; dest="$(APP_DIR)/$(T)/$(REL)/src/main.cpp" ;; \
			main.c)   [ "$(T)" = c ] || continue; dest="$(APP_DIR)/$(T)/$(REL)/src/main.c" ;; \
			*)        dest="$(APP_DIR)/$(T)/$(REL)/$$out" ;; \
		esac; \
		sed 's/__APP_NAME__/$(app)/g' "$$tpl" > "$$dest"; \
	done
	@echo "Created $(APP_DIR)/$(T)/$(REL) — build & run: make run$(if $(filter c,$(lang)),-c,) app=$(app)"

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

build-all:
	@for a in $(CPP_APPS); do $(MAKE) --no-print-directory build   app=$$a || exit 1; done
	@for a in $(C_APPS);   do $(MAKE) --no-print-directory build-c app=$$a || exit 1; done
	@echo "Built all apps."

# --- single-app build (only when app= is set) ---
ifdef app
# Prefer the direct path (apps/<tree>/<app>); if it isn't there, the app lives
# in a group folder (apps/<tree>/<group>/<app>) — find it by name.
LDIR := $(SRC_ROOT)/$(TREE)/$(app)
ifeq ($(wildcard $(LDIR)),)
LDIR := $(shell find "$(SRC_ROOT)/$(TREE)" -type d -name '$(app)' 2>/dev/null | head -n1)
endif

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
# This app brings its own Makefile — hand the build off to it (recursive
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
