.PHONY: all run valgrind clean tar
.SILENT: run

CC := gcc
CFLAGS += -Wall -pedantic -g -O2
SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=%.o)
BINS := $(SRCS:%.c=%)
ARGS := data/pub08.in

all: $(BINS)

$(BINS): $(OBJS)
	$(CC) $(CFLAGS) $< -o $@

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $< -c -o $@

run: $(BINS)
	./$(BINS) <$(ARGS) && cat $(ARGS)

valgrind: $(BINS)
	valgrind ./$(BINS) <$(ARGS)

clean:
	rm -rf $(OBJS) $(BINS) *.tar

tar:
	tar -cvf $(BINS).tar $(SRCS)
