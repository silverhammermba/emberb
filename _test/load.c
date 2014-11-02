#include <ruby.h>

int main(int argc, char* argv[])
{
	ruby_init();

	void* node = rb_load_file("-");

	int state = ruby_exec_node(node);

	return ruby_cleanup(state);
}
