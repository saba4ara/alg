.PHONY: all run valgrind clean
.SILENT: run

CC := gcc
FLAGS := -Wall -O2
SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=%.o)
BINS := $(SRCS:%.c=%)
ARGS := <data/pub01.in

all: $(BINS)

%: %.o
	$(CC) $(FLAGS) $< -o $@

%.o: %.c
	$(CC) $(FLAGS) -c -o $@ $<

run: $(BINS)
	./$(BINS) $(ARGS)

valgrind: $(BINS)
	valgrind ./$(BINS) $(ARGS)

clean:
	rm -rf $(OBJS) $(BINS)
