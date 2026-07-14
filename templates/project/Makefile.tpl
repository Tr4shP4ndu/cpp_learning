# __PROJECT_NAME__ — your own project. THIS Makefile owns the build; the
# workspace just hands off to it:
#   make project       name=__PROJECT_NAME__   # build + run
#   make project-build name=__PROJECT_NAME__   # build only
#
# The workspace passes CXX / CC / STD / CSTD / BUILD_TYPE down as overridable
# defaults (and ARGS for `run`). Use them, ignore them, or add whatever you
# need — this is your build. Add as many folders (src/, include/, assets/, …)
# as you like; just make `build` and `run` do the right thing.
CXX ?= c++
STD ?= c++23

BIN  := build/__PROJECT_NAME__
SRCS := $(wildcard src/*.cpp)

.PHONY: build run clean
build: $(BIN)

$(BIN): $(SRCS)
	@mkdir -p build
	$(CXX) -std=$(STD) -Wall -Wextra -Iinclude -Isrc $(SRCS) -o $(BIN)

run: build
	@./$(BIN) $(ARGS)

clean:
	@rm -rf build
