const char *msg = "Bad request";
rb_exc_raise(rb_exc_new_cstr(rb_eRuntimeError, msg));
