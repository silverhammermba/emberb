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

		ruby_finalize(); // XXX you should probably guard against exceptions here!!!
	}

	return 0;
}
