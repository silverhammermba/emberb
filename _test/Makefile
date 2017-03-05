RUBY=2.4
CFLAGS=-std=c11 -Wall -ggdb -pg $(shell pkg-config --cflags ruby-$(RUBY))
LDLIBS=$(shell pkg-config --libs ruby-$(RUBY))

BINS=$(basename $(wildcard *.c))

.PHONY: ruby.h

all: $(BINS)

# if you want to see all the stuff that including ruby.h gives you
ruby.h:
	echo '#include <ruby.h>' | $(CC) -E -P -dD $(CFLAGS) -xc -o $@ -
