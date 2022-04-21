SRC	       := $(wildcard src/*.c)
OBJS       := $(SRC:src/%.c=build/%.o)
UTIL       := $(wildcard util/*.c)
UTIL_OBJS  := $(UTIL:util/%.c=build/%.o)
EXEC       := silly
CFLAGS     := -std=c99 -Wall -Wextra -I include -I cake_libs/ -O0 -g \
              -Wno-unused-function -Wno-unused-parameter -Werror
CC	       ?= gcc
TESTS      := $(wildcard test/*.c)
TESTS_OBJS := $(TESTS:test/%_test.c=build/%_test.o)

ifeq ($(DEBUG), true)
	CFLAGS += -DDEBUG
endif
ifeq ($(ENV), dev)
	CFLAGS += -DENV_DEV
endif
ifeq ($(STAGE), preproc)
	CFLAGS += -E
endif
ifeq ($(LOGGING), colored)
	CFLAGS += -DOUTCOLORS
endif

.PHONY: all util objs tests exec_test parser_test test clean

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

tests: $(TESTS:test/%.c=%)

run_tests: $(TESTS:test/%.c=%)
	for test in $^; do \
		echo "Running test $$test..."; \
		./bin/$$test b; \
		echo "...Finished test $$test"; \
	done

exec_test: build/exec_test.o build/exec.o
	@mkdir -p bin
	cc -o bin/$@ $^ -lcunit

parser_test: build/parser.o build/parser_test.o
	@mkdir -p bin
	cc -o bin/$@ $^ -lcunit

clean:
	@if [ -d build ]; then rm -rf build; fi
	@if [ -d bin ]; then rm -rf bin; fi
