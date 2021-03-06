#ifndef WONKY_TYPE_H 
#define WONKY_TYPE_H WONKY_TYPE_H
#include <type.hh>
#include <denoted.h>
#include <scope.h>
#include <limits.h>
#include <program.h>
#include <map.h>
#include <ast.h>






enum Type_Specifier;
enum Type_Constraint;
enum Type_Signedness;

struct Type
{
	enum Type_Specifier specifier;
	struct Map *node;
};
struct Type_Error
{	
	enum Type_Specifier specifier;
	struct Map *node;
	struct Type *error;
};

struct Type_Struct_Union
{
	enum Type_Specifier specifier;
	struct Map *node;
	struct Struct_Union *struct_union;

	char is_const:1;
	char is_volatile:1;
};
struct Struct_Union
{
	enum Type_Specifier specifier;
	struct Map *node;
	size_t size;
	/*queue of denoted objects for preserving the order of the members*/
	struct Queue *members;
	struct Normal_Scope *inner_namespace;

	char is_finished;
};
struct Type_Bit_Field
{
	enum Type_Specifier specifier;
	struct Map *node;
	size_t number_of_bits;
	struct Type *base;
};
struct Type_Basic
{
	enum Type_Specifier specifier;
	enum Type_Constraint constraint;
	struct Map *node;
	enum Type_Signedness sign;
	size_t size;
	char is_const:1;
	char is_volatile:1;
};
struct Type_Pointer
{
	enum Type_Specifier specifier;
	struct Map *node;
	size_t size;
	struct Type *points_to;
	char is_const:1;
	char is_volatile:1;
};
struct Type_Array
{
	enum Type_Specifier specifier;
	struct Map *node;
	size_t size;
	size_t number_of_elements;
	struct Type *is_array_of;
};
struct Type_Function
{
	enum Type_Specifier specifier;
	struct Map *node;
	struct Type *return_type;
	/*types*/
	size_t number_of_arguments;

	struct Denoted_Object** arguments;
	struct Normal_Scope *function_prototype_scope;

};
struct Type_Enum
{
	enum Type_Specifier specifier;
	struct Map *node;
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



struct Type* type_check_and_push(struct Type *type,struct Map *base,size_t struct_size);


struct Type* get_type_error(struct Type *type);
struct Type* get_struct_union_type(struct Denotation_Prototype *prototype);
struct Struct_Union* get_struct_union_base(struct Scope *scope ,enum Type_Specifier struct_or_union);
struct Enum *get_enum_base();
struct Type* get_basic_type(struct Denotation_Prototype *prototype);
struct Type* get_pointer_type(struct Type *points_to,char is_const,char is_volatile);
struct Type* get_array_type(struct Type *array_of,struct AST* number_of_elements,struct Translation_Data *translation_data);
struct Type* get_enum_type(struct Denotation_Prototype *prototype);
struct Type* get_type_bitfield(struct Type *base,struct AST* number_of_bits,struct Translation_Data *translation_data);
struct Type* get_function_type(struct Type *return_type,struct Queue *parameters,struct Normal_Scope* function_prototype_scope);

void delete_enum(struct Enum *enumeration);
void delete_struct_union(struct Struct_Union *su);
void delete_type(void *type);

char is_type(struct Translation_Data *translation_data,struct Scope *scope);
size_t get_type_size(struct Type *type);

char types_are_identical(struct Type *a,struct Type *b);






#endif
