BINS=$(patsubst %.c,%.out,$(wildcard *.c))

all: $(BINS)

%.out: %.c
	$(CC) `pkg-config --cflags --libs ruby-2.1` -o $@ $<
