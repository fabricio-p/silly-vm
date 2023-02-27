SRC				 := $(wildcard src/*.c)
OBJS			 := $(SRC:src/%.c=build/obj/%.o)
EXEC			 := silly
CFLAGS			+= -std=c99 -I . -I cake_libs/ -I include/ 									\
						-Wall -Wextra -Wformat=2 -Wshadow														\
						-Wwrite-strings -Wstrict-prototypes -Wold-style-definition	\
						-Wredundant-decls -Wnested-externs -Wmissing-include-dirs		\
						-Wno-unused-parameter -Wno-unused-command-line-argument			\
						-Wno-missing-braces -Wno-unused-function # -ferror-limit=8
LDFLAGS		 := -lm
CC				 := clang
TESTS			:= $(wildcard test/*.c)
TESTS_OBJS := $(TESTS:test/%_test.c=build/obj/%_test.o)

ifeq ($(DEBUG), true)
	CFLAGS += -DDEBUG -O0 -ggdb
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
	@mkdir -p build/bin
	$(CC) $(LDFLAGS) -o build/bin/$@ $^

build/obj/%.o: src/%.c
	@mkdir -p build/obj
	$(CC) $(CFLAGS) -c $^ -o $@

build/obj/%.o: test/%.c
	@mkdir -p build/obj
	$(CC) $(CFLAGS) -c $^ -o $@

tests: $(TESTS:test/%.c=%)

run_tests: $(TESTS:test/%.c=%)
	for test in $^; do								\
		echo "Running test $$test...";	\
		./bin/$$test b;									\
		echo "...Finished test $$test";	\
	done

exec_test: build/obj/exec_test.o build/obj/exec.o
	@mkdir -p build/bin
	$(CC) $(LDFLAGS) -o build/bin/$@ $^ -lcunit

parser_test: build/obj/parser.o build/obj/parser_test.o
	@mkdir -p build/bin
	$(CC) $(LDFLAGS) -o build/bin/$@ $^ -lcunit

clean:
	@if [ -d build ]; then rm -rf build; fi
