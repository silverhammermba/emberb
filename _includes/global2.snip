VALUE global;

VALUE global_getter(ID id)
{
	/* return some VALUE, probably based on global */
}

void global_setter(VALUE val, ID id)
{
	/* set global, probably based on val */
}

void some_func()
{
	/* ... */

	/* initialize global first! */

	/* $w can be changed freely in Ruby */
	rb_define_variable("$w", &global);

	/* assigning a new value to $x in Ruby will raise a NameError */
	rb_define_readonly_variable("$x", &global);

	/* $y can be changed freely in Ruby, but through the specified functions */
	rb_define_hooked_variable("$y", &global, global_getter, global_setter);

	/* same as previous, but there's no corresponding VALUE! */
	rb_define_virtual_variable("$z", global_getter, global_setter)

	/* ... */
}
