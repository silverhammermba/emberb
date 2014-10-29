#include <ruby.h>
#include <limits.h>

int main(int argc, char* argv[])
{
	ruby_init();

	VALUE y = INT2FIX(INT_MAX);
	char z = NUM2CHR(y);

	printf("%hhd\n", z);

	return ruby_cleanup(0);
}
