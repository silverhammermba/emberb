VALUE foo_alloc(VALUE self)
{
	int* data;
	/* allocate and wrap. note that it needs the type to allocate */
	return Data_Make_Struct(self, int, NULL, RUBY_DEFAULT_FREE, data);
}
