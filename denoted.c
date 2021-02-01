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
struct Denoted* get_denoted_function(struct Denotation_Prototype *prototype)
{
	struct Denoted_Function *ret;
	ret=malloc(sizeof(struct Denoted_Function));
	ret->denotation=DT_Function;
	ret->type=prototype->type;
	ret->function_specifier=prototype->function_specifier;
	ret->body=NULL;

	return (struct Denoted*)ret;
}
struct Denoted* get_denoted_object(struct token *id, struct Object *object)
{
	struct Denoted_Object *ret;
	ret=malloc(sizeof(struct Denoted_Object));
	ret->denotation=DT_Object;
	ret->id=id;
	ret->object=object;

	return (struct Denoted*)ret;
}

struct Denoted* get_denoted_typedef(struct Denotation_Prototype *prototype)
{
	struct Denoted_Typedef *ret;
	ret=malloc(sizeof(struct Denoted_Typedef));
	ret->denotation=DT_Typedef;
	ret->type=prototype->type;
	ret->id=prototype->id;

	return (struct Denoted*)ret;

}
struct Denoted* get_denoted_enum_const(struct token *id,struct Type_Enum *parent,int value)
{
	struct Denoted_Enum_Const *ret;
	ret=malloc(sizeof(struct Denoted_Enum_Const));
	ret->denotation=DT_Enum_Constant;
	ret->id=id;
	ret->parent=parent;
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
#endif
