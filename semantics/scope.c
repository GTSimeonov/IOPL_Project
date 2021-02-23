#ifndef GCC_SCOPE_C
#define GCC_SCOPE_C GCC_SCOPE_C
#include "scope.h"


struct Scope* get_normal_scope(struct Scope *parent,enum Scope_Type type)
{
	struct Normal_Scope *ret;
	assert(type==BLOCK_SCOPE || type==EXTERN_SCOPE || type==FILE_SCOPE || type==FUNCTION_PROTOTYPE_SCOPE);
	ret=malloc(sizeof(struct Normal_Scope));
	ret->type=type;

	assert((type!=EXTERN_SCOPE) || parent==NULL);
	assert((type!=FILE_SCOPE) || parent->type==EXTERN_SCOPE);
	ret->parent=parent;

	Map_Init(&ret->tags);
	Map_Init(&ret->ordinary);

	return (struct Scope*)ret;

}
struct Scope* get_function_scope(struct Scope *parent)
{
	struct Function_Scope *ret;
	assert(parent!=NULL && parent->type==FILE_SCOPE);

	ret=malloc(sizeof(struct Function_Scope));
	ret->type=FUNCTION_SCOPE;
	ret->parent=parent;

	Map_Init(&ret->labels);


	return (struct Scope*)ret;
}

void delete_normal_scope(struct Normal_Scope *scope)
{
	Map_Map(&scope->tags,delete_denoted_wrapper);
	Map_Destroy(&scope->tags);
	Map_Map(&scope->ordinary,delete_denoted_wrapper);
	Map_Destroy(&scope->ordinary);
	free(scope);
}

void delete_function_scope(struct Function_Scope *scope)
{
	Map_Map(&scope->labels,delete_denoted_wrapper);
	Map_Destroy(&scope->labels);
	free(scope);
}

void delete_scope(struct Scope *scope)
{
	switch(scope->type)
	{
		case BLOCK_SCOPE:
		case FILE_SCOPE:
		case EXTERN_SCOPE:
		case FUNCTION_PROTOTYPE_SCOPE:
			delete_normal_scope((struct Normal_Scope*)scope);
			break;
		case FUNCTION_SCOPE:
			delete_function_scope((struct Function_Scope*)scope);
		default:
			assert(0);
	}
}


void* check_label(struct Scope *current,struct token *id)
{
	void *hold;
	hold=NULL;
	while(current!=NULL && current->type!=FUNCTION_SCOPE)
	{
		current=current->parent;
	}

	if(current!=NULL)
	{
		hold=Map_Check(&((struct Function_Scope*)current)->labels,id->data,id->data_size);
	}
	return hold;
}

struct Denoted* check_tag(struct Scope *current,struct token *id)
{
	void *hold;
	hold=NULL;
	while(current!=NULL && hold==NULL && current->type!=FUNCTION_SCOPE)
	{
		hold=Map_Check(&((struct Normal_Scope*)current)->tags,id->data,id->data_size);
		current=current->parent;
	}
	return hold;
}
void* check_ordinary(struct Scope *current,struct token *id)
{
	void *hold;
	hold=NULL;
	while(current!=NULL && hold==NULL)
	{
		hold=Map_Check(&((struct Normal_Scope*)current)->ordinary,id->data,id->data_size);
		current=current->parent;
	}
	return hold;
}

void Scope_Push(struct Scope *scope,struct Denoted *declarator)
{
	switch(declarator->denotation)
	{
		/*
		case DT_Label:
			if(check_label(scope,((struct Denoted_Object
			return 1;
			*/
		case DT_Function:
		case DT_Typedef:
		case DT_Object:
		case DT_Enum_Constant:
		case DT_Struct_Union_Member:
			push_ordinary(scope,((struct Denoted_Object*)declarator)->id,declarator);
			return;
		case DT_Enum:
		case DT_Struct_Union_Tag:
			push_tag(scope,((struct Denoted_Object*)declarator)->id,declarator);
			return;
	}
}
char check_if_typedefed(struct Scope* scope,struct token *id)
{
	struct Denoted *hold;
	hold=check_ordinary(scope,id);
	if(hold==NULL || hold->denotation!=DT_Typedef)
		return 0;
	else
		return 1;

}
void push_tag(struct Scope *current,struct token *id,struct Denoted *denot)
{
	/*TODO fix this shit*/
	assert(current->type!=FUNCTION_SCOPE);
	assert(denot!=NULL);
	Map_Push(&((struct Normal_Scope*)current)->tags,id->data,id->data_size,denot);
}
void push_ordinary(struct Scope *current,struct token *id,struct Denoted *denot)
{
	assert(current->type!=FUNCTION_SCOPE);
	assert(denot!=NULL);
	Map_Push(&((struct Normal_Scope*)current)->ordinary,id->data,id->data_size,denot);
}
#endif
