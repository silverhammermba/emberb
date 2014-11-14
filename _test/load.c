#include <ruby.h>

VALUE test(VALUE x)
{
	VALUE y = rb_str_new_cstr("foo");
	char z = NUM2CHR(y);
	printf("%hhd\n", z);
	return Qundef;
}

int main(int argc, char* argv[])
{
	ruby_init();

	int state;
	rb_protect(test, Qundef, &state);

	return ruby_cleanup(state);
}
