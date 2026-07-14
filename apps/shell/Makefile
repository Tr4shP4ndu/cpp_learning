# shell — self-owned build (a C project). From the repo root: make project name=shell
CC   ?= cc
CSTD ?= c17

BIN  := build/shell
SRCS := $(wildcard src/*.c)

.PHONY: build run clean
build: $(BIN)

$(BIN): $(SRCS)
	@mkdir -p build
	$(CC) -std=$(CSTD) -Wall -Wextra $(SRCS) -o $(BIN)

run: build
	@./$(BIN) $(ARGS)

clean:
	@rm -rf build
