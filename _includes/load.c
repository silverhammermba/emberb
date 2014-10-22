#include <ruby.h>
#include <stdio.h>

int inspect(VALUE obj)
{
	int status;
	rb_gv_set("$xxx_do_not_use", obj);
	rb_eval_string_protect("puts $xxx_do_not_use.inspect", &status);

	return status;
}

int main(int argc, char* argv[])
{
	ruby_init();

	VALUE obj = Qnil;

#include "raise.snip"

	return ruby_cleanup(0);
}
