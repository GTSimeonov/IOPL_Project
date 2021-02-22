#ifndef GCC_TYPE_C 
#define GCC_TYPE_C GCC_TYPE_C
#include "type.h"


void type_check_and_push_heavy(struct Type_Map_Pair *pair,struct Type *type)
{
	switch(type->specifier)
	{
		case TS_VOID:
		case TS_CHAR:
		case TS_INT:
		case TS_FLOAT:
		case TS_DOUBLE:
			type_check_and_push(pair,type,sizeof(struct Type_Basic));
			break;
		case TS_STRUCT:
		case TS_UNION:
			type_check_and_push(pair,type,sizeof(struct Type_Struct_Union));
			break;
		case TS_ENUM:
			type_check_and_push(pair,type,sizeof(struct Type_Enum));
			break;
		case TS_POINTER:
			type_check_and_push(pair,type,sizeof(struct Type_Pointer));
			break;
		case TS_ARRAY:
			type_check_and_push(pair,type,sizeof(struct Type_Array));
			break;
		case TS_FUNC:
			type_check_and_push(pair,type,sizeof(struct Type_Function));
			break;
		case TS_BITFIELD:
			type_check_and_push(pair,type,sizeof(struct Type_Bit_Field));
			break;
		case TS_ERROR:
			type_check_and_push(pair,type,sizeof(struct Type_Error));
			break;
		case TS_NONE:
		default:
			assert(0);
	}
}

void type_check_and_push(struct Type_Map_Pair *pair,struct Type *type,size_t struct_size)
{

	struct Map *hold_node;
	hold_node=Map_Check_And_Get(pair->node,type,struct_size);

	if(hold_node==NULL)
	{
		pair->type=type;
		pair->node=Map_Push_And_Get(pair->node,type,struct_size,pair->type);
	}else
	{
		free(type);
		pair->node=hold_node;
		pair->type=(struct Type*)hold_node->ID;
	}
}

void get_type_error(struct Type_Map_Pair *pair)
{
	struct Type_Error *ret;

	ret=malloc(sizeof(struct Type_Error));
	ret->specifier=TS_ERROR;
	ret->error=pair->type;

	type_check_and_push(pair,(struct Type*)ret,sizeof(struct Type_Error));
	
}
void get_struct_union_type(struct Denotation_Prototype *prototype)
{
	struct Type_Struct_Union *ret;
	
	assert(prototype->denotation=DT_Prototype);
	prototype->denotation=DT_Object;

	ret=malloc(sizeof(struct Type_Struct_Union));
	ret->specifier=prototype->specifier;
	ret->struct_union=prototype->struct_union;

	ret->is_const=prototype->is_const;
	ret->is_volatile=prototype->is_volatile;

	type_check_and_push(prototype->pair,(struct Type*)ret,sizeof(struct Type_Struct_Union));
	if(prototype->constraint!=TC_NONE || prototype->sign!=TSIGN_NONE || (prototype->specifier!=TS_UNION && prototype->specifier!=TS_STRUCT))
	{
		get_type_error(prototype->pair);
	}
}
struct Struct_Union* get_struct_union_base(struct Scope *scope ,enum Type_Specifier struct_or_union)
{
	struct Struct_Union *ret;


