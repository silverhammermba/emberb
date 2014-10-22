CFLAGS=-std=c99
BINS=$(patsubst %.c,%.out,$(wildcard *.c))

all: $(BINS)

%.out: %.c
	$(CC) $(CFLAGS) `pkg-config --cflags --libs ruby-2.1` -o $@ $<
