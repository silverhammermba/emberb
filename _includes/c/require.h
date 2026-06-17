rb_require("foo");

/* or, using a Ruby String (first argument is ignored) */
rb_f_require(RUBY_Qnil, rb_str_new_cstr("foo"));
