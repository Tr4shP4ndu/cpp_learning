APP_NAME := 18-void-functions
SRC_DIR  := src
## Build outside app dir under workspace 'build/<app>'
BUILD_ROOT ?= $(abspath $(CURDIR)/../../build)
BUILD_DIR  := $(BUILD_ROOT)/$(APP_NAME)
BIN_DIR    := $(BUILD_DIR)/bin

CXX ?= g++
CXXFLAGS ?= -std=c++20 -O2 -Wall -Wextra -Wpedantic
LDFLAGS ?=

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
BIN  := $(BIN_DIR)/$(APP_NAME)

.PHONY: all build run clean

all: build

$(BUILD_DIR):
	@mkdir -p "$(BUILD_DIR)" "$(BIN_DIR)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build: $(OBJS)
	$(CXX) $(OBJS) -o "$(BIN)" $(LDFLAGS)

run: build
ifeq ($(OS),Windows_NT)
	@"$(BIN).exe"
else
	@"$(BIN)"
endif

clean:
	rm -rf "$(BUILD_DIR)"
