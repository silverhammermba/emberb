VALUE result;

/* yield a value. To yield nothing, use Qundef */
result = rb_yield(Qundef);
/* yield several values */
result = rb_yield_values(3, Qtrue, Qfalse, Qnil);
/* splat a Ruby array and yield it */
result = rb_yield_splat(ary);
