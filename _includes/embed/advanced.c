#include <ruby.h>

int main(int argc, char* argv[])
{
	if (ruby_setup())
	{
		/* run code without Ruby */
	}
	else
	{
		/* Ruby goes here */

		ruby_finalize(); /* XXX rescue exceptions here!!! */
	}

	return 0;
}
