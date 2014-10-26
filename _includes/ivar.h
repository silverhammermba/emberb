VALUE obj;
VALUE iv;

iv = rb_iv_get(obj, "@x");
rb_iv_set(obj, "@x", iv);
