# =============================================================
# Build launcher for the C / C++ learning apps.
#
# Each app lives in apps/<lang>/<name>/ and has a tiny Makefile with a single
# `all:` rule. THIS Makefile owns the compiler flags, exports them, and drives
# each app's build. You never pass app=... — the app NAME is part of the target:
#
#   make new-cpp-app-NAME      create apps/cpp/NAME from the template
#   make build-cpp-app-NAME    build it              (…-c-app-NAME for C)
#   make run-cpp-app-NAME      build & run it        (run-c-app-NAME for C)
#   make delete-cpp-app-NAME   delete it (source + build)
#   make build                 build every app
#   make list                  list all apps         (list-cpp / list-c)
#   make clean                 remove build output
#   make install               set up a compiler in ./toolchain (or verify it)
#
# Choose a standard / build type / arguments on the command line:
#   make run-cpp-app-20-strings STD=c++98
#   make run-cpp-app-mandelbrot BUILD_TYPE=Release
#   make run-cpp-app-calc ARGS='1 + 2 * 3'
# =============================================================

# --- config (override on the command line) -----------------------------------
CXX        ?= c++
CC         ?= cc
STD        ?= c++23
CSTD       ?= c17
BUILD_TYPE ?= Debug

APP_DIR      := apps
BUILD_DIR    := build/apps
TEMPLATE_DIR := templates
LLVM_VERSION ?= 19.1.7

# Prefer a repo-local ./toolchain (from `make install`) unless a compiler was
# named explicitly. (CXX/CC are make built-ins, hence the $(origin ...) test.)
ifneq ($(wildcard toolchain/bin/clang++),)
  ifeq ($(origin CXX),default)
    CXX := $(abspath toolchain/bin/clang++)
  endif
  ifeq ($(origin CC),default)
    CC := $(abspath toolchain/bin/clang)
  endif
endif

# --- flags --------------------------------------------------------------------
WARN := -Wall -Wextra -Wpedantic
ifeq ($(BUILD_TYPE),Debug)
  OPT := -O0 -g -fsanitize=address,undefined     # debuggable; catches memory/UB bugs
else ifeq ($(BUILD_TYPE),Release)
  OPT := -O2 -DNDEBUG                            # optimized; asserts compiled out
else
  $(error Invalid BUILD_TYPE '$(BUILD_TYPE)'. Use Debug or Release.)
endif

CXXFLAGS := -std=$(STD)  $(WARN) $(OPT)
CFLAGS   := -std=$(CSTD) $(WARN) $(OPT)

# Handed down to every app's Makefile — its `all:` rule builds with these.
export CXX CC CXXFLAGS CFLAGS

# --- app discovery (an app is any folder with a src/) -------------------------
CPP_APPS := $(sort $(notdir $(patsubst %/src,%,$(shell find $(APP_DIR)/cpp -type d -name src 2>/dev/null))))
C_APPS   := $(sort $(notdir $(patsubst %/src,%,$(shell find $(APP_DIR)/c   -type d -name src 2>/dev/null))))

.PHONY: help install build clean list list-cpp list-c

help:
	@echo "Targets (NAME is the app folder name):"
	@echo "  make new-cpp-app-NAME      Create apps/cpp/NAME     (new-c-app-NAME for C)"
	@echo "  make build-cpp-app-NAME    Build it                 (build-c-app-NAME for C)"
	@echo "  make run-cpp-app-NAME      Build & run it           (run-c-app-NAME for C)"
	@echo "  make delete-cpp-app-NAME   Delete it                (delete-c-app-NAME for C)"
	@echo "  make build                 Build every app"
	@echo "  make list                  List all apps            (list-cpp / list-c)"
	@echo "  make clean                 Remove build/ output"
	@echo "  make install               Put a compiler in ./toolchain (or verify it)"
	@echo ""
	@echo "Every app's Makefile is one line that compiles src/ with the flags this"
	@echo "Makefile exports. Change how things build in ONE place: the flags below."
	@echo ""
	@echo "Variables: STD=c++98..c++23 (def c++23)  CSTD=c99|c11|c17"
	@echo "           BUILD_TYPE=Debug|Release  CXX=g++|clang++  CC=gcc|clang  ARGS='...'"

