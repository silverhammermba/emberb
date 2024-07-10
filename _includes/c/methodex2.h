VALUE my_method(int argc, VALUE* argv, VALUE self)
{
	/*
	 * We want to define a method like
	 *
	 *     def my_method kw:, optkw: nil, **opts
	 */

	static ID keyword_ids[2];
	VALUE kwargs[2];
	VALUE kw, optkw;
	VALUE opts;

	if (!keyword_ids[0]) {
		keyword_ids[0] = rb_intern("kw");
		keyword_ids[1] = rb_intern("optkw");
	}

	rb_scan_args(argc, argv, ":", &opts);
	rb_get_kwargs(opts, keyword_ids, 1, -2, kwargs);
	kw = kwargs[0];
	optkw = kwargs[1];

	/* omitted optional keyword arguments are set to Qundef */
	if (optkw == Qundef) optkw = Qnil;

	/* ... */
}
