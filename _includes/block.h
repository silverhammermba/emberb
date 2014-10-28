VALUE my_block(VALUE block_arg, VALUE data, int argc, VALUE* argv)
{
	/* block_arg will be the first yielded value */
	/* data will be the last argument you passed to rb_block_call */
	/* if multiple values are yielded, use argc/argv to access them */
}

void some_func()
{
	/* ... */

	VALUE obj;
	VALUE result;

	result = rb_block_call(obj, rb_intern("each"), 0, NULL, my_block, Qnil);

	/* ... */
}
