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

char Scope_Push(struct Scope *scope,struct Denoted *declarator)
{
	return 0;
}
char check_if_typedefed(struct Scope* scope,struct token *id)
{
}
#endif
