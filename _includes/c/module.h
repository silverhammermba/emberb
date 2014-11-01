VALUE mFoo;
VALUE mBar;

/* toplevel module Foo */
mFoo = rb_define_module("Foo");
/* nested module Foo::Bar */
mBar = rb_define_module_under(mFoo, "Bar");
