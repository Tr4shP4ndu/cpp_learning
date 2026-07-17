# =============================================================
# Launcher for the C / C++ learning apps.
#
# Every app has its OWN small Makefile (two lines: `APP := <name>` then an
# include of rules.mk). So the simplest way to build anything is to go into it:
#
#     cd apps/cpp/calc && make run
#
# This top-level Makefile is just a convenience layer: give it an app NAME and
# it finds the folder for you and runs that app's Makefile — plus scaffolding,
# listing, the toolchain setup, and clean.
#
#   make run   app=calc              # find 'calc', build & run it
#   make run   app=calc ARGS='1 + 2'  # pass arguments through
#   make build app=45-classes-basics  # build one app
#   make build                        # build every app
#   make list / make list-c           # list C++ / C apps
#   make app app=my-thing [lang=c] [group=graphics]   # scaffold a new app
#   make install                      # put a compiler in ./toolchain (or verify)
#   make clean                        # remove every app's build/ output
#
# Standards & build type pass straight through to the app's Makefile:
#   make run app=20-strings STD=c++98        make run app=mandelbrot BUILD_TYPE=Release
#
# The full build recipe lives in ./rules.mk (documented there). An app can opt
# out of the shared rules by writing its own real Makefile instead of the
# two-line include — this launcher just calls whatever Makefile it finds.
#
# Follow a tutorial in your OWN scratch copy (never touches the reference app):
#   make practice app=01-hello-world           # -> practice/cpp/01-hello-world
#   make run      app=01-hello-world PRACTICE=1  # build & run YOUR copy
# =============================================================
APP_DIR      := apps
TEMPLATE_DIR := templates/app
LLVM_VERSION ?= 19.1.7

# PRACTICE=1 operates on your scratch copies under practice/ instead of apps/.
SEARCH_ROOT := $(if $(PRACTICE),practice,$(APP_DIR))

# Overrides we forward to an app's Makefile — but ONLY the ones you actually set
# on the command line, so we never clobber the app's own sensible defaults.
PASS := $(if $(STD),STD='$(STD)') $(if $(CSTD),CSTD='$(CSTD)') \
        $(if $(BUILD_TYPE),BUILD_TYPE='$(BUILD_TYPE)') \
        $(if $(CXX),CXX='$(CXX)') $(if $(CC),CC='$(CC)')

# Find an app folder by name, anywhere under the search root (direct or grouped).
find_app = find "$(SEARCH_ROOT)" -type d -name '$(app)' 2>/dev/null | head -n1

# Every app owns a src/ dir, so we discover apps by locating those.
CPP_APPS := $(sort $(notdir $(patsubst %/src,%,$(shell find $(APP_DIR)/cpp -type d -name src 2>/dev/null))))
C_APPS   := $(sort $(notdir $(patsubst %/src,%,$(shell find $(APP_DIR)/c   -type d -name src 2>/dev/null))))
ALL_APP_DIRS := $(patsubst %/src,%,$(shell find $(APP_DIR) -type d -name src 2>/dev/null | sort))

define require_app
$(if $(app),,$(error Usage: make $(1) app=<name>))
endef

.PHONY: help install list list-c build build-c build-all run run-c \
        clean delete-app app practice

help:
	@echo "Targets:"
	@echo "  make install                  Put a C/C++ compiler in ./toolchain (or verify it)"
	@echo "  make run    app=<name>        Find an app, build & run it (C or C++)"
	@echo "  make build [app=<name>]       Build one app, or ALL apps if omitted"
	@echo "  make list / make list-c       List C++ / C apps"
	@echo "  make app app=<name> [lang=c] [group=<g>]  Scaffold a new app"
	@echo "  make practice app=<name> [lang=c]  Make a scratch copy to follow a tutorial"
	@echo "  make clean                    Remove every app's build/ output"
	@echo "  make delete-app app=<name>    Delete an app (both trees)"
	@echo ""
	@echo "Each app has its own two-line Makefile that includes ./rules.mk, so you"
	@echo "can also just:  cd apps/cpp/<name> && make run"
	@echo ""
	@echo "Apps live under apps/<cpp|c>/, optionally inside a named group folder"
	@echo "(apps/cpp/graphics/<name>); they are found by name wherever they sit."
	@echo ""
	@echo "Variables (forwarded to the app): STD=c++98..c++23 (def c++23)  CSTD=c99|c11|c17"
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

