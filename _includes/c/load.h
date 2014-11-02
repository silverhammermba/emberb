VALUE script = rb_str_new_cstr("./foo.rb");

rb_load(script, 0);

/* or, handle exceptions like rb_eval_string_protect() does */
int state;
rb_load_protect(script, 0, &state);

if (state)
{
	/* got exception */
}
