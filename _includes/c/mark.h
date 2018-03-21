/* ... */

struct foo_data
{
	VALUE x;
};

void foo_mark(void* data)
{
	rb_gc_mark(((struct foo_data*)data)->x);
}

static const rb_data_type_t foo_type = {
	/* ... */
	.function = {
		.dmark = foo_mark,
		/* ... */
	},
	/* ... */
};


VALUE foo_alloc(VALUE self)
{
	struct foo_data* data;
	/* wrap */
	return TypedData_Make_Struct(self, struct foo_data, &foo_type, data);
}

/* ... */
