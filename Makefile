CC=gcc
DEPS= $(SRC) $(INCLUDE)
CFLAGS=-Wall -Wextra -Werror
DFLAGS=-g -fsanitize=address

micro_paint: src/micro_paint/micro_paint.c
	$(CC) $(CFLAGS) $(DFLAGS) $^ -o $@

mini_paint: src/mini_paint/mini_paint.c
	$(CC) $(CFLAGS) $(DFLAGS) $^ -o $@
