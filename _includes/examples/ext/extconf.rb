#!/usr/bin/env ruby
require 'mkmf'

raise "Can't find GMP lib"       unless have_library 'gmp'
raise "Can't find GMP header"    unless have_header  'gmp.h'
raise "Can't find string header" unless have_header  'string.h'

create_makefile 'gmp'
