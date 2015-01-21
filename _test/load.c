#include <ruby.h>
#include <stdio.h>

VALUE test(int argc, VALUE* argv, VALUE self)
{
	VALUE man1, man2;
	VALUE opt1, opt2;
	VALUE splat;
	VALUE opts;
	VALUE blk;

	rb_scan_args(argc, argv, "12*1:&", &man1, &opt1, &opt2, &splat, &man2, &opts, &blk);

	VALUE str;
	str = rb_sprintf("%+"PRIsVALUE" %+"PRIsVALUE" %+"PRIsVALUE" %+"PRIsVALUE" %+"PRIsVALUE, opt1, opt2, splat, opts, blk);
	printf("%s\n", StringValueCStr(str));

	return Qnil;
}

int main(int argc, char* argv[])
{
	ruby_init();

	rb_define_global_function("test", test, -1);
	int state;
	rb_eval_string_protect("test 1, 2", &state);
	rb_eval_string_protect("test 1, 2, 3", &state);
	rb_eval_string_protect("test 1, 2, 3, 4", &state);
	rb_eval_string_protect("test 1, 2, 3, 4, 5", &state);
	rb_eval_string_protect("test 1, 2, 3, 4, 5, 6", &state);
	rb_eval_string_protect("test 1, 2, 3, 4, 5, 6, a: 3", &state);

	return ruby_cleanup(state);
}
