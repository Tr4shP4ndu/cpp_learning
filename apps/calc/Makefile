# calc — self-owned build. From the repo root: make project name=calc
CXX ?= c++
STD ?= c++23

BIN  := build/calc
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
