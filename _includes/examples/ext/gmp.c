#include <ruby.h>
#include <gmp.h>
#include <string.h>

/* it's nice to have these as globals for easy access in methods */
VALUE mGMP;
VALUE cInteger;

void integer_free(mpz_t* data)
{
	/* free memory allocated by GMP */
	mpz_clear(*data);

	free(data);
}

/* GMP::Integer.allocate */
VALUE integer_c_alloc(VALUE self)
{
	mpz_t* data = malloc(sizeof(mpz_t));
	/* GMP initialization */
	mpz_init(*data);

	return Data_Wrap_Struct(self, NULL, integer_free, data);
}

/* construct a GMP::Integer from an mpz_t */
VALUE integer_from_mpz(mpz_t x)
{
	VALUE val = integer_c_alloc(cInteger);

	mpz_t* data;
	Data_Get_Struct(val, mpz_t, data);
	mpz_set(*data, x);

	return val;
}

/* GMP::Integer#initialize
 *
 * Converts first argument to an mpz_t.
 *
 * If the first argument is a String, you can supply a second Fixnum argument
 * as the base for interpreting the String. The default base of 0 means that
 * the base will be determined by the String's prefix.
 */
VALUE integer_m_initialize(int argc, VALUE* argv, VALUE self)
{
	int base = 0;

	/* check for optional base argument */
	VALUE val;
	VALUE rbase;
	if (rb_scan_args(argc, argv, "11", &val, &rbase) == 2)
	{
		/* base only makes sense with a string */
		Check_Type(val, T_STRING);
		Check_Type(rbase, T_FIXNUM);

		base = FIX2INT(rbase);

		/* GMP only accepts certain bases */
		if (!(base >= 2 && base <= 62) && base != 0)
			rb_raise(rb_eRangeError, "base must be 0 or in (2..62)");
	}

	/* unwrap */
	mpz_t* data;
	Data_Get_Struct(self, mpz_t, data);

	VALUE str;

	switch (TYPE(val))
	{
		case T_FIXNUM:
			/* easy case */
			mpz_set_si(*data, FIX2LONG(val));
			return self;
		case T_BIGNUM:
			/* this is the easiest way to safely convert */
			str = rb_funcall(val, rb_intern("to_s"), 0);
			base = 10;
			break;
		case T_STRING:
			str = val;
			break;
		case T_DATA:
			/* copy another GMP::Integer */
			if (CLASS_OF(val) == cInteger)
			{
				mpz_t* other;
				Data_Get_Struct(val, mpz_t, other);

				mpz_set(*data, *other);

				return self;
			}
			/* break intentionally omitted */
		case T_OBJECT:
			rb_raise(rb_eTypeError, "%+"PRIsVALUE" is not an integer type", val);
			break;
		default:
			/* shouldn't get here */
			rb_raise(rb_eRuntimeError, "invalid object %+"PRIsVALUE, val);
			break;
	}

	/* assign */
	char* cstr = StringValueCStr(str);
	if (mpz_set_str(*data, cstr, base))
	{
		if (base == 0)
			rb_raise(rb_eArgError, "invalid number: %"PRIsVALUE, val);
		else
			rb_raise(rb_eArgError, "invalid base %d number: %"PRIsVALUE, base, val);
	}

	return self;
}

/* GMP::Integer#to_s
 *
 * Accepts an optional Fixnum argument for the base of the String (default 10)
 */
VALUE integer_m_to_s(int argc, VALUE* argv, VALUE self)
{
	int base = 10;

	/* check for optional base argument */
	VALUE rbase;
	if (rb_scan_args(argc, argv, "01", &rbase) == 1)
	{
		Check_Type(rbase, T_FIXNUM);

		base = FIX2INT(rbase);

		/* GMP only accepts certain bases */
		if (!(base >= -36 && base <= -2) && !(base >= 2 && base <= 62))
			rb_raise(rb_eRangeError, "base must be in (-36..-2) or (2..62)");
	}

	/* unwrap */
	mpz_t* data;
	Data_Get_Struct(self, mpz_t, data);

	/* get C string from GMP */
	char* cstr = malloc(mpz_sizeinbase(*data, base) + 2);
	mpz_get_str(cstr, base, *data);

	/* create Ruby String */
	VALUE str = rb_str_new_cstr(cstr);

	/* free memory */
	free(cstr);

	return str;
}

/* entry point */
void Init_gmp()
{
	mGMP = rb_define_module("GMP");

	/* define GMP::Integer */
	cInteger = rb_define_class_under(mGMP, "Integer", rb_cObject);
	rb_define_alloc_func(cInteger, integer_c_alloc);
	rb_define_method(cInteger, "initialize", integer_m_initialize, -1);
	rb_define_method(cInteger, "to_s", integer_m_to_s, -1);
	rb_define_alias(cInteger, "inspect", "to_s");
}
