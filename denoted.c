#ifndef GCC_DENOTED_C
#define GCC_DENOTED_C GCC_DENOTED_C


struct Denoted_Error* get_denoted_error(struct Denoted *error)
{
	struct Denoted_Error *ret;
	ret=malloc(sizeof(struct Denoted_Error));
	ret->denotation=DT_Error;
	ret->error=error;

	return error;
}
struct Denoted_Function* get_denoted_function(struct Type *type)
{
	struct Denoted_Function *ret;
	ret=malloc(sizeof(struct Denoted_Function));
	ret->denotation=DT_Function;
	ret->type=type;
	ret->body=NULL;

	return ret;
}
struct Denoted_Object* get_denoted_object(struct Type *type)
{
	struct Denoted_Object *ret;
	ret=malloc(sizeof(struct Denoted_Object));
	ret->denotation=DT_Object;
	ret->type=type;
	ret->location=NULL;

	return ret;
}

struct Denoted_Typedef* get_denoted_typedef(struct token* id,struct Type *typedefed)
{
	struct Denoted_Typedef *ret;
	ret=malloc(sizeof(struct Denoted_Typedef));
	ret->denotation=DT_Typedef;
	ret->type=typedefed;
	ret->id=id;

	return ret;

}
struct Denoted_Enum_Const* get_denoted_enum_const(struct token *id,struct Type_Enum *parent,int value)
{
	struct Denoted_Enum_Const *ret;
	ret=malloc(sizeof(struct Denoted_Enum_Const));
	ret->denotation=DT_Enum_Constant;
	ret->id=id;
	ret->parent=parent;
	ret->value=value;

	return ret;
	
}
struct Denoted_Struct_Union_Member* get_denoted_struct_union_member(struct token *id,struct Type *type,size_t offset)
{
	struct Denoted_Struct_Union_Member *ret;
	ret=malloc(sizeof(struct Denoted_Struct_Union_Member));
	ret->denotation=DT_Struct_Union_Member;
	ret->id=id;
	ret->type=type;
	ret->offset=offset;

	return ret;
}

#endif
