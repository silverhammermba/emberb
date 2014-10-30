CFLAGS=-std=c99 -Wall
BINS=$(patsubst %.c,%.out,$(wildcard *.c))

all: $(BINS)

%.out: %.c
	$(CC) $(CFLAGS) `pkg-config --cflags --libs ruby-2.1` -o $@ $<

ruby.h:
	echo '#include <ruby.h>' | $(CC) -E -P -dD `pkg-config --cflags ruby-2.1` -xc -o ruby.h -
