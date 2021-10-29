SRC	       := $(wildcard src/*.c)
OBJS       := $(SRC:src/%.c=build/%.o)
UTIL       := $(wildcard util/*.c)
UTIL_OBJS  := $(UTIL:util/%.c=build/%.o)
EXEC       := silly
CFLAGS     := -std=c99 -Wall -Wextra -I include -I cake_libs/ -O0 -g \
              -Wno-unused-function -Wno-unused-parameter -lxxhash
CC	       ?= clang
TESTS      := $(wildcard test/*.c)
TESTS_OBJS := $(TESTS:test/%_test.c=build/%_test.o)

.PHONY: all util objs clean

all: objs $(EXEC)

objs: $(SRC) $(OBJS) util

util: $(UTIL_OBJS)

$(EXEC): $(OBJS) $(UTIL_OBJS)
	@mkdir -p bin
	$(CC) -o bin/$@ $^

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $^ -o $@

build/%.o: test/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $^ -o $@

common_test: build/common_test.o build/silly_common.o
	cc -o bin/$@ $^ -lcunit

bytecode_test: build/bytecode_test.o build/silly_bytecode.o\
		build/silly_common.o
	cc -o bin/$@ $^ -lcunit

functable_test: test/functable_test.c
	cc $(CFLAGS) -o bin/$@ $^ -lcunit

clean:
	@for f in $$(find build -type f) $$(find bin -type f); do \
		if [ -f $$f ]; then \
			rm $$f ; \
		fi ; \
	done;
