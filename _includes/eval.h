int state;
VALUE result;
result = rb_eval_string_protect("puts 'Hello, world!'", &state);

if (state)
{
	/* handle exception */
}
