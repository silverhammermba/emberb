#include <ruby.h>
#include <gmp.h>
#include <string.h>

void integer_free(mpz_t* data)
{
	/* free memory allocated by GMP */
	mpz_clear(*data);

	free(data);
}

VALUE integer_c_alloc(VALUE self)
{
	mpz_t* data = malloc(sizeof(mpz_t));
	/* GMP initialization */
	mpz_init(*data);

	return Data_Wrap_Struct(self, NULL, integer_free, data);
}

VALUE integer_m_initialize(VALUE self, VALUE x)
{
	mpz_t* data;
	Data_Get_Struct(self, mpz_t, data);

	VALUE str;

	switch (TYPE(x))
	{
		case T_FIXNUM:
			mpz_set_si(*data, FIX2LONG(x));
			return self;
		case T_BIGNUM:
			str = rb_funcall(x, rb_intern("to_s"), 0);
			break;
		case T_STRING:
			str = x;
			break;
		case T_DATA:
		case T_OBJECT:
			rb_raise(rb_eTypeError, "%"PRIsVALUE" is not an integer type", x);
			break;
		default:
			rb_raise(rb_eRuntimeError, "invalid object %+"PRIsVALUE, x);
			break;
	}

	char* cstr = StringValueCStr(str);
	mpz_set_str(*data, cstr, 10);

	return self;
}

VALUE integer_m_to_s(VALUE self)
{
	mpz_t* data;
	Data_Get_Struct(self, mpz_t, data);

	char* cstr = malloc(mpz_sizeinbase(*data, 10) + 2);
	mpz_get_str(cstr, 10, *data);

	VALUE str = rb_str_new_cstr(cstr);

	free(cstr);

	return str;
}

void Init_gmp()
{
	VALUE mGMP = rb_define_module("GMP");

	VALUE cInteger = rb_define_class_under(mGMP, "Integer", rb_cObject);
	rb_define_alloc_func(cInteger, integer_c_alloc);
	rb_define_method(cInteger, "initialize", integer_m_initialize, 1);
	rb_define_method(cInteger, "to_s", integer_m_to_s, 0);
}
