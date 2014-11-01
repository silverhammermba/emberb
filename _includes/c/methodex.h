VALUE my_method(int argc, VALUE* argv, VALUE self)
{
	/*
	 * We want to define a method like
	 *
	 *     def my_method man1, opt1 = true, opt2 = false, *splat, man2, opts = {}, &blk
	 *
	 * Interestingly, that gives you a SyntaxError in Ruby. But the API can do it :D
	*/

	VALUE man1, man2;
	VALUE opt1, opt2;
	VALUE splat;
	VALUE opts;
	VALUE blk;

	rb_scan_args(argc, argv, "12*1:&", &man1, &opt1, &opt2, &splat, &man2, &opts, &blk);

	// set default values for optional args that weren't passed
	if (NIL_P(opt1)) opt1 = Qtrue;
	if (NIL_P(opt2)) opt2 = Qfalse;
	if (NIL_P(opts)) opts = rb_hash_new();

	/* ... */
}
