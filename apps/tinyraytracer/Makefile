# tinyraytracer — self-owned build. From the repo root: make project name=tinyraytracer
# -O2 because tracing ~800k rays with recursion is compute-heavy.
CXX ?= c++
STD ?= c++23

BIN  := build/tinyraytracer
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
