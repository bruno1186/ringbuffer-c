CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -O2 -Iinclude

SRC = src/ringbuffer.c
TEST_SRC = tests/test_ringbuffer.c
BIN = build/test_ringbuffer

.PHONY: all test clean

all: test

$(BIN): $(SRC) $(TEST_SRC) include/ringbuffer.h
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) $(TEST_SRC) -o $(BIN)

test: $(BIN)
	./$(BIN)

clean:
	rm -rf build
