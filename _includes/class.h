VALUE cFoo;
VALUE cBar;

/* toplevel class Foo < Object */
cFoo = rb_define_class("Foo", rb_cObject);
/* nested class Foo::Bar < Array */
cBar = rb_define_class_under(cFoo, "Bar", rb_cArray);
