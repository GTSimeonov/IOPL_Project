#ifndef GCC_TYPE_H 
#define GCC_TYPE_H GCC_TYPE_H
#include "denoted.h"
#include "scope.h"
#include <limits.h>

#define PTR_SIZE 4

#define INT_SIZE 4

#define CHAR_SIZE 1

#define FLOAT_SIZE 4
#define DOUBLE_SIZE 8



struct Denotation_Prototype;

/*this isn't just type-specifier*/
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
	TS_TYPEDEF,
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


struct Type
{
	enum Type_Specifier specifier;
};
struct Type_Error
{	
	enum Type_Specifier specifier;
	struct Type *error;
};

struct Type_Struct_Union
{
	enum Type_Specifier specifier;
	struct Struct_Union *struct_union;

	char is_const:1;
	char is_volatile:1;
};
struct Struct_Union
{
	enum Type_Specifier specifier;
	size_t size;
	struct Queue *members;
	struct Scope *inner_namespace;
};
struct Type_Bit_Field
{
	enum Type_Specifier specifier;
	size_t number_of_bits;
	struct Type *base;
	struct AST *expression;
};
struct Type_Basic
{
	enum Type_Specifier specifier;
	enum Type_Constraint constraint;
	enum Type_Signedness sign;
	size_t size;
	char is_const:1;
	char is_volatile:1;
	char is_signed:1;
};
struct Type_Pointer
{
	enum Type_Specifier specifier;
	size_t size;
	struct Type *points_to;
	char is_const:1;
	char is_volatile:1;
};
struct Type_Array
{
	enum Type_Specifier specifier;
	size_t size;
	size_t number_of_elements;
	struct Type *is_array_of;

	struct AST *expression;
};
struct Type_Function
{
	enum Type_Specifier specifier;
	struct Type *return_type;
	struct Queue *parameters;

};
struct Type_Enum
{
	enum Type_Specifier specifier;
	struct Enum *enumeration;

	char is_const:1;
	char is_volatile:1;

};
struct Enum
{
	enum Type_Specifier specifier;
	size_t number_of_constants;
	struct Denoted_Enum_Const **consts;
};

struct Type* get_type_error(struct Type* error);
struct Type* get_struct_union_type(struct Denotation_Prototype *prototype);
struct Struct_Union* get_struct_union_base();
struct Enum *get_enum_base();
struct Type* get_basic_type(struct Denotation_Prototype *prototype);
struct Type* get_pointer_type(struct Type* points_to);
struct Type* get_array_type(struct Type *is_array_of,struct AST* number_of_elements);
struct Type* get_enum_type(struct Denotation_Prototype *prototype);
struct Type* get_type_bitfield(struct Type* base,size_t number_of_bits);
struct Type* get_function_type(struct Type* return_type,struct Queue *parameters);



#endif
