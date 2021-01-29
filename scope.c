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

void* check_tag(struct Scope *current,struct token *id)
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

char Scope_Push(struct Scope *scope,struct Declarator *declarator)
{
	struct Type_Node *hold;
	hold=declarator->type->components.last->data;
	if(hold->is_typedef)
	{
		
		if(Map_Check(&scope->ordinary,declarator->id->data,declarator->id->data_size))
		{
			return 0;
		}else
		{
			Map_Push(&scope->ordinary,declarator->id->data,declarator->id->data_size,declarator->type);
			return 1;
		}
	}

	hold=declarator->type->components.first->data;
		
	switch(hold->type_specifier)
	{
		case TS_INT:
		case TS_CHAR:
		case TS_FLOAT:
		case TS_DOUBLE:
		case TS_ARRAY:
		case TS_POINTER:
			if(Map_Check(&scope->ordinary,declarator->id->data,declarator->id->data_size))
			{
				return 0;
			}else
			{
				Map_Push(&scope->ordinary,declarator->id->data,declarator->id->data_size,declarator->type);
				return 1;
			}
		case TS_STRUCT:
		case TS_UNION:
			if(Map_Check(&scope->tags,declarator->id->data,declarator->id->data_size))
			{
				return 0;
			}else
			{
				Map_Push(&scope->ordinary,declarator->id->data,declarator->id->data_size,declarator->type);
				Map_Push(&scope->tags,kkkkkkk
				return 1;
			}
	}
	//shouldnt be able to go here
	return 0;
}
char check_if_typedefed(struct Scope* scope,struct token *id)
{
	struct Type *hold;
	hold=check_ordinary(scope,id);
	if(hold)
	{
		return (((struct Type_Node*)(hold->components.last->data))->is_typedef);
	}else
	{
		return 0;
	}

}
#endif
