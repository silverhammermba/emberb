#include <ruby/thread.h>

/* ... */

void unblocking_func(void* arg)
{
	/* somehow tell slow_func to return early */
}

VALUE my_method(VALUE self)
{
	/* ... */

	rb_thread_call_without_gvl(slow_func, slow_arg, unblocking_func, unblocking_arg);

	/* ... */
}
