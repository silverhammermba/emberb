#include <ruby.h>
#include <stdio.h>

VALUE Foofoo(VALUE self, VALUE x)
{
	rb_eval_string_protect("puts 'Foo foo'", NULL);
	rb_funcall(rb_mKernel, rb_intern("p"), 1, x);
	return Qnil;
}

VALUE Barfoo(VALUE self, VALUE x)
{
	rb_eval_string_protect("puts 'Bar foo'", NULL);
	rb_funcall(rb_mKernel, rb_intern("p"), 1, x);
	rb_call_super(1, &x);
	return Qnil;
}

int main(int argc, char* argv[])
{
	ruby_init();

	VALUE cFoo = rb_define_class("Foo", rb_cObject);
	rb_define_method(cFoo, "foo", Foofoo, 1);

	VALUE cBar = rb_define_class("Bar", cFoo);
	rb_define_method(cBar, "foo", Barfoo, 1);

	VALUE bar = rb_funcall(cBar, rb_intern("new"), 0);
	rb_funcall(bar, rb_intern("foo"), 1, INT2FIX(42));

	return ruby_cleanup(0);
}
