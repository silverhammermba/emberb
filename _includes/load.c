#include <ruby.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	ruby_init();

	rb_define_global_const("UNDEFINED", Qundef);

	int status;

	rb_eval_string_protect("p UNDEFINED", &status);

	return ruby_cleanup(status);
}
