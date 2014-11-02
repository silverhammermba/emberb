#include <ruby.h>

void error(int state)
{
	if (!state) return;

	VALUE exception = rb_errinfo();
	rb_set_errinfo(Qnil);

	rb_warn("%"PRIsVALUE, exception);
}

int main(int argc, char* argv[])
{
	ruby_init();

	int state;

	rb_eval_string_protect("def x; 3; end", &state);
	error(state);

	rb_eval_string_wrap("puts x", &state);
	error(state);

	//rb_eval_string_protect("puts x", &state);
	//error(state);

	return ruby_cleanup(0);
}
