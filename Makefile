CC=clang
CFLAGS=-fPIC -Wall -Wextra -Wpedantic -Wshadow -Werror -std=c23 -O2 -g
LDFLAGS=-shared -pthread

SOURCES=$(wildcard *.c)
PATCHES=$(SOURCES:.c=.so)

all: $(PATCHES)

%.so: %.c util.h
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f *.so

.PHONY: all clean
