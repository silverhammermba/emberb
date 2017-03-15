# this is just a hack to get the Ruby version in this guide
# you can just hard-code it e.g. RUBY=2.4
RUBY=$(shell grep rbversion ../../_config.yml | cut -d' ' -f2)

CFLAGS=-std=gnu11 -Wall $(shell pkg-config --cflags ruby-$(RUBY) sdl2)
LDLIBS=$(shell pkg-config --libs ruby-$(RUBY) sdl2)

all: tag

clean:
	rm -f tag *.o
