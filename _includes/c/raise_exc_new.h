const char msg[] = "Message possibly with NULL bytes";
rb_exc_raise(rb_exc_new(rb_eRuntimeError, msg, sizeof(msg)));
