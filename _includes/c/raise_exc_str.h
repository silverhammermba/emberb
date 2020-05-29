VALUE msg = rb_sprintf("Error code %d", 404);
rb_exc_raise(rb_exc_new_str(rb_eRuntimeError, msg));
