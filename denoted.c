#ifndef GCC_DENOTED_C
#define GCC_DENOTED_C GCC_DENOTED_C
#include "denoted.h"


struct Denoted* get_denoted_error(struct Denoted *error)
{
	struct Denoted_Error *ret;
	ret=malloc(sizeof(struct Denoted_Error));
	ret->denotation=DT_Error;
	ret->error=error;

	return (struct Denoted*)ret;
}
struct Denoted* get_denoted_base(struct token *id,struct Type *type,enum Denotation_Type denotation)
{
	struct Denoted_Base *ret;
	ret=malloc(sizeof(struct Denoted_Base));
	ret->denotation=denotation;
	ret->id=id;
	ret->type=type;


	return (struct Denoted*)ret;
}
struct Denoted* get_denoted_function(struct token *id,struct Type *return_type,enum Function_Specifier fs)
{
	struct Denoted_Function *ret;
	ret=malloc(sizeof(struct Denoted_Function));
	ret->denotation=DT_Function;
	ret->id=id;
	ret->return_type=return_type;
	ret->function_specifier=fs;
	ret->body=NULL;

	return (struct Denoted*)ret;
}
struct Denoted* get_denoted_object(struct token *id, enum Storage_Class sc,struct Type *type)
{
	struct Denoted_Object *ret;
	ret=malloc(sizeof(struct Denoted_Object));
	ret->denotation=DT_Object;
	ret->id=id;

	ret->object=malloc(sizeof(struct Object));
	ret->object->type=type;
	ret->object->location=NULL;
	ret->object->storage_class=sc;

	return (struct Denoted*)ret;
}

struct Denoted* get_denoted_typedef(struct Denoted_Base *base)
{
	struct Denoted_Typedef *ret;
	ret=malloc(sizeof(struct Denoted_Typedef));
	ret->denotation=DT_Typedef;
	ret->type=base->type;
	ret->id=base->id;

	return (struct Denoted*)ret;

}
struct Denoted* get_denoted_enum_const_expr(struct token *id,struct Enum *parent,struct AST* expression)
{
	struct Denoted_Enum_Const *ret;
	ret=malloc(sizeof(struct Denoted_Enum_Const));
	ret->denotation=DT_Enum_Constant;
	ret->id=id;
	ret->parent=parent;
	ret->expression=expression;
	ret->value=evaluate_const_expression_integer(expression);

	return (struct Denoted*)ret;
	
}
struct Denoted* get_denoted_enum_const_num(struct token *id,struct Enum *parent,int value)
{
	struct Denoted_Enum_Const *ret;
	ret=malloc(sizeof(struct Denoted_Enum_Const));
	ret->denotation=DT_Enum_Constant;
	ret->id=id;
	ret->parent=parent;
	ret->expression=NULL;
	ret->value=value;

	return (struct Denoted*)ret;
}
struct Denoted* get_denoted_enum(struct token *id,struct Enum *enumerator)
{
	struct Denoted_Enum *ret;
	ret=malloc(sizeof(struct Denoted_Enum));
	ret->denotation=DT_Enum;
	ret->id=id;
	ret->enumeration=enumerator;

	return (struct Denoted*)ret;
}
struct Denoted* get_denoted_struct_union(struct token *id,struct Struct_Union *struct_union)
{
	struct Denoted_Struct_Union *ret;
	ret=malloc(sizeof(struct Denoted_Struct_Union));
	ret->denotation=DT_Struct_Union_Tag;
	ret->id=id;
	ret->struct_union=struct_union;

	return (struct Denoted*)ret;
}
struct Denoted* get_denotation_prototype()
{
	struct Denotation_Prototype *ret;
	ret=malloc(sizeof(struct Denotation_Prototype));
	ret->denotation=DT_Prototype;
	ret->specifier=TS_NONE;
	ret->storage_class=SC_NONE;
	ret->constraint=TC_NONE;
	ret->sign=TSIGN_NONE;
	ret->function_specifier=FS_None;
	ret->size=0;
	ret->is_const=ret->is_volatile=0;

	return (struct Denoted*)ret;
}
struct Denoted* extract_denoted(struct Denoted_Base *base,struct Denotation_Prototype *prototype,char allow_abstract)
{
	if(base->type->specifier==TS_FUNC)
	{
		if(base->id==NULL && !allow_abstract)
		{
			return get_denoted_error(get_denoted_function(NULL,((struct Type_Function*)base->type)->return_type,prototype->function_specifier));
		}else
		{
			return get_denoted_function(base->id,((struct Type_Function*)base->type)->return_type,prototype->function_specifier);
		}
	}else if(prototype->storage_class==SC_TYPEDEF)
	{
		if(base->id==NULL && !allow_abstract)
		{
			return get_denoted_error(get_denoted_typedef(base));
		}else
		{
			return get_denoted_typedef(base);
		}
	}else
	{
		if(base->id==NULL && !allow_abstract)
		{
			return get_denoted_error(get_denoted_object(base->id,prototype->storage_class,base->type));
		}else
		{
			return get_denoted_object(base->id,prototype->storage_class,base->type);
		}
	}
}
#endif
