#include <ruby.h>
#include <stdlib.h>

VALUE foo_m_to_i(VALUE self)
{
	int* data;
	Data_Get_Struct(self, int, data);

	return INT2FIX(*data);
}

VALUE foo_m_initialize(VALUE self, VALUE val)
{
	int* data;
	Data_Get_Struct(self, int, data);

	*data = NUM2INT(val);

	return self;
}

VALUE foo_alloc(VALUE self)
{
	int* data;
	return Data_Make_Struct(self, int, NULL, -1, data);
}

int main(int argc, char* argv[])
{
	ruby_init();

	VALUE cFoo = rb_define_class("Foo", rb_cObject);

	rb_define_alloc_func(cFoo, foo_alloc);
	rb_define_method(cFoo, "initialize", foo_m_initialize, 1);
	rb_define_method(cFoo, "to_i", foo_m_to_i, 0);

	int state;
	rb_eval_string_protect("f = Foo.new(1234321); puts f.to_i", &state);

	return ruby_cleanup(state);
}
