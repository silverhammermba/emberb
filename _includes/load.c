#include <ruby.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	ruby_init();

	VALUE cFoo = rb_define_class("Foo", rb_cObject);
	VALUE foo = rb_funcall(cFoo, rb_intern("new"), 0);

	rb_iv_set(foo, "@x", Qtrue);
	Check_Type(rb_iv_get(foo, "@x"), T_TRUE);

	return ruby_cleanup(0);
}
