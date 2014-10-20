#include <ruby.h>

int main(int argc, char* argv[])
{
	ruby_init();

#include "eval.snip"

	return ruby_cleanup(0);
}
