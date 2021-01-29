#ifndef GCC_TYPE_H 
#define GCC_TYPE_H GCC_TYPE_H
#include "queue.c"
#include "map.c"
#include "scope.h"
#include <limits.h>

#define PTR_SIZE 4

#define INT_SIZE 4

#define CHAR_SIZE 1

#define FLOAT_SIZE 4
#define DOUBLE_SIZE 8




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
	TS_NONE,
	TS_ERROR,
};
enum Type_Constraint
{
	TC_LONG,
	TC_LONG_LONG,
	TC_SHORT,
	TC_NONE
};
enum Type_Storage_Class
{
	TSC_EXTERN,
	TSC_STATIC,
	TSC_NONE
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
struct Type_Prototype
{
	enum Type_Specifier specifier;
	enum Type_Storage_Class storage_class;
	enum Type_Constraint constraint;
	enum Type_Signedness sign;
	size_t size;
	char is_const:1;
	char is_volatile:1;
	struct Type *points_to;
};
struct Type_Struct_Union
{
	enum Type_Specifier specifier;
	size_t size;

	size_t number_of_members;
	struct Denoted_Struct_Union_Member **members;
	struct Scope *inner_namespace;
	struct token *id;
	char is_const:1;
	char is_volatile:1;
};
struct Type_Basic
{
	enum Type_Specifier specifier;
	enum Type_Storage_Class storage_class;
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
	enum Type_Storage_Class storage_class;
	size_t size;
	struct Type *points_to;
	char is_const:1;
	char is_volatile:1;
};
struct Type_Array
{
	enum Type_Specifier specifier;
	enum Type_Storage_Class storage_class;
	size_t size;
	size_t number_of_elements;
	struct Type *is_array_of;
};
struct Type_Function
{
	enum Type_Specifier specifier;
	struct Type *return_type;

	struct Declarator **parameters;
	size_t number_of_parameters;
};
struct Type_Enum
{
	enum Type_Specifier specifier;

	size_t number_of_constants;
	struct Denoted_Enum_Const **consts;
	struct token *id;
};
struct Type_Error* get_type_error(struct Type* error);
struct Type_Prototype* get_type_prototype();
struct Type* get_struct_union(struct Type_Prototype *prototype,struct token *id,enum Type_Specifier ts);
struct Type* get_basic_type(struct Type_Prototype *prototype);
struct Type* get_pointer_type(struct Type* points_to);
struct Type* get_array_type(struct Type *is_array_of);
struct Type* get_enum_type(struct token *id);



#endif
