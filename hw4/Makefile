.PHONY: all run san clean tar
.SILENT: run

CC := gcc
CFLAGS += -Wall -Wextra -Wconversion -pedantic -g -O2
SANFLAGS += -fsanitize=address,undefined 

SRCS := $(wildcard *.c)
HEADS := $(wildcard *.h)
OBJS := $(SRCS:.c=.o)

BIN := rescue
SAN_BIN := $(BIN)_san
ARGS := pub01

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJS): %.o: %.c $(HEADS)
	$(CC) $(CFLAGS) -c $< -o $@

$(SAN_BIN): $(SRCS) $(HEADS)
	$(CC) $(CFLAGS) $(SANFLAGS) $(SRCS) -o $@

run: $(BIN)
	./$(BIN) <data/$(ARGS).in

san: $(SAN_BIN)
	./$(SAN_BIN) <data/$(ARGS).in

clean:
	rm -rf $(OBJS) $(BIN) $(SAN_BIN) $(BIN).tar

tar:
	tar -cvf $(BIN).tar $(SRCS) $(HEADS)

