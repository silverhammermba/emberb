CFLAGS=-std=c99 -Wall -ggdb -pg $(shell pkg-config --cflags ruby-2.2)
BINS=$(patsubst %.c,%.out,$(wildcard *.c))
LIBS=$(shell pkg-config --libs ruby-2.2)

all: $(BINS)

%.out: %.c
	$(CC) $(CFLAGS)  -o $@ $< $(LIBS)

ruby.h:
	echo '#include <ruby.h>' | $(CC) -E -P -dD `pkg-config --cflags ruby-2.2` -xc -o ruby.h -
