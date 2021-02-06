#ifndef GCC_SCOPE_C
#define GCC_SCOPE_C GCC_SCOPE_C
#include "scope.h"


struct Scope* get_scope(struct Scope *parent)
{
	struct Scope *ret;
	ret=malloc(sizeof(struct Scope));
	Map_Init(&ret->labels);
	Map_Init(&ret->tags);
	Map_Init(&ret->ordinary);
	ret->parent=parent;
	if(parent==NULL)
	{
		ret->location=get_global_location();
	}else
	{
		ret->location=(struct Location*)get_relative_location(parent->location,0);
	}
	return ret;
}

void* check_label(struct Scope *current,struct token *id)
{
	void *hold;
	hold=NULL;
	while(current!=NULL && hold==NULL)
	{
		hold=Map_Check(&current->labels,id->data,id->data_size);
		current=current->parent;
	}
	return hold;
}

struct Denoted* check_tag(struct Scope *current,struct token *id)
{
	void *hold;
	hold=NULL;
	while(current!=NULL && hold==NULL)
	{
		hold=Map_Check(&current->tags,id->data,id->data_size);
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
		hold=Map_Check(&current->ordinary,id->data,id->data_size);
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
	Map_Push(&current->tags,id->data,id->data_size,denot);
}
void push_ordinary(struct Scope *current,struct token *id,struct Denoted *denot)
{
	Map_Push(&current->ordinary,id->data,id->data_size,denot);
}
#endif
