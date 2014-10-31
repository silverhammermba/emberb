#include <ruby.h>
#include <ruby/thread.h>
#include <unistd.h>

void* concurrent_func(void* arg)
{
	unsigned int* x = arg;

	for (unsigned int i = 0; i < *x; ++i)
		sleep(1);

	*x *= 2;

	return NULL;
}

void unblock_func(void* arg)
{
	unsigned int* x = arg;
	*x = 0;
}

VALUE foo(VALUE self, VALUE x)
{
	Check_Type(x, T_FIXNUM);

	unsigned int y = NUM2UINT(x);

	//concurrent_func((void*)&y);
	rb_thread_call_without_gvl(concurrent_func, &y, unblock_func, &y);

	return LONG2FIX(y);
}

int main(int argc, char* argv[])
{
	ruby_init();

	rb_define_global_function("foo", foo, 1);

	int state;
	rb_eval_string_protect("threads = Array.new(3) { Thread.new { foo 10 } }; threads.map(&:kill)", &state);

	return ruby_cleanup(state);
}
