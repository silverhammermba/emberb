/* ... */

struct foo_data
{
	VALUE x;
};

void foo_mark(struct foo_data* data)
{
	rb_gc_mark(data->x);
}

VALUE foo_alloc(VALUE self)
{
	struct foo_data* data;
	/* wrap */
	return Data_Make_Struct(self, struct foo_data, foo_mark, RUBY_DEFAULT_FREE, data);
}

/* ... */
