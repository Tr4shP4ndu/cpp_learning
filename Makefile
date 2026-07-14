# =============================================================
# Central build for the C / C++ learning workspace.
# ONE Makefile builds every lesson — there are no per-lesson Makefiles.
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
# Follow a tutorial in your OWN scratch copy (never touches the reference):
#   make practice app=01-hello-world          # -> practice/cpp/01-hello-world
#   make practice app=01-hello-world lang=c    # -> practice/c/01-hello-world
#   make run   app=01-hello-world PRACTICE=1   # build & run YOUR copy
#   make run-c app=01-hello-world PRACTICE=1
# =============================================================
APP_DIR      := apps
BUILD_DIR    := build
TEMPLATE_DIR := templates/app

# Where sources come from. PRACTICE=1 switches to your scratch copies under
# practice/ so `run`/`build` use your code instead of the reference lesson.
SRC_ROOT := $(if $(PRACTICE),practice,$(APP_DIR))

CXX ?= g++
CC  ?= gcc
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

CPP_APPS := $(notdir $(wildcard $(APP_DIR)/cpp/*))
C_APPS   := $(notdir $(wildcard $(APP_DIR)/c/*))

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

.PHONY: help list list-c build build-c build-all run run-c clean delete-app app practice

help:
	@echo "Targets:"
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
	@echo "Variables: STD=c++98..c++23 (def c++23)  CSTD=c99|c11|c17"
	@echo "           BUILD_TYPE=Debug|Release  CXX=g++|clang++  CC=gcc|clang"
	@echo "           PRACTICE=1  build/run your scratch copy under practice/"

list:
	@printf "%s\n" $(CPP_APPS)

list-c:
	@printf "%s\n" $(C_APPS)

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

build-all:
	@for a in $(CPP_APPS); do $(MAKE) --no-print-directory build   app=$$a || exit 1; done
	@for a in $(C_APPS);   do $(MAKE) --no-print-directory build-c app=$$a || exit 1; done
	@echo "Built all lessons."

# --- single-lesson build (only when app= is set) ---
ifdef app
LDIR     := $(SRC_ROOT)/$(TREE)/$(app)
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
build: build-all
run:
	$(call require_app,run)
run-c:
	$(call require_app,run-c)
endif
