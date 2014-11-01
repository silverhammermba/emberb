#include <ruby.h>

VALUE func(VALUE arg)
{
VALUE x;
x = rb_str_new_cstr("Hello, world!");

VALUE str;
str = rb_sprintf("pi = %f. %"PRIsVALUE" inspected: %+"PRIsVALUE, M_PI, x, x);
	rb_funcall(rb_mKernel, rb_intern("puts"), 1, str);
	rb_warn("%"PRIsVALUE" %+"PRIsVALUE" : %ld %+ld", arg, arg, arg, arg);
	rb_raise(rb_eRuntimeError, "%"PRIsVALUE" %+"PRIsVALUE" : %ld %+ld", arg, arg, arg, arg);
}

int main(int argc, char* argv[])
{
	ruby_init();

	int state;
	VALUE x = rb_eval_string_protect("\"hey\"", &state);
	rb_protect(func, x, &state);

	return ruby_cleanup(state);
}
