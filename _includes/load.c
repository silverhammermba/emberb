#include <ruby.h>
#include <stdio.h>

VALUE my_thread(VALUE i)
{
	rb_thread_sleep(2);
	printf("%d\n", FIX2INT(i));
}

int main(int argc, char* argv[])
{
	ruby_init();

	VALUE threads[10];

	for (int i = 0; i < 10; ++i)
		threads[i] = rb_thread_create(RUBY_METHOD_FUNC(my_thread), (void*)INT2FIX(i));

	for (int i = 0; i < 10; ++i)
		rb_funcall(threads[i], rb_intern("join"), 0);

	return ruby_cleanup(0);
}
