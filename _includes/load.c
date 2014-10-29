#include <ruby.h>
#include <ruby/thread.h>

VALUE my_thread(VALUE arg)
{
	rb_thread_sleep(5);
	return arg;
}

int main(int argc, char* argv[])
{
	ruby_init();

	VALUE x = INT2FIX(10);

	VALUE thread = rb_thread_create(my_thread, (void*)x);

	x = rb_funcall(thread, rb_intern("join"), 0);
	rb_funcall(rb_mKernel, rb_intern("p"), 1, x);

	return ruby_cleanup(0);
}
