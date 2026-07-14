# tinykaboom — self-owned build. From the repo root: make project name=tinykaboom
CXX ?= c++
STD ?= c++23

BIN  := build/tinykaboom
SRCS := $(wildcard src/*.cpp)

.PHONY: build run clean
build: $(BIN)

$(BIN): $(SRCS)
	@mkdir -p build
	$(CXX) -std=$(STD) -O2 -Wall -Wextra -Iinclude -Isrc $(SRCS) -o $(BIN)

run: build
	@./$(BIN) $(ARGS)

clean:
	@rm -rf build out.ppm
