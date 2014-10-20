#!/usr/bin/env ruby
require 'mkmf'

# perform preparation for compiling here
# * generate source files
# * check features of current platform
# * check existence of required libraries
# * etc.

create_header
create_makefile 'foobar'
