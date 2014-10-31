#include <ruby.h>

struct foo
{
	VALUE x;
	int num_vals;
	VALUE* vals;
};

void foo_mark(struct foo* data)
{
	rb_gc_mark(data->x);
	rb_gc_mark_locations(data->vals, data->vals + data->num_vals);
}

void foo_free(struct foo* data)
{
	free(data->vals);
	free(data);
}

VALUE foo_m_puts(VALUE self)
{
	struct foo* data;
	Data_Get_Struct(self, struct foo, data);

	rb_funcall(rb_mKernel, rb_intern("puts"), 1, data->x);
	rb_funcallv(rb_mKernel, rb_intern("puts"), data->num_vals, data->vals);

	return Qnil;
}

VALUE foo_m_initialize(VALUE self, VALUE val)
{
	struct foo* data;
	Data_Get_Struct(self, struct foo, data);

	for (int i = 0; i < data->num_vals; ++i)
	{
		data->vals[i] = (i % 2 == 0 ? Qtrue : Qfalse);
	}

	data->x = val;

	return self;
}

VALUE foo_alloc(VALUE self)
{
	struct foo* data = malloc(sizeof(struct foo));
	data->num_vals = 5;
	data->vals = calloc(5, sizeof(VALUE));

	return Data_Wrap_Struct(self, foo_mark, foo_free, data);
}

int main(int argc, char* argv[])
{
	ruby_init();

	VALUE cFoo = rb_define_class("Foo", rb_cObject);

	rb_define_alloc_func(cFoo, foo_alloc);
	rb_define_method(cFoo, "initialize", foo_m_initialize, 1);
	rb_define_method(cFoo, "show", foo_m_puts, 0);

	int state;
	rb_eval_string_protect("f = Foo.new(1234321); f.show", &state);

	return ruby_cleanup(state);
}
