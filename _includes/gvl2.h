#include <ruby/thread.h>

void* slow_func(void* slow_arg)
{
	/* slow code that DOES NOT USE THE API */
}

VALUE my_method(VALUE self)
{
	/* arg parsing, API stuff, etc. */

	rb_thread_call_without_gvl(slow_func, (void*)slow_arg, NULL, NULL);

	/* more API stuff. probably turn the result of slow_func into a VALUE */
}
