#ifndef GCC_TYPE_C 
#define GCC_TYPE_C GCC_TYPE_C
#include "type.h"



struct Type* type_check_and_push(struct Type *type,struct Map *base,size_t struct_size)
{
	struct Map *hold_node;
	hold_node=Map_Check_And_Get(base,type,struct_size);

	if(hold_node==NULL)
	{
		type->node=Map_Push_And_Get(base,type,struct_size,type);
		return type;
	}else
	{
		free(type);
		return (struct Type*)hold_node->ID;
	}
}

struct Type* get_type_error(struct Type *type)
{
	struct Type_Error *ret;

	ret=calloc(1,sizeof(struct Type_Error));
	ret->specifier=TS_ERROR;
	ret->error=type;
	ret=(struct Type_Error*)type_check_and_push((struct Type*)ret,type->node,sizeof(struct Type_Error));
	return (struct Type*)ret;
	
}
struct Type* get_struct_union_type(struct Denotation_Prototype *prototype)
{
	struct Type_Struct_Union *ret;
	
	assert(prototype->denotation=DT_Prototype);
	prototype->denotation=DT_Object;

	ret=calloc(1,sizeof(struct Type_Struct_Union));
	ret->specifier=prototype->specifier;
	ret->struct_union=prototype->struct_union;

	ret->is_const=prototype->is_const;
	ret->is_volatile=prototype->is_volatile;

	ret=(struct Type_Struct_Union*)type_check_and_push((struct Type*)ret,prototype->node,sizeof(struct Type_Struct_Union));
	if(prototype->constraint!=TC_NONE || prototype->sign!=TSIGN_NONE || (prototype->specifier!=TS_UNION && prototype->specifier!=TS_STRUCT))
	{
		return get_type_error((struct Type*)ret);
	}else
	{
		return (struct Type*)ret;
	}
}
struct Struct_Union* get_struct_union_base(struct Scope *scope ,enum Type_Specifier struct_or_union)
{
	struct Struct_Union *ret;


	ret=calloc(1,sizeof(struct Struct_Union));
	ret->specifier=struct_or_union;
	ret->members=malloc(sizeof(struct Queue));
	Queue_Init(ret->members);

	ret->inner_namespace=(struct Normal_Scope*)get_normal_scope(scope,BLOCK_SCOPE);
	ret->is_finished=0;

	return ret;
}
struct Enum *get_enum_base()
{
	struct Enum *ret;
	ret=malloc(sizeof(struct Enum));
	ret->specifier=TS_ENUM;
	ret->consts=malloc(sizeof(struct Queue));
	Queue_Init(ret->consts);

	ret->is_finished=0;

	return ret;
}
struct Type* get_basic_type(struct Denotation_Prototype *prototype)
{
	struct Type_Basic *ret;
	ret=calloc(1,sizeof(struct Type_Basic));


	assert(prototype->denotation=DT_Prototype);
	prototype->denotation=DT_Object;

	ret->size=prototype->size;
	ret->is_const=prototype->is_const;
	ret->is_volatile=prototype->is_volatile;
	ret->constraint=prototype->constraint;
	ret->sign=prototype->sign;
	

	if(prototype->specifier==TS_NONE)
	{
		ret->specifier=TS_INT;
	}else
	{
		ret->specifier=prototype->specifier;

	}

	ret=(struct Type_Basic*)type_check_and_push((struct Type*)ret,prototype->node,sizeof(struct Type_Basic));

	switch(ret->specifier)
	{
		case TS_DOUBLE:
			if(ret->constraint==TC_LONG_LONG 
				|| prototype->constraint==TC_SHORT
				|| prototype->sign!=TSIGN_NONE)
			{
				return get_type_error((struct Type*)ret);
			}
			break;
		case TS_CHAR:
			if(prototype->constraint!=TC_NONE)
			{
				return get_type_error((struct Type*)ret);
			}
			break;
		case TS_INT:
			break;
		default:
			if(prototype->constraint!=TC_NONE || prototype->sign!=TSIGN_NONE)
			{
				return get_type_error((struct Type*)ret);
			}
			
	}
	return (struct Type*)ret;

}
struct Type* get_pointer_type(struct Type *points_to,char is_const,char is_volatile)
{
	struct Type_Pointer *ret;
	ret=calloc(1,sizeof(struct Type_Pointer));
	ret->specifier=TS_POINTER;
	ret->size=PTR_SIZE;
	ret->points_to=points_to;
	ret->is_const=is_const;
	ret->is_volatile=is_volatile;

	ret=(struct Type_Pointer*)type_check_and_push((struct Type*)ret,points_to->node,sizeof(struct Type_Pointer));
	return (struct Type*)ret;

}
struct Type* get_array_type(struct Type *array_of,struct AST* number_of_elements)
{
	struct Type_Array *ret;
	ret=calloc(1,sizeof(struct Type_Array));
	ret->specifier=TS_ARRAY;
	ret->size=0;
	if(number_of_elements!=NULL)
	{
		ret->number_of_elements=evaluate_const_expression_integer(number_of_elements);
		delete_ast(number_of_elements);
	}else
	{
		ret->number_of_elements=0;
	}
	ret->is_array_of=array_of;
	ret=(struct Type_Array*)type_check_and_push((struct Type*)ret,array_of->node,sizeof(struct Type_Array));
	return (struct Type*)ret;


}
struct Type* get_enum_type(struct Denotation_Prototype *prototype)
{
	struct Type_Enum *ret;

