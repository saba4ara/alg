.PHONY: all run valgrind clean tar
.SILENT: run

CC := gcc
CFLAGS += -Wall -pedantic -g -O2
SRCS := procdata.c
OBJS := procdata.o
BINS := procdata
ARGS := data/pub01.in

all: $(BINS)

$(BINS): $(OBJS)
	$(CC) $(CFLAGS) $< -o $@

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) $< -c -o $@

run: $(BINS)
	./$(BINS) <$(ARGS)

valgrind: $(BINS)
	valgrind ./$(BINS) <$(ARGS)

clean:
	rm -rf $(OBJS) $(BINS) *.tar

tar:
	tar -cvf $(BINS).tar $(SRCS)

