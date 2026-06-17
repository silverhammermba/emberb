VALUE result;

/* yield a value. To yield nothing, use RUBY_Qundef */
result = rb_yield(RUBY_Qundef);
/* yield several values */
result = rb_yield_values(3, RUBY_Qtrue, RUBY_Qfalse, RUBY_Qnil);
/* splat a Ruby array and yield it */
result = rb_yield_splat(ary);
