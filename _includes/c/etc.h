/* klass.include module */
rb_include_module(klass, module);

/* klass.prepend module */
rb_prepend_module(klass, module);

/* obj.extend module */
rb_extend_object(obj, module);

/* klass.class_eval { undef :method } */
rb_undef_method(klass, "method")

/* klass.class_eval { alias :meth2 :meth1 } */
rb_define_alias(klass, "meth2", "meth1")

/* klass.attr_reader :x */
rb_define_attr(klass, "x", 1, 0);
/* klass.attr_writer :x */
rb_define_attr(klass, "x", 0, 1);
/* klass.attr_accessor :x */
rb_define_attr(klass, "x", 1, 1);

/* obj.singleton_class # handy in combination with the other functions */
VALUE singleton;
singleton = rb_singleton_class(obj);