	assert(prototype->denotation=DT_Prototype);
	prototype->denotation=DT_Object;

	ret=calloc(1,sizeof(struct Type_Enum));
	ret->specifier=TS_ENUM;
	ret->enumeration=prototype->enumerator;
	ret->is_const=prototype->is_const;
	ret->is_volatile=prototype->is_volatile;

	ret=(struct Type_Enum*)type_check_and_push((struct Type*)ret,prototype->node,sizeof(struct Type_Enum));
	if(prototype->sign!=TSIGN_NONE || prototype->constraint!=TC_NONE)
	{
		return get_type_error((struct Type*)ret);
	}
	return (struct Type*)ret;
}
struct Type* get_type_bitfield(struct Type *base,struct AST* number_of_bits)
{
	struct Type_Bit_Field *ret;
	ret=calloc(1,sizeof(struct Type_Bit_Field));
	ret->specifier=TS_BITFIELD;

	assert(number_of_bits!=NULL);
	ret->number_of_bits=evaluate_const_expression_integer(number_of_bits);
	delete_ast(number_of_bits);

	ret->base=base;

	ret=(struct Type_Bit_Field*)type_check_and_push((struct Type*)ret,base->node,sizeof(struct Type_Bit_Field));
	return (struct Type*)ret;
}

struct Type* get_function_type(struct Type *return_type,struct Queue *parameters,struct Normal_Scope* function_prototype_scope)
{
	struct Type_Function *ret;
	size_t i;
	struct Map *hold_node;

	ret=calloc(1,sizeof(struct Type_Function));
	ret->specifier=TS_FUNC;

	ret->return_type=return_type;


	ret->function_prototype_scope=function_prototype_scope;


	ret->number_of_arguments=parameters->size;
	ret->arguments=malloc(sizeof(struct Denoted_Object*)*ret->number_of_arguments);
	for(i=0;parameters->size>0;++i)
	{
		ret->arguments[i]=(struct Denoted_Object*)Queue_Pop(parameters);
	}
	free(parameters);
	ret=(struct Type_Function*)type_check_and_push((struct Type*)ret,return_type->node,sizeof(struct Type_Function));

}
char is_type(struct Translation_Data *translation_data,struct Scope *scope)
{
	struct token *hold;
	struct Denoted *thing;

	hold=translation_data->tokens->first->data;

	switch(hold->type)
	{
		case KW_ID:
			thing=check_ordinary(scope,hold);
			if(thing!=NULL && thing->denotation==DT_Typedef) 
				return 1;
			else return 0;
		case KW_CONST:
		case KW_VOLATILE:
		case KW_INT:
		case KW_VOID:
		case KW_CHAR:
		case KW_DOUBLE:
		case KW_FLOAT:
		case KW_LONG:
		case KW_SHORT:
		case KW_EXTERN:
		case KW_STATIC:
		case KW_TYPEDEF:
		case KW_STRUCT:
		case KW_UNION:
		case KW_ENUM:
			return 1;
		default:
			return 0;

	}
}
size_t get_type_size(struct Type *type)
{
	switch(type->specifier)
	{
		case TS_VOID:
			return 0;
		case TS_CHAR:
			return 1;
		case TS_INT:
			return INT_SIZE;
		case TS_FLOAT:
			return FLOAT_SIZE;
		case TS_DOUBLE:
			return FLOAT_SIZE*2;
		case TS_STRUCT:
			return ((struct Type_Struct_Union*)type)->struct_union->size;
		case TS_ENUM:
			return INT_SIZE;
		case TS_UNION:
			return ((struct Type_Struct_Union*)type)->struct_union->size;
		case TS_POINTER:
			return PTR_SIZE;
		case TS_ARRAY:
			return ((struct Type_Array*)type)->size;
		case TS_FUNC:
			return 0;
		case TS_BITFIELD:
			return ((struct Type_Bit_Field*)type)->number_of_bits;
		case TS_NONE:
			return 0;
		case TS_ERROR:
			return 0;

	}	
}


void delete_enum(struct Enum *enumeration)
{
	while(enumeration->consts->size>0)
		Queue_Pop(enumeration->consts);
	free(enumeration);
}
void delete_struct_union(struct Struct_Union *su)
{
	delete_scope((struct Scope*)su->inner_namespace);
	while(su->members->size>0)
		Queue_Pop(su->members);

	free(su);
}

void delete_type(void *type)
{
	if(((struct Type*)type)->specifier!=TS_FUNC)
	{
		free(type);
	}else
	{
		delete_normal_scope(AS_TYPE_FUNC_PTR(type)->function_prototype_scope);
		free(AS_TYPE_FUNC_PTR(type)->arguments);
		free(type);
	}
}
#endif
