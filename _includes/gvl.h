#include <ruby/thread.h>

/* ... */

void unblocking_func(void* arg)
{
	/* signal slow_func to return early */
}

VALUE my_method(VALUE self)
{
	/* ... */

	rb_thread_call_without_gvl(slow_func, (void*)slow_arg, unblocking_func, (void*)unblocking_arg);

	/* ... */
}
