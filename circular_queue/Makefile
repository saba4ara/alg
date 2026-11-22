.PHONY: all run valgrind clean tar
.SILENT: run

CC := gcc
CFLAGS += -Wall -Wextra -Wconversion -fsanitize=address,undefined -pedantic -g -O2
SRCS := circular_q.c
OBJS := circular_q.o
BINS := circular_q

all: $(BINS)

$(BINS): $(OBJS)
	$(CC) $(CFLAGS) $< -o $@

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $< -c -o $@

run: $(BINS)
	./$(BINS)

valgrind: $(BINS)
	valgrind ./$(BINS)

clean:
	rm -rf $(OBJS) $(BINS)

