#ifndef GCC_DENOTED_H
#define GCC_DENOTED_H GCC_DENOTED_H
#include "denoted.hh"
#include "type.h"
#include "scope.h"
#include "semantics.h"


enum Denotation_Type;
enum Function_Specifier;
enum Storage_Class;


struct Denoted
{
	enum Denotation_Type denotation;
};


struct Denoted_Error
{
	enum Denotation_Type denotation;
	struct Denoted *error;
};
struct Denoted_Base
{
	enum Denotation_Type denotation;
	struct token *id;
	struct Type *type;
};
struct Denoted_Function
{
	enum Denotation_Type denotation;
	struct token *id;
	struct Type *type;


	enum Function_Specifier function_specifier;
	struct AST_Compound_Statement *body;
};
struct Denoted_Object
{
	enum Denotation_Type denotation;
	struct token *id;

	struct Object *object;
};
struct Denoted_Typedef
{
	enum Denotation_Type denotation;
	struct token *id;
	struct Type *type;

};
struct Denoted_Enum
{
	enum Denotation_Type denotation;
	struct token *id;

	struct Enum *enumeration;
};
struct Denoted_Enum_Const
{
	enum Denotation_Type denotation;
	struct token *id;

	struct Enum *parent;
	int value;
	struct AST* expression;
};

struct Denoted_Struct_Union
{
	enum Denotation_Type denotation;
	struct token *id;

	struct Struct_Union *struct_union;
};

struct Denotation_Prototype
{
	enum Denotation_Type denotation;
	struct Type *type;


	enum Storage_Class storage_class;
	enum Type_Specifier specifier;
	enum Type_Constraint constraint;
	enum Type_Signedness sign;
	enum Function_Specifier function_specifier;

	struct Struct_Union *struct_union;
	struct Enum *enumerator;

	size_t size;
	char is_const:1;
	char is_volatile:1;
};
struct Object
{
	struct Type *type;
	struct Location *location;
	enum Storage_Class storage_class;
};




struct Denoted* get_denoted_error(struct Denoted *error);
struct Denoted* get_denoted_function(struct token *id,struct Type *return_type,enum Function_Specifier fs);
struct Denoted* get_denoted_object(struct token *id, enum Storage_Class sc,struct Type *type);
struct Denoted* get_denoted_typedef(struct Denoted_Base *base);
struct Denoted* get_denoted_enum_const_expr(struct token *id,struct Enum *parent,struct AST* expression);
struct Denoted* get_denoted_enum_const_num(struct token *id,struct Enum *parent,int value);
struct Denoted* get_denoted_enum(struct token *id,struct Enum *enumerator);
struct Denoted* get_denoted_struct_union(struct token *id,struct Struct_Union *struct_union);
struct Denoted* get_denoted_base(struct token *id,struct Type *type,enum Denotation_Type denotation);

struct Denoted* extract_denoted(struct Denoted_Base *base,struct Denotation_Prototype *prototype,char allow_abstract);
struct Denoted* get_denotation_prototype();
#endif
