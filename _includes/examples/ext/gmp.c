#include <ruby.h>
#include <gmp.h>
#include <string.h>

/*
 * we're going to be unwrapping VALUEs to get the C data A LOT.
 * It's not that hard, but it gets tedious. This lets us go
 * straight from a VALUE to the underlying data
 */
#define UNWRAP(val, data) \
	mpz_t* data;\
	Data_Get_Struct(val, mpz_t, data);

/*
 * we're also going to be pretty strict about accepting only
 * objects of our GMP::Integer type, so this will be a frequent test
 */
#define CHECK_MPZ(val) \
	if (CLASS_OF(val) != cInteger)\
		rb_raise(rb_eTypeError, "%+"PRIsVALUE" is not a %"PRIsVALUE, val, cInteger);

/* it's nice to have these as globals for easy access in methods */
VALUE mGMP;
VALUE cInteger;

/* function to free data wrapped in GMP::Integer */
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

/* GMP::Integer#initialize
 *
 * Sets internal mpz_t using first argument
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

	UNWRAP(self, data);

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
				UNWRAP(val, other);

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

	UNWRAP(self, data);

	/* get C string from GMP */
	char* cstr = malloc(mpz_sizeinbase(*data, base) + 2);
	mpz_get_str(cstr, base, *data);

	/* create Ruby String */
	VALUE str = rb_str_new_cstr(cstr);

	/* free memory */
	free(cstr);

	return str;
}

/* GMP::Integer#to_i */
VALUE integer_m_to_i(VALUE self)
{
	/* safest and easiest way to convert is to call to_s.to_i */
	return rb_funcall(integer_m_to_s(0, NULL, self), rb_intern("to_i"), 0);
}

/* GMP::Integer#<=> */
VALUE integer_m_spaceship(VALUE self, VALUE x)
{
	CHECK_MPZ(x);

	UNWRAP(self, data);
	UNWRAP(x, other);

	/* shortcut for identical objects */
	if (data == other)
		return INT2FIX(0);

	return INT2FIX(mpz_cmp(*data, *other));
}

/* GMP::Integer#== */
VALUE integer_m_eq(VALUE self, VALUE x)
{
	/* for GMP::Integers, use <=> */
	if (CLASS_OF(x) == cInteger)
		return integer_m_spaceship(self, x) == INT2FIX(0) ? Qtrue : Qfalse;

	return rb_call_super(1, &x);
}

/* GMP::Integer#+ */
VALUE integer_m_add(VALUE self, VALUE x)
{
	CHECK_MPZ(x);

	UNWRAP(self, data);
	UNWRAP(x, other);

	/*
	 * we need a new GMP::Integer to store the result, but there's no need
	 * to actually use the `new` method
	 */
	VALUE result = integer_c_alloc(cInteger);
	UNWRAP(result, res);

	mpz_add(*res, *data, *other);

	return result;
}
/* multiplication and subtraction would be defined nearly identically */

VALUE integer_m_neg(VALUE self)
{
	UNWRAP(self, data);

	/* bypassing `new` as in the + method */
	VALUE result = integer_c_alloc(cInteger);
	UNWRAP(result, res);

	mpz_neg(*res, *data);

	return result;
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
	rb_define_method(cInteger, "to_i", integer_m_to_i, 0);
	rb_define_method(cInteger, "<=>", integer_m_spaceship, 1);
	rb_define_method(cInteger, "==", integer_m_eq, 1);
	rb_define_method(cInteger, "+", integer_m_add, 1);
	rb_define_method(cInteger, "-@", integer_m_neg, 0);

	rb_define_alias(cInteger, "inspect", "to_s");
}
