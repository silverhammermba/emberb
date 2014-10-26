#include <ruby.h>
#include <stdio.h>

VALUE foo(VALUE self)
{
	rb_funcall(rb_mKernel, rb_intern("p"), 1, self);
}

int main(int argc, char* argv[])
{
	ruby_init();

	rb_define_global_function("foo", foo, 0);

	rb_eval_string("foo");

	return ruby_cleanup(0);
}
