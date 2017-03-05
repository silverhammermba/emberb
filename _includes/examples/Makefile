RUBY=2.4
CFLAGS=-std=gnu11 -Wall $(shell pkg-config --cflags ruby-$(RUBY) sdl2)
LDLIBS=$(shell pkg-config --libs ruby-$(RUBY) sdl2)

all: tag

clean:
	rm -f tag *.o
