Check_Type(obj, T_STRING); /* raises a TypeError if TYPE(obj) != T_STRING */
FIXNUM_P(obj);             /* same as TYPE(obj) == T_FIXNUM */
NIL_P(obj);                /* same as TYPE(obj) == T_NIL */
