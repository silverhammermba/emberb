#include <ruby.h>

int main(int argc, char* argv[])
{
	ruby_init();

	VALUE cFoo = rb_define_class("Foo", rb_cObject);
	rb_cv_set(cFoo, "@@x", INT2FIX(3));

	VALUE foo = rb_funcall(cFoo, rb_intern("new"), 0);
	VALUE x = rb_cv_get(cFoo, "@@x");

	rb_warn("%+"PRIsVALUE, x);

	return ruby_cleanup(0);
}
