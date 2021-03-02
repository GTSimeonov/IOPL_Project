#ifndef GCC_DENOTED_C
#define GCC_DENOTED_C GCC_DENOTED_C
#include <denoted.h>

struct Denoted* get_denoted_error(struct Denoted *error)
{
	struct Denoted_Error *ret;
	ret=malloc(sizeof(struct Denoted_Error));
	ret->denotation=DT_Error;
	ret->error=error;

	return (struct Denoted*)ret;
}
struct Denoted_Base* get_denoted_base(struct Denotation_Prototype *prototype)
{
	struct Denoted_Base *ret;
	ret=malloc(sizeof(struct Denoted_Base));
	ret->denotation=prototype->denotation;
	ret->id=NULL;
	ret->type=prototype->type;

	return ret;
}
struct Denoted* get_denoted_function(struct token *id,struct Type *return_type,enum Function_Specifier fs)
{
	struct Denoted_Function *ret;
	ret=malloc(sizeof(struct Denoted_Function));
	ret->denotation=DT_Function;
	ret->linkage=LINKAGE_NONE;
	ret->id=id;
	ret->type=return_type;
	ret->function_specifier=fs;
	ret->body=NULL;

	return (struct Denoted*)ret;
}
struct Denoted* get_denoted_object(struct token *id, enum Storage_Class_Specifier sc,struct Type *type,struct AST *initializer)
{
	struct Denoted_Object *ret;
	ret=malloc(sizeof(struct Denoted_Object));
	ret->denotation=DT_Object;
	ret->linkage=LINKAGE_NONE;
	ret->id=id;

	ret->object=malloc(sizeof(struct Object));
	ret->object->type=type;
	ret->object->location=NULL;
	ret->object->storage_class=sc;

	ret->initializer=initializer;

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
struct Denoted* get_denotation_prototype(struct Map *types)
{
	struct Denotation_Prototype *ret;
	ret=malloc(sizeof(struct Denotation_Prototype));
	ret->denotation=DT_Prototype;
	ret->type=NULL;
	ret->node=types;
	ret->storage_class=SCS_NONE;
	ret->specifier=TS_NONE;
	ret->constraint=TC_NONE;
	ret->sign=TSIGN_NONE;
	ret->function_specifier=FS_None;
	ret->struct_union=NULL;
	ret->enumerator=NULL;
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
			struct Denoted_Function *hold_denoted_function;
			hold_denoted_function=(struct Denoted_Function*)get_denoted_function(base->id,base->type,prototype->function_specifier);
			/*hack*/
			switch(prototype->storage_class)
			{
				case SCS_NONE:
					hold_denoted_function->linkage=LINKAGE_NONE;
					break;
				case SCS_EXTERN:
					hold_denoted_function->linkage=LINKAGE_EXTERNAL;
					break;
				case SCS_STATIC:
					hold_denoted_function->linkage=LINKAGE_INTERNAL;
					break;

			}
			return (struct Denoted*)hold_denoted_function;
		}
	}else if(prototype->storage_class==SCS_TYPEDEF)
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
			return get_denoted_error(get_denoted_object(base->id,prototype->storage_class,base->type,NULL));
		}else
		{
			return get_denoted_object(base->id,prototype->storage_class,base->type,NULL);
		}
	}
}

void delete_denoted(struct Denoted *denoted)
{
	switch(denoted->denotation)
	{
		case DT_Label:
			free(denoted);
			break;
		case DT_Object:
			delete_denoted_object((struct Denoted_Object*)denoted);
			break;
		case DT_Typedef:
			delete_denoted_typedef((struct Denoted_Typedef*)denoted);
			break;
		case DT_Function:
			delete_denoted_function((struct Denoted_Function*)denoted);
			break;
		case DT_Enum:
			delete_denoted_enum((struct Denoted_Enum*)denoted);
			break;
		case DT_Enum_Constant:
			delete_denoted_enum_constant((struct Denoted_Enum_Const*)denoted);
			break;
		case DT_Struct_Union_Tag:
			delete_denoted_struct_union((struct Denoted_Struct_Union*)denoted);
			break;
		case DT_Error:
			delete_denoted_error((struct Denoted_Error*)denoted);
			break;
		case DT_Prototype:
		default:
			assert(0);
	}
}
void delete_denoted_error(struct Denoted_Error *error)
{
	if(error->error!=NULL)
		delete_denoted(error->error);
	free(error);
}
void delete_denoted_function(struct Denoted_Function *function)
{
	if(function->id!=NULL)
		free(function->id);
	if(function->body!=NULL)
		delete_ast_compound_statement(function->body);
	free(function);
}
void delete_denoted_object(struct Denoted_Object *object)
{
	if(object->id!=NULL)
		free(object->id);
	if(object->object!=NULL)
		delete_object(object->object);
	if(object->initializer!=NULL)
		delete_ast(object->initializer);
	free(object);
}
void delete_denoted_typedef(struct Denoted_Typedef *typedefed)
{
	if(typedefed->id!=NULL)
		free(typedefed->id);
	free(typedefed);
}
void delete_denoted_enum(struct Denoted_Enum *enumeration)
{
	if(enumeration->id!=NULL)
		free(enumeration->id);
	if(enumeration->enumeration!=NULL)
		delete_enum(enumeration->enumeration);
	free(enumeration);
}
void delete_denoted_enum_constant(struct Denoted_Enum_Const *enum_const)
{
	if(enum_const->id!=NULL)
		free(enum_const->id);
	if(enum_const->expression!=NULL)
		delete_ast(enum_const->expression);
	free(enum_const);
}
void delete_denoted_struct_union(struct Denoted_Struct_Union *su)
{
	if(su->id!=NULL)
		free(su->id);
	if(su->struct_union!=NULL)
		delete_struct_union(su->struct_union);
	free(su);
}
void delete_object(struct Object *object)
{
	if(object->location!=NULL)
		delete_location(object->location);
	free(object);
}
void delete_denoted_prototype(struct Denotation_Prototype *prototype)
{
	free(prototype);
}
void delete_denoted_base(struct Denoted_Base *base)
{
	free(base);
}
void delete_denoted_wrapper(void *denoted)
{
	delete_denoted(denoted);
}
void delete_denoted_with_no_linkage_wrapper(void *denoted)
{
	if( ((struct Denoted*)denoted)->denotation == DT_Object )
	{
		if( AS_DENOTED_OBJECT_PTR(denoted)->linkage!=LINKAGE_NONE )
			return;
	}
	delete_denoted(denoted);
}




#endif
