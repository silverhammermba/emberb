CFLAGS=-std=gnu99 -Wall $(shell pkg-config --cflags ruby-2.1 sdl2)
LIBS=$(shell pkg-config --libs ruby-2.1 sdl2)

tag: tag.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f tag *.o
