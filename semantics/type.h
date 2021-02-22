#ifndef GCC_TYPE_H 
#define GCC_TYPE_H GCC_TYPE_H
#include <type.hh>
#include <denoted.h>
#include <scope.h>
#include <limits.h>
#include <program.h>
#include <map.h>
#include <ast.h>

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
	/*queue of denoted objects for preserving the order of the members*/
	struct Queue *members;
	struct Normal_Scope *inner_namespace;

	char is_finished;
};
struct Type_Bit_Field
{
	enum Type_Specifier specifier;
	size_t number_of_bits;
	struct Type *base;
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
};
struct Type_Function
{
	enum Type_Specifier specifier;
	struct Type *return_type;
	/*types*/
	size_t number_of_arguments;
	/*array of pointers to Type*/
	struct Type** arguments;
	struct Normal_Scope *function_prototype_scope;

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

struct Type_Map_Pair
{
	struct Type *type;
	/*corresponding map node*/
	struct Map *node;
};



void type_check_and_push_heavy(struct Type_Map_Pair *pair,struct Type *type);
void type_check_and_push(struct Type_Map_Pair *pair,struct Type *type,size_t struct_size);

struct Type_Map_Pair* get_type_map_pair(struct Type *type,struct Map *types);
void get_type_error(struct Type_Map_Pair *pair);
void get_struct_union_type(struct Denotation_Prototype *prototype);
struct Struct_Union* get_struct_union_base(struct Scope *scope ,enum Type_Specifier struct_or_union);
struct Enum *get_enum_base();
void get_basic_type(struct Denotation_Prototype *prototype);
void get_pointer_type(struct Type_Map_Pair *pair,char is_volatile,char is_constant);
void get_array_type(struct Type_Map_Pair *pair,struct AST* number_of_elements);
void get_enum_type(struct Denotation_Prototype *prototype);
void get_type_bitfield(struct Type_Map_Pair *pair,struct AST* number_of_bits);

void delete_enum(struct Enum *enumeration);
void delete_struct_union(struct Struct_Union *su);


void get_function_type(struct Type_Map_Pair *pair,struct Queue *parameters,struct Normal_Scope* function_prototype_scope);
char is_type(struct Translation_Data *translation_data,struct Scope *scope);
size_t get_type_size(struct Type *type);







#endif
