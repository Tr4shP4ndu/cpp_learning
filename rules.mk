# =============================================================
# rules.mk — the ONE shared build recipe, included by every app's Makefile.
#
# An app's Makefile is just two lines:
#     APP := my-app
#     include <path-to>/rules.mk
#
# From here we:
#   1. pick the compiler — C++ if the app has src/*.cpp, otherwise C;
#   2. prefer the repo-local ./toolchain if `make install` set one up;
#   3. apply warning / optimization / sanitizer flags;
#   4. build src/ (+ optional include/) into ./build/<APP>.
#
# Everything is overridable on the command line, e.g.
#     make run STD=c++98            # try an older C++ standard
#     make build BUILD_TYPE=Release # optimized, no sanitizers
# =============================================================

# The repo root is wherever THIS file lives — found no matter how deep the app
# folder is nested (apps/cpp/foo or apps/cpp/graphics/foo both work).
REPO := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

# --- compiler & language standard (override on the command line) -------------
CXX  ?= c++
CC   ?= cc
STD  ?= c++23
CSTD ?= c17
BUILD_TYPE ?= Debug

# Prefer ./toolchain/bin/clang from `make install`, unless the user named a
# compiler explicitly. (CXX/CC are built-in make variables, so a plain `?=`
# can't override them — we test $(origin ...) instead.)
ifneq ($(wildcard $(REPO)/toolchain/bin/clang++),)
  ifeq ($(origin CXX),default)
    CXX := $(REPO)/toolchain/bin/clang++
  endif
  ifeq ($(origin CC),default)
    CC := $(REPO)/toolchain/bin/clang
  endif
endif

# --- flags -------------------------------------------------------------------
WARN := -Wall -Wextra -Wpedantic
ifeq ($(BUILD_TYPE),Release)
  OPT := -O2 -DNDEBUG                            # optimized; asserts compiled out
else
  OPT := -O0 -g -fsanitize=address,undefined     # debuggable; catches memory/UB bugs
endif

# --- sources: C++ if there are .cpp files, otherwise C -----------------------
CPP_SRCS := $(wildcard src/*.cpp)
C_SRCS   := $(wildcard src/*.c)
ifneq ($(CPP_SRCS),)
  SRCS     := $(CPP_SRCS)
  COMPILER := $(CXX)
  STDFLAG  := -std=$(STD)
else
  SRCS     := $(C_SRCS)
  COMPILER := $(CC)
  STDFLAG  := -std=$(CSTD)
endif

HDRS := $(wildcard include/*.h include/*.hpp src/*.h src/*.hpp)
INC  := $(addprefix -I,$(wildcard include)) -Isrc
BIN  := build/$(APP)

.PHONY: build run clean
build: $(BIN)

# One command compiles and links every source. Rebuilds if any source OR header
# changes. Simple on purpose — these apps are small.
$(BIN): $(SRCS) $(HDRS)
	@mkdir -p build
	$(COMPILER) $(STDFLAG) $(WARN) $(OPT) $(INC) $(SRCS) -o $(BIN)

run: build
	@./$(BIN) $(ARGS)

clean:
	@rm -rf build
