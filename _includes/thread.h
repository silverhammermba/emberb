VALUE my_thread(VALUE arg)
{
	/* ... */
}

void some_func()
{
	/* ... */

	VALUE thread;

	thread = rb_thread_create(my_thread, arg);

	/* ... */
}
