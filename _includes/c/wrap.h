#include <stdlib.h>

void foo_free(void* data)
{
	free(data);
}

size_t foo_size(const void* data)
{
	return sizeof(int);
}

static const rb_data_type_t foo_type = {
	.wrap_struct_name = "foo",
	.function = {
		.dmark = NULL,
		.dfree = foo_free,
		.dsize = foo_size,
	},
	.data = NULL,
	.flags = RUBY_TYPED_FREE_IMMEDIATELY,
};

VALUE foo_alloc(VALUE self)
{
	/* allocate */
	int* data = malloc(sizeof(int));

	/* wrap */
	return TypedData_Wrap_Struct(self, &foo_type, data);
}

VALUE foo_m_initialize(VALUE self, VALUE val)
{
	int* data;
	/* unwrap */
	TypedData_Get_Struct(self, int, &foo_type, data);

	*data = NUM2INT(val);

	return self;
}

void some_func()
{
	/* ... */

	VALUE cFoo = rb_define_class("Foo", rb_cObject);

	rb_define_alloc_func(cFoo, foo_alloc);
	rb_define_method(cFoo, "initialize", foo_m_initialize, 1);

	/* ... */
}
