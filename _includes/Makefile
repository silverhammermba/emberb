CFLAGS=-I/usr/include/ruby-2.1.0 -I/usr/include/ruby-2.1.0/x86_64-linux
BINS=$(patsubst %.c,%.out,$(wildcard *.c))

all: $(BINS)

%.out: %.c
	$(CC) $(CFLAGS) -o $@ $< -lruby