# run and run-c are the same — the app's rules.mk detects C vs C++ itself.
# run-c is kept so the C lessons' READMEs keep working.
run run-c:
	$(call require_app,run)
	@d=$$($(find_app)); \
	 [ -n "$$d" ] || { echo "No app named '$(app)' under $(SEARCH_ROOT)/"; exit 1; }; \
	 $(MAKE) --no-print-directory -C "$$d" run ARGS='$(ARGS)' $(PASS)

# build one app; with no app=, build them all.
build build-c:
	@if [ -n "$(app)" ]; then \
	   d=$$($(find_app)); \
	   [ -n "$$d" ] || { echo "No app named '$(app)' under $(SEARCH_ROOT)/"; exit 1; }; \
	   $(MAKE) --no-print-directory -C "$$d" build $(PASS); \
	 else \
	   $(MAKE) --no-print-directory build-all; \
	 fi

build-all:
	@set -e; for d in $(ALL_APP_DIRS); do \
	   echo "==> $$d"; $(MAKE) --no-print-directory -C "$$d" build $(PASS); \
	 done; echo "Built all apps."

clean:
	@find $(APP_DIR) practice -type d -name build -prune -exec rm -rf {} + 2>/dev/null || true
	@rm -rf build
	@echo "Cleaned all build output."

delete-app:
	$(call require_app,delete-app)
	@for t in cpp c; do \
	  d=$$(find "$(APP_DIR)/$$t" -type d -name '$(app)' 2>/dev/null | head -n1); \
	  if [ -n "$$d" ]; then rm -rf "$$d"; echo "Removed $$d"; fi; \
	done
	@echo "Deleted: $(app)"

# Scaffold a new app under apps/<lang>/, or inside a named group folder when
# group=<g> is given (apps/<lang>/<group>/<app>). Drops src/main.*, a README,
# and the two-line Makefile that includes rules.mk.
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
	@echo "Created $(APP_DIR)/$(T)/$(REL) — build & run: make run app=$(app)"

# Make a personal scratch copy to follow a tutorial in. Writes a starter source
# file ONLY if none exists (never clobbers your work) plus its Makefile.
practice:
	$(call require_app,practice)
	$(eval T := $(if $(filter c,$(lang)),c,cpp))
	$(eval E := $(if $(filter c,$(lang)),c,cpp))
	@mkdir -p "practice/$(T)/$(app)/src"
	@sed 's/__APP_NAME__/$(app)/g' "$(TEMPLATE_DIR)/Makefile.tpl" > "practice/$(T)/$(app)/Makefile"
	@dest="practice/$(T)/$(app)/src/main.$(E)"; \
	 if [ -e "$$dest" ]; then \
		echo "Kept existing $$dest (not overwritten)"; \
	 elif [ "$(E)" = c ]; then \
		printf '/* Practice: %s\n   Follow the tutorial: apps/c/%s/README.md\n   Build & run YOUR copy: make run app=%s PRACTICE=1 */\n#include <stdio.h>\n\nint main(void) {\n    /* TODO: follow the README and build it up step by step. */\n    printf("practice: %s\\n");\n    return 0;\n}\n' "$(app)" "$(app)" "$(app)" "$(app)" > "$$dest"; \
		echo "Created $$dest"; \
	 else \
		printf '// Practice: %s\n// Follow the tutorial: apps/cpp/%s/README.md\n// Build & run YOUR copy: make run app=%s PRACTICE=1\n#include <iostream>\n\nint main() {\n    // TODO: follow the README and build it up step by step.\n    std::cout << "practice: %s\\n";\n    return 0;\n}\n' "$(app)" "$(app)" "$(app)" "$(app)" > "$$dest"; \
		echo "Created $$dest"; \
	 fi
	@echo "Now: make run app=$(app) PRACTICE=1"
