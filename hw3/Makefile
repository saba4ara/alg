.PHONY: all run valgrind clean tar
.SILENT: run

CC := gcc
CFLAGS += -Wall -pedantic -g -O2
SRCS := procdata.c
OBJS := procdata.o
BINS := procdata
ARGS := pub01

all: $(BINS)

$(BINS): $(OBJS)
	$(CC) $(CFLAGS) $< -o $@

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $< -c -o $@

run: $(BINS)
	./$(BINS) <data/$(ARGS).in

valgrind: $(BINS)
	valgrind ./$(BINS) <data/$(ARGS).in

clean:
	rm -rf $(OBJS) $(BINS) *.tar

tar:
	tar -cvf $(BINS).tar $(SRCS)

