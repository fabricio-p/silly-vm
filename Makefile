SRC	  := $(wildcard src/*.c)
HEAD	  := $(wildcard src/*.h)
OBJS	  := $(SRC:.c=.o)
UTIL	  := $(wildcard util/*.c)
UTIL_OBJS := $(UTIL:.c=.o)
EXEC	  := silly
CFLAGS	  := -Wall -Wextra -std=c99 -I src -I util
CC	  ?= clang

.PHONY: all util clean

all: $(SRC) $(HEAD) $(OBJS) $(UTIL) $(UTIL_OBJS) $(EXEC)

$(EXEC): $(OBJS) $(UTIL_OBJS)
	$(CC) -o $@ $^

util/%.o: util/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

util: $(UTIL_OBJS)

%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	@for f in $(OBJS) $(UTIL_OBJS) $(EXEC); do \
		if [ -f $$f ]; then \
			rm $$f ; \
		fi ; \
	done;
