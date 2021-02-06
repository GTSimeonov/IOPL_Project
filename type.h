#ifndef GCC_TYPE_H 
#define GCC_TYPE_H GCC_TYPE_H
#include "type.hh"
#include "denoted.h"
#include "scope.h"
#include <limits.h>

#define PTR_SIZE 4

#define INT_SIZE 4

#define CHAR_SIZE 1

#define FLOAT_SIZE 4
#define DOUBLE_SIZE 8





enum Type_Specifier;
enum Type_Constraint;
enum Type_Signedness;

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

	char is_finished;
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
	struct Scope *function_prototype_scope;

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
	struct Queue *consts;
	char is_finished;
};

struct Type* get_type_error(struct Type* error);
struct Type* get_struct_union_type(struct Denotation_Prototype *prototype);
struct Struct_Union* get_struct_union_base(struct Scope *scope ,enum Type_Specifier struct_or_union);
struct Enum *get_enum_base();
struct Type* get_basic_type(struct Denotation_Prototype *prototype);
struct Type* get_pointer_type(struct Type* points_to);
struct Type* get_array_type(struct Type *is_array_of,struct AST* number_of_elements);
struct Type* get_enum_type(struct Denotation_Prototype *prototype);
struct Type* get_type_bitfield(struct Type* base,struct AST* number_of_bits);
struct Type* get_function_type(struct Type* return_type,struct Queue *parameters,struct Scope* function_prototype_scope);
char is_type(struct Queue *tokens,struct Scope *scope);
size_t get_type_size(struct Type *type);



#endif
