#include <ruby.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	ruby_init();

	VALUE cTest = rb_define_class("Test", rb_cObject);

	VALUE x = rb_funcall(cTest, rb_intern("new"), 0);

	switch (TYPE(x))
	{
		case T_NIL:
			printf("nil\n");
			break;
		case T_CLASS:
			printf("class\n");
			break;
		case T_MODULE:
			printf("module\n");
			break;
		case T_FLOAT:
			printf("float\n");
			break;
		case T_STRING:
			printf("string\n");
			break;
		case T_REGEXP:
			printf("regexp\n");
			break;
		case T_ARRAY:
			printf("array\n");
			break;
		case T_HASH:
			printf("hash\n");
			break;
		case T_STRUCT:
			printf("struct\n");
			break;
		case T_BIGNUM:
			printf("bignum\n");
			break;
		case T_FIXNUM:
			printf("fixnum\n");
			break;
		case T_COMPLEX:
			printf("complex\n");
			break;
		case T_RATIONAL:
			printf("rational\n");
			break;
		case T_FILE:
			printf("file\n");
			break;
		case T_TRUE:
			printf("true\n");
			break;
		case T_FALSE:
			printf("false\n");
			break;
		case T_DATA:
			printf("data\n");
			break;
		case T_SYMBOL:
			printf("symbol\n");
			break;
		case T_OBJECT:
			printf("object\n");
			break;
		default:
			printf("N/A\n");
	}

	return ruby_cleanup(0);
}
