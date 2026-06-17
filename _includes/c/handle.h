VALUE exception = rb_errinfo(); /* get last exception */
rb_set_errinfo(RUBY_Qnil);      /* clear last exception */