	ret=malloc(sizeof(struct Struct_Union));
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
void get_basic_type(struct Denotation_Prototype *prototype)
{
	struct Type_Basic *ret;
	ret=malloc(sizeof(struct Type_Basic));


	assert(prototype->denotation=DT_Prototype);
	prototype->denotation=DT_Object;

	ret->size=prototype->size;
	ret->is_const=prototype->is_const;
	ret->is_volatile=prototype->is_volatile;
	ret->constraint=prototype->constraint;
	ret->size=prototype->sign;
	

	if(prototype->specifier==TS_NONE)
	{
		ret->specifier=TS_INT;
	}else
	{
		ret->specifier=prototype->specifier;

	}

	type_check_and_push(prototype->pair,(struct Type*)ret,sizeof(struct Type_Basic));

	switch(prototype->pair->type->specifier)
	{
		case TS_DOUBLE:
			if(prototype->constraint==TC_LONG_LONG 
				|| prototype->constraint==TC_SHORT
				|| prototype->sign!=TSIGN_NONE)
			{
				get_type_error(prototype->pair);
			}
			break;
		case TS_CHAR:
			if(prototype->constraint!=TC_NONE)
			{
				get_type_error(prototype->pair);
			}
			break;
		case TS_INT:
			break;
		default:
			if(prototype->constraint!=TC_NONE || prototype->sign!=TSIGN_NONE)
			{
				get_type_error(prototype->pair);
			}
			
	}

}
void get_pointer_type(struct Type_Map_Pair *pair,char is_volatile,char is_constant)
{
	struct Type_Pointer *ret;
	ret=malloc(sizeof(struct Type_Pointer));
	ret->specifier=TS_POINTER;
	ret->size=PTR_SIZE;
	ret->points_to=pair->type;
	ret->is_const=is_constant;
	ret->is_volatile=is_volatile;

	type_check_and_push(pair,(struct Type*)ret,sizeof(struct Type_Pointer));

}
void get_array_type(struct Type_Map_Pair *pair,struct AST* number_of_elements)
{
	struct Type_Array *ret;
	ret=malloc(sizeof(struct Type_Array));
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
	ret->is_array_of=pair->type;
	type_check_and_push(pair,(struct Type*)ret,sizeof(struct Type_Array));

}
void get_enum_type(struct Denotation_Prototype *prototype)
{
	struct Type_Enum *ret;

	assert(prototype->denotation=DT_Prototype);
	prototype->denotation=DT_Object;

	ret=malloc(sizeof(struct Type_Enum));
	ret->specifier=TS_ENUM;
	ret->enumeration=prototype->enumerator;
	ret->is_const=prototype->is_const;
	ret->is_volatile=prototype->is_volatile;

	type_check_and_push(prototype->pair,(struct Type*)ret,sizeof(struct Type_Enum));
	if(prototype->sign!=TSIGN_NONE || prototype->constraint!=TC_NONE)
	{
		get_type_error(prototype->pair);
	}
}
void get_type_bitfield(struct Type_Map_Pair *pair,struct AST* number_of_bits)
{
	struct Type_Bit_Field *ret;
	ret=malloc(sizeof(struct Type_Bit_Field));
	ret->specifier=TS_BITFIELD;

	assert(number_of_bits!=NULL);
	ret->number_of_bits=evaluate_const_expression_integer(number_of_bits);
	delete_ast(number_of_bits);

	ret->base=pair->type;

	type_check_and_push(pair,(struct Type*)ret,sizeof(struct Type_Bit_Field));
}

void get_function_type(struct Type_Map_Pair *pair,struct Queue *parameters,struct Normal_Scope* function_prototype_scope)
{
	struct Type_Function *ret;
	size_t i;
	struct Map *hold_node;

	ret=malloc(sizeof(struct Type_Function));
	ret->specifier=TS_FUNC;

	ret->return_type=pair->type;


	ret->function_prototype_scope=function_prototype_scope;


	ret->number_of_arguments=parameters->size;
	ret->arguments=malloc(sizeof(struct Type*)*ret->number_of_arguments);
	for(i=0;parameters->size>0;++i)
	{
		ret->arguments[i]=(struct Type*)Queue_Pop(parameters);
	}
	type_check_and_push(pair,(struct Type*)ret,sizeof(struct Type_Function));

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

struct Type_Map_Pair* get_type_map_pair(struct Type *type,struct Map *types)
{
	struct Type_Map_Pair *ret;
	ret=malloc(sizeof(struct Type_Map_Pair));
	ret->type=type;
	ret->node=types;

	return ret;
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
#endif
