#ifndef GCC_DENOTED_H
#define GCC_DENOTED_H GCC_DENOTED_H
#include "type.h"
#include "ast.h"

enum Denotation_Type
{
	DT_Macro,
	DT_Macro_Parameter,
	DT_Label,
	DT_Object,
	DT_Typedef,
	DT_Function,
	DT_Enum_Constant,
	DT_Struct_Union_Member,
	DT_Error
};

struct Denoted
{
	enum Denotation_Type denotation;
};

struct Declarator
{
	struct Denoted *thing;
	struct token *id;
};

struct Denoted_Error
{
	enum Denotation_Type denotation;
	struct Denoted *error;
}
struct Denoted_Function
{
	enum Denotation_Type denotation;

	struct Type *type;
	struct AST_Compound_Statement *body;
};
struct Denoted_Object
{
	enum Denotation_Type denotation;

	enum Type_Storage_Class storage_class;
	struct Type *type;
	struct Location *location;
};
struct Denoted_Typedef
{
	enum Denotation_Type denotation;
	struct token *id;
	struct Type *type;
};
struct Denoted_Enum_Const
{
	enum Denotation_Type denotation;
	struct token *id;
	struct Type_Enum *parent;
	int value;
};

struct Denoted_Struct_Union_Member
{
	enum Denotation_Type denotation;
	struct Type *type;
	size_t offset;
	struct token *id;
};

#endif
