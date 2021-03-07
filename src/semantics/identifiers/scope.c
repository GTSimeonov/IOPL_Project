#ifndef WONKY_SCOPE_C
#define WONKY_SCOPE_C WONKY_SCOPE_C
#include "scope.h"


struct Scope* get_normal_scope(struct Scope *parent,enum Scope_Type type)
{
	struct Normal_Scope *ret;
	assert(type==BLOCK_SCOPE || type==FILE_SCOPE || type==FUNCTION_PROTOTYPE_SCOPE);
	ret=malloc(sizeof(struct Normal_Scope));
	ret->type=type;

	assert((type!=FILE_SCOPE) || parent==NULL);
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
	Map_Map(&scope->ordinary,delete_denoted_with_no_linkage_wrapper);
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

void Scope_Push(struct Scope *scope,struct Denoted *declarator,struct Translation_Data *translation_data)
{

	switch(declarator->denotation)
	{
		case DT_Label:
			/*perhaps lables should be denoted*/
			assert(0);
		case DT_Function:
			push_function(scope,translation_data,AS_DENOTED_FUNCTION(declarator));
			break;
		case DT_Object:
			push_object(scope,translation_data,AS_DENOTED_OBJECT_PTR(declarator));
			break;
		case DT_Typedef:
			push_typedef(scope,translation_data,AS_DENOTED_TYPEDEF(declarator));
			break;
		case DT_Enum_Constant:
			push_denoted_enum_constant(scope,translation_data,AS_DENOTED_ENUM_CONST(declarator));
			break;
		case DT_Struct_Union_Member:
			assert(0);
			break;
		case DT_Enum:
			push_denoted_enum_tag(scope,translation_data,AS_DENOTED_ENUM(declarator));
			break;
		case DT_Struct_Union_Tag:
			push_denoted_struct_union_tag(scope,translation_data,AS_DENOTED_STRUCT_UNION(declarator));
			break;
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
#define CHECK_AND_PUSH(thing,scope) Map_Check_And_Push(scope,thing->id->data,thing->id->data_size,thing)

void push_object(struct Scope *current,struct Translation_Data *translation_data,struct Denoted_Object *denoted_object)
{
#define PO_ERROR(msg) delete_denoted_object(denoted_object);push_translation_error(msg,translation_data);return;

	struct Denoted_Object *hold_object;
	struct Linkage *linkage;
	resolve_object_linkage(current,translation_data,denoted_object);
	if(has_new_errors(translation_data))
		{PO_ERROR("in declaration");}

	hold_object=CHECK_AND_PUSH(denoted_object,&AS_NORMAL_SCOPE(current)->ordinary);
	if(hold_object!=NULL && hold_object->linkage==LINKAGE_NONE)
			{PO_ERROR("redeclaration of identifier");}

	if(denoted_object->linkage==LINKAGE_NONE)
	{
		if(hold_object!=NULL)
			{PO_ERROR("redeclaration of identifier");}
	}else if(denoted_object->linkage==LINKAGE_EXTERNAL || denoted_object->linkage==LINKAGE_INTERNAL)
	{
		linkage=(denoted_object->linkage==LINKAGE_EXTERNAL?translation_data->external_linkage:translation_data->internal_linkage);
		hold_object=CHECK_AND_PUSH(denoted_object,&linkage->ids);
		if(hold_object!=NULL)
		{
			if(hold_object->denotation!=DT_Object)
				{PO_ERROR("linking an object to a function");}
			if(!types_are_identical(hold_object->object->type,denoted_object->object->type))
				{PO_ERROR("linking an objects with mismatching types");}
			if(denoted_object->initializer!=NULL)
			{
				if(hold_object->initializer==NULL)
					hold_object->initializer=denoted_object->initializer;
				else
					{PO_ERROR("two initialisers for static storage duration object");}
				//delete_denoted_object(denoted_object);
			}
		}
	}else
	{
		assert(0);
	}

#undef PO_ERROR
}
void push_function(struct Scope *current,struct Translation_Data *translation_data,struct Denoted_Function *denoted_function)
{
#define PF_ERROR(msg) delete_denoted_function(denoted_function);push_translation_error(msg,translation_data);return;

	struct Denoted_Function *hold_function;
	struct Linkage *linkage;
	resolve_function_linkage(current,translation_data,denoted_function);
	if(has_new_errors(translation_data))
		{PF_ERROR("in declaration");}

	if(denoted_function->linkage==LINKAGE_NONE)
	{
		hold_function=CHECK_AND_PUSH(denoted_function,&AS_NORMAL_SCOPE(current)->ordinary);
		if(hold_function!=NULL)
			{PF_ERROR("id with function type without linkage");}
	}else if(denoted_function->linkage==LINKAGE_EXTERNAL || denoted_function->linkage==LINKAGE_INTERNAL)
	{
		linkage=(denoted_function->linkage==LINKAGE_EXTERNAL?translation_data->external_linkage:translation_data->internal_linkage);
		hold_function=CHECK_AND_PUSH(denoted_function,&linkage->ids);
		if(hold_function!=NULL)
		{
			if(hold_function->denotation!=DT_Function)
				{PF_ERROR("linking an function to a object");}
			if(!types_are_identical(hold_function->type,denoted_function->type))
				{PF_ERROR("linking functions with mismatching types");}
			if(hold_function->body!=NULL && denoted_function->body!=NULL)
				{PF_ERROR("redefinition of a function");}
			if(denoted_function->body!=NULL)
			{
				hold_function->body=denoted_function->body;
				denoted_function->body=NULL;
		//		delete_denoted_function(denoted_function);
			}
		}
	}else
	{
		assert(0);
	}

#undef PF_ERROR
}
void push_typedef(struct Scope *current,struct Translation_Data *translation_data,struct Denoted_Typedef *denoted_typedef)
{
	struct Denoted *hold_denotated;
	hold_denotated=CHECK_AND_PUSH(denoted_typedef,&AS_NORMAL_SCOPE(current)->ordinary);
	if(hold_denotated)
	{
		delete_denoted_typedef(denoted_typedef);
		push_translation_error("redefinition of identifier",translation_data);
	}
}
void push_denoted_enum_tag(struct Scope *current,struct Translation_Data *translation_data,struct Denoted_Enum *denoted_enum)
{
	struct Denoted *hold_denotated;
	hold_denotated=CHECK_AND_PUSH(denoted_enum,&AS_NORMAL_SCOPE(current)->tags);
	if(hold_denotated)
	{
		delete_denoted_enum(denoted_enum);
		push_translation_error("redefinition of tag",translation_data);
	}
}
void push_denoted_enum_constant(struct Scope *current,struct Translation_Data *translation_data,struct Denoted_Enum_Const *denoted_enum_constant)
{
	struct Denoted *hold_denotated;
	hold_denotated=CHECK_AND_PUSH(denoted_enum_constant,&AS_NORMAL_SCOPE(current)->ordinary);
	if(hold_denotated)
	{
		delete_denoted_enum_constant(denoted_enum_constant);
		push_translation_error("redefinition of identifier",translation_data);
	}
}
void push_denoted_struct_union_tag(struct Scope *current,struct Translation_Data *translation_data,struct Denoted_Struct_Union *denoted_struct_union)
{
	struct Denoted *hold_denotated;
	hold_denotated=CHECK_AND_PUSH(denoted_struct_union,&AS_NORMAL_SCOPE(current)->tags);
	if(hold_denotated)
	{
		delete_denoted_struct_union(denoted_struct_union);
		push_translation_error("redefinition of tag",translation_data);
	}
}
#undef CHECK_AND_PUSH
#endif