# --- create (copy the template folder verbatim) ------------------------------
define new_app                                       # $(1) = lang
	@test ! -d "$(APP_DIR)/$(1)/$*" || { echo "App '$*' already exists at $(APP_DIR)/$(1)/$*"; exit 1; }
	@mkdir -p "$(APP_DIR)/$(1)/$*"
	@cp -R "$(TEMPLATE_DIR)/$(1)/." "$(APP_DIR)/$(1)/$*/"
	@echo "Created $(APP_DIR)/$(1)/$*  —  build & run: make run-$(1)-app-$*"
endef

new-cpp-app-%: ; $(call new_app,cpp)
new-c-app-%:   ; $(call new_app,c)

# --- build one app: hand the compile to its Makefile, passing name + out dir --
define build_app                                     # $(1) = lang
	@mkdir -p "$(BUILD_DIR)/$(1)/$*"
	@$(MAKE) --no-print-directory -C "$(APP_DIR)/$(1)/$*" \
		BUILD_DIR="$(abspath $(BUILD_DIR)/$(1)/$*)" OUTPUT_NAME="$*"
	@echo "Built $(BUILD_DIR)/$(1)/$*/$*"
endef

build-cpp-app-%: ; $(call build_app,cpp)
build-c-app-%:   ; $(call build_app,c)

# --- build everything ---------------------------------------------------------
build: $(addprefix build-cpp-app-,$(CPP_APPS)) $(addprefix build-c-app-,$(C_APPS))
	@echo "Built all apps."

# --- run (builds first, then executes from the repo root so ARGS paths work) --
define run_app                                       # $(1) = lang
	@bin="$(BUILD_DIR)/$(1)/$*/$*"; \
	 test -x "$$bin" || { echo "run: '$*' was not built"; exit 1; }; \
	 "./$$bin" $(ARGS)
endef

run-cpp-app-%: build-cpp-app-% ; $(call run_app,cpp)
run-c-app-%:   build-c-app-%   ; $(call run_app,c)

# --- delete -------------------------------------------------------------------
define delete_app                                    # $(1) = lang
	@test -d "$(APP_DIR)/$(1)/$*" || { echo "App '$*' does not exist at $(APP_DIR)/$(1)/$*"; exit 1; }
	@rm -rf "$(APP_DIR)/$(1)/$*" "$(BUILD_DIR)/$(1)/$*"
	@echo "Deleted $(APP_DIR)/$(1)/$*"
endef

delete-cpp-app-%: ; $(call delete_app,cpp)
delete-c-app-%:   ; $(call delete_app,c)

# --- list ---------------------------------------------------------------------
list: list-cpp list-c
list-cpp:
	@echo "C++ apps:";  printf '    %s\n' $(CPP_APPS)
list-c:
	@echo "C apps:";    printf '    %s\n' $(C_APPS)

# --- clean --------------------------------------------------------------------
clean:
	@rm -rf build
	@echo "Cleaned build/."

# --- install a repo-local compiler -------------------------------------------
# macOS verifies the Xcode Command Line Tools (the compiler needs Apple's SDK,
# which can't live in the repo). Linux/Windows(MSYS) download a prebuilt LLVM.
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
	      echo "put in ./$$dest — you are ready to build:  make run-cpp-app-01-hello-world"; \
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
	      echo "Downloaded, but $$dest/bin/clang++ not found — asset layout may differ."; exit 1; \
	    fi ;; \
	  *) echo "Unrecognized OS '$$os'. See the README (Installing a compiler)."; exit 1 ;; \
	esac
