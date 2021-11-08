SRC	       := $(wildcard src/*.c)
OBJS       := $(SRC:src/%.c=build/%.o)
UTIL       := $(wildcard util/*.c)
UTIL_OBJS  := $(UTIL:util/%.c=build/%.o)
EXEC       := silly
CFLAGS     := -std=c99 -Wall -Wextra -I include -I cake_libs/ -O0 -g \
              -Wno-unused-function -Wno-unused-parameter -Werror
CC	       ?= clang
TESTS      := $(wildcard test/*.c)
TESTS_OBJS := $(TESTS:test/%_test.c=build/%_test.o)

ifeq ($(ENV), dev)
	CFLAGS += -DENV_DEV
endif
ifeq ($(STAGE), preproc)
	CFLAGS += -E
endif

.PHONY: all util objs clean

all: objs $(EXEC)

objs: $(SRC) $(OBJS) util

util: $(UTIL_OBJS)

$(EXEC): $(OBJS) $(UTIL_OBJS)
	@mkdir -p bin
	$(CC) -o bin/$@ $^ -lxxhash

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $^ -o $@

build/%.o: test/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $^ -o $@

common_test: build/common_test.o build/silly_common.o
	cc -o bin/$@ $^ -lcunit

exec_test: build/exec_test.o build/exec.o
	cc -o bin/$@ $^ -lcunit

functable_test: test/functable_test.c
	cc $(CFLAGS) -o bin/$@ $^ -lcunit -lxxhash

clean:
	@for f in $$(find build -type f) $$(find bin -type f); do \
		if [ -f $$f ]; then \
			rm $$f ; \
		fi ; \
	done;
