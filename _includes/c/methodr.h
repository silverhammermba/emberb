/* the usual */
rb_define_method(klass, "my_method", my_method, argc);
/* or, like a toplevel def (by defining a private method in Kernel) */
rb_define_global_function("my_method", my_method, argc);

/* or, with access control */
rb_define_private_method(klass, "my_method", my_method, argc);
rb_define_protected_method(klass, "my_method", my_method, argc);

/* or, in the singleton class */
rb_define_singleton_method(object, "my_method", my_method, argc);
