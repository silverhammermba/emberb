#include <ruby.h>

VALUE func(VALUE arg)
{
	VALUE str = rb_sprintf("%i %+i : %d %+d", arg, arg, arg, arg);
	rb_funcall(rb_mKernel, rb_intern("puts"), 1, str);
	rb_warn("%i %+i : %d %+d", arg, arg, arg, arg);
	rb_raise(rb_eRuntimeError, "%i %+i : %d %+d", arg, arg, arg, arg);
}

int main(int argc, char* argv[])
{
	ruby_init();

	int state;
	VALUE x = INT2FIX(314);
	rb_protect(func, x, &state);

	return ruby_cleanup(state);
}
