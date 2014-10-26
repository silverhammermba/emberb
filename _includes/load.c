#include <ruby.h>
#include <stdio.h>

#include "method2.snip"

int main(int argc, char* argv[])
{
	ruby_init();

	rb_define_global_function("foo", my_method, -1);

	rb_eval_string("puts '2:'; foo 1, 2, a: 'a', b: 'b'");
	rb_eval_string("puts '3:'; foo 1, 2, 3, a: 'a', b: 'b'");
	rb_eval_string("puts '4:'; foo 1, 2, 3, 4, a: 'a', b: 'b'");
	rb_eval_string("puts '5:'; foo 1, 2, 3, 4, 5, a: 'a', b: 'b'");
	rb_eval_string("puts '6:'; foo 1, 2, 3, 4, 5, 6, a: 'a', b: 'b'");
	rb_eval_string("puts '7:'; foo 1, 2, 3, 4, 5, 6, 7, a: 'a', b: 'b'");
	rb_eval_string("puts '8:'; foo 1, 2, 3, 4, 5, 6, 7, 8, a: 'a', b: 'b'");

	return ruby_cleanup(0);
}
