#include <ruby.h>
#include <stdio.h>

VALUE dangerous_func(VALUE obj)
{
	rb_require("blah");
	return Qtrue;
}

VALUE rescue_func(VALUE obj)
{

	return Qfalse;
}

int main(int argc, char* argv[])
{
	ruby_init();

	VALUE result;

	int status;
	result = rb_protect(dangerous_func, Qtrue, &status);

	if (status)
	{
		VALUE exception = rb_errinfo();
		rb_funcall(rb_mKernel, rb_intern("puts"), 1, exception);
	}

	return ruby_cleanup(0);
}
