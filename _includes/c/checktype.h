Check_Type(obj, T_STRING); /* raises a TypeError if TYPE(obj) != T_STRING */
RB_TYPE_P(obj, T_STRING);  /* same as TYPE(obj) == T_STRING */

/* tests for specific types */
FIXNUM_P(obj);             /* same as TYPE(obj) == T_FIXNUM */
RB_FLOAT_TYPE_P(obj);      /* same as TYPE(obj) == T_FLOAT */
SYMBOL_P(obj);             /* same as TYPE(obj) == T_SYMBOL */
NIL_P(obj);                /* same as TYPE(obj) == T_NIL */
RTEST(obj);                /* test if obj is "truthy" i.e. not nil or false */
