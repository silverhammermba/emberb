#include <ruby.h>

int main(int argc, char* argv[])
{
	ruby_init();

	char* options[] = { "-v", "-eputs 'Hello, world!'" };
	void* node = ruby_options(2, options);

	int state;
	if (ruby_executable_node(node, &state))
	{
		state = ruby_exec_node(node);
	}
	else
	{
		// code didn't compile
	}

	return ruby_cleanup(state);
}
