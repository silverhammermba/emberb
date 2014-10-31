void foo_free(int* data)
{
	free(data);
}

VALUE foo_alloc(VALUE self)
{
	/* allocate */
	int* data = malloc(sizeof(int));

	/* wrap */
	return Data_Wrap_Struct(self, NULL, foo_free, data);
}

VALUE foo_m_initialize(VALUE self, VALUE val)
{
	int* data;
	/* unwrap */
	Data_Get_Struct(self, int, data);

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
