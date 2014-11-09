#include <ruby.h>

int main(int argc, char* argv[])
{
	ruby_init();

	VALUE cFoo = rb_define_class("Foo", rb_cArray);
	VALUE x = rb_funcall(cFoo, rb_intern("new"), 0);

	if (RB_TYPE_P(x, T_ARRAY))
		printf("true\n");
	else
		printf("false\n");

	return ruby_cleanup(0);
}
