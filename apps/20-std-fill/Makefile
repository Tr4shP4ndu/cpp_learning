# Per-app Makefile — one lesson, one command.
APP_NAME := 20-std-fill
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
