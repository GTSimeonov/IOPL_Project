#ifndef WONKY_TYPE_HH
#define WONKY_TYPE_HH WONKY_TYPE_HH

#define PTR_SIZE 4
#define INT_SIZE 4
#define CHAR_SIZE 1
#define FLOAT_SIZE 4
#define DOUBLE_SIZE 8


#define AS_BASIC_TYPE_PTR(x) ((struct Type_Basic*)x)
#define AS_STRUCT_UNION_PTR(x) ((struct Type_Struct_Union*)x)
#define AS_TYPE_PTR_PTR(x) ((struct Type_Pointer*)x)
#define AS_TYPE_ARR_PTR(x) ((struct Type_Array*)x)
#define AS_TYPE_ENUM_PTR(x) ((struct Type_Enum*)x)
#define AS_TYPE_FUNC_PTR(x) ((struct Type_Function*)x)

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
