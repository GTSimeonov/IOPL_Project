#ifndef GCC_TYPE_HH
#define GCC_TYPE_HH GCC_TYPE_HH


/*this isn't just type-specifier :DD*/
enum Type_Specifier
{
	TS_VOID,
	TS_CHAR,
	TS_INT,
	TS_FLOAT,
	TS_DOUBLE,
	TS_STRUCT,
	TS_ENUM,
	TS_UNION,
	TS_POINTER,
	TS_ARRAY,
	TS_FUNC,
	TS_BITFIELD,
	TS_NONE,
	TS_ERROR
};
enum Type_Constraint
{
	TC_LONG,
	TC_LONG_LONG,
	TC_SHORT,
	TC_NONE
};
enum Type_Signedness
{
	TSIGN_SIGNED,
	TSIGN_UNSIGNED,
	TSIGN_NONE
};
struct Type;
struct Type_Error;
struct Type_Struct_Union;
struct Struct_Union;
struct Type_Bit_Field;
struct Type_Basic;
struct Type_Pointer;
struct Type_Array;
struct Type_Function;
struct Type_Enum;
struct Enum;
struct Type_Map_Pair;

#endif
