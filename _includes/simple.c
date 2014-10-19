#include <ruby.h>

int main(int argc, char** argv)
{
	ruby_init();

	/* Ruby goes here */

	return ruby_cleanup(0);
}
