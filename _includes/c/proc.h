VALUE proc;

/* assuming proc is assigned a Proc from somewhere */
result = rb_funcall_with_block(obj, rb_intern("each"), 0, NULL, proc);
