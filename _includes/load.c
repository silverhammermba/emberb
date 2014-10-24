#include <ruby.h>
#include <stdio.h>

VALUE global;

int main(int argc, char* argv[])
{
	ruby_init();

	int status;

	printf("Testing R/W\n");
	/* $x can be changed freely in Ruby */
	rb_eval_string_protect("$x = true", &status);
	rb_define_variable("$x", &global);
	rb_eval_string_protect("p $x", &status);
	//Check_Type(global, T_TRUE);

	return ruby_cleanup(0);
}
