VALUE x;
x = rb_str_new_cstr("Hello, world!");

VALUE str;
str = rb_sprintf("pi = %f. %"PRIsVALUE" inspected: %+"PRIsVALUE, M_PI, x, x);

/* pi = 3.141593. Hello, world! inspected: "Hello, world!" */
