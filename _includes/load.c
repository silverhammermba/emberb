#include <ruby.h>
#include <ruby/thread.h>
#include <unistd.h>

void* sleeper(void* arg)
{
	sleep(5);
	return NULL;
}

void unblock(void* arg)
{
	printf("Interrupted\n");
}

VALUE foo(VALUE self)
{
	rb_thread_call_without_gvl2(sleeper, NULL, unblock, NULL);

	return Qnil;
}

int main(int argc, char* argv[])
{
	ruby_init();

	rb_define_global_function("foo", foo, 1);

	int state;
	rb_eval_string_protect("threads = Array.new(3) { Thread.new { foo 10 } }; threads.map(&:join)", &state);

	return ruby_cleanup(state);
}
