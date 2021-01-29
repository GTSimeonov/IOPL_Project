#ifndef GCC_TYPE_H 
#define GCC_TYPE_H GCC_TYPE_H
#include "queue.c"
#include "map.c"
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
	TS_PTR_ERROR,
	TS_ARR_ERROR,
	TS_FUNC_ERROR,
	TS_GROUP_ERROR,
	TS_ID_ERROR,
	TS_ERROR
};
enum Type_Constraint
{
	TC_LONG,
	TC_LONG_LONG,
	TC_SHORT,
	TC_NONE
};
struct Struct_Union
{
	struct Map inner_namespace;
	struct Queue declarations;
	struct token* id;
};
struct Type_Node
{
	enum Type_Specifier type_specifier;
	enum Type_Constraint type_constraints;
	char is_signed:1;
	char is_const:1;
	char is_volatile:1;
	char is_extern:1;
	char is_static:1;
	char is_bit_field:1;
	char is_typedef:1;
	char error:1;

	union 
	{
		struct Queue arg_types;
		struct {
			struct AST *number_of_elements_expr;
			size_t number_of_elements;
			}arr;
		struct Struct_Union *struct_union;
		struct AST* bit_field_length;
		void *error;
	}specifics;
	struct Type *type_def;

};
struct Type
{
	struct Queue components;
	size_t size;
};

struct Type_Node* check_first_type_component(struct Type *type);
struct Type_Node* check_base_type_component(struct Type *type);
size_t size_of(struct Type *type);
size_t size_of_array(struct Type *type);
size_t size_of_struct(struct Type *type);
size_t size_of_union(struct Type *type);
struct Type_Node* get_node();
struct Struct_Union* get_struct_union();
void merge_type_nodes(struct Type_Node *consumer,struct Type_Node *source);
#endif
