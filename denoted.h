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
	DT_Struct_Union_Tag,
	DT_Error
};

enum Function_Specifier
{
	TFS_Inline,
	TFS_None
};
enum Storage_Class
{
	TSC_EXTERN,
	TSC_STATIC,
	TSC_NONE
};
struct Denoted
{
	enum Denotation_Type denotation;
};


struct Denoted_Error
{
	enum Denotation_Type denotation;
	struct Denoted *error;
};
struct Denoted_Function
{
	enum Denotation_Type denotation;
	enum Function_Specifier function_specifier;

	struct Type *type;
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
struct Denoted_Struct_Union
{
	enum Denotation_Type denotation;
	struct token *id;
	struct Struct_Union *struct_union;
};

struct Denotation_Prototype
{
	enum Storage_Class storage_class;
	enum Type_Constraint constraint;
	enum Type_Signedness sign;
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
struct Denoted* get_denoted_function(struct Type *type);
struct Denoted* get_denoted_object(struct Type *type);
struct Denoted* get_denoted_typedef(struct token* id,struct Type *typedefed);
struct Denoted* get_denoted_enum_const(struct token *id,struct Type_Enum *parent,int value);
struct Denoted* get_denoted_struct_union_member(struct token *id,struct Type *type,size_t offset);
struct Denoted* get_denoted_struct_union(struct token *id,struct Struct_Union *struct_union);

struct Object_Prototype* get_denotation_prototype();
#endif
