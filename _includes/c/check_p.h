FIXNUM_P(obj);        /* like RB_TYPE_P(obj, T_FIXNUM) */
RB_FLOAT_TYPE_P(obj); /* like RB_TYPE_P(obj, T_FLOAT) */
SYMBOL_P(obj);        /* like RB_TYPE_P(obj, T_SYMBOL) */
NIL_P(obj);           /* like RB_TYPE_P(obj, T_NIL) */
RTEST(obj);           /* return true if obj is "truthy" i.e. not nil or false */
