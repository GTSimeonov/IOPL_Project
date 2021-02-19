#ifndef GCC_TYPE_C 
#define GCC_TYPE_C GCC_TYPE_C
#include "type.h"



struct Type* get_type_error(struct Type* error)
{
	struct Type_Error *ret;
	ret=malloc(sizeof(struct Type_Error));
	ret->specifier=TS_ERROR;
	ret->error=error;
	
	return (struct Type*)ret;
}
/*could return error */
struct Type* get_struct_union_type(struct Denotation_Prototype *prototype)
{
	struct Type_Struct_Union *ret;
	
	ret=malloc(sizeof(struct Type_Struct_Union));
	ret->specifier=prototype->specifier;
	ret->struct_union=prototype->struct_union;

	ret->is_const=prototype->is_const;
	ret->is_volatile=prototype->is_volatile;

	if(prototype->constraint!=TC_NONE || prototype->sign!=TSIGN_NONE || (prototype->specifier!=TS_UNION && prototype->specifier!=TS_STRUCT))
	{
		return (struct Type*)get_type_error((struct Type*)ret);
	}else
	{
		return (struct Type*)ret;
	}
}
struct Struct_Union* get_struct_union_base(struct Scope *scope ,enum Type_Specifier struct_or_union)
{
	struct Struct_Union *ret;
	ret=malloc(sizeof(struct Struct_Union));
	ret->specifier=struct_or_union;
	ret->members=malloc(sizeof(struct Queue));
	Queue_Init(ret->members);

	ret->inner_namespace=get_scope(scope);
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
/*could return error*/
struct Type* get_basic_type(struct Denotation_Prototype *prototype)
{
	struct Type_Basic *ret;
	ret=malloc(sizeof(struct Type_Basic));

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

	switch(prototype->specifier)
	{
		case TS_DOUBLE:
			if(prototype->constraint==TC_LONG_LONG 
				|| prototype->constraint==TC_SHORT
				|| prototype->sign!=TSIGN_NONE)
			{
				return (struct Type*)get_type_error((struct Type*)ret);
			}
			break;
		case TS_CHAR:
			if(prototype->constraint!=TC_NONE)
			{
				return (struct Type*)get_type_error((struct Type*)ret);
			}
			break;
		case TS_INT:
			break;
		default:
			if(prototype->constraint!=TC_NONE || prototype->sign!=TSIGN_NONE)
			{
				return (struct Type*)get_type_error((struct Type*)ret);
			}
			
	}
	return (struct Type*)ret;

}
struct Type* get_pointer_type(struct Type* points_to)
{
	struct Type_Pointer *ret;
	ret=malloc(sizeof(struct Type_Pointer));
	ret->specifier=TS_POINTER;
	ret->size=PTR_SIZE;
	ret->points_to=points_to;
	ret->is_const=ret->is_volatile=0;
	return (struct Type*)ret;

}
struct Type* get_array_type(struct Type *is_array_of,struct AST* number_of_elements)
{
	struct Type_Array *ret;
	ret=malloc(sizeof(struct Type_Array));
	ret->specifier=TS_ARRAY;
	ret->size=0;
	if(number_of_elements!=NULL)
	{
		ret->number_of_elements=evaluate_const_expression_integer(number_of_elements);
	}else
	{
		ret->number_of_elements=0;
	}
	ret->expression=number_of_elements;
	ret->is_array_of=is_array_of;

	return (struct Type*)ret;
}
struct Type* get_enum_type(struct Denotation_Prototype *prototype)
{
	struct Type_Enum *ret;
	ret=malloc(sizeof(struct Type_Enum));
	ret->specifier=TS_ENUM;
	ret->enumeration=prototype->enumerator;
	ret->is_const=prototype->is_const;
	ret->is_volatile=prototype->is_volatile;
	if(prototype->sign!=TSIGN_NONE || prototype->constraint!=TC_NONE)
	{
		return get_type_error((struct Type*)ret);
	}
	return (struct Type*)ret;
}
struct Type* get_type_bitfield(struct Type* base,struct AST* number_of_bits)
{
	struct Type_Bit_Field *ret;
	ret=malloc(sizeof(struct Type_Bit_Field));
	ret->specifier=TS_BITFIELD;
	ret->expression=number_of_bits;
	ret->number_of_bits=evaluate_const_expression_integer(number_of_bits);
	ret->base=base;

	return (struct Type*)ret;
}
struct Type* get_function_type(struct Type* return_type,struct Queue *parameters,struct Scope* function_prototype_scope)
{
	struct Type_Function *ret;
	ret=malloc(sizeof(struct Type_Function));
	ret->specifier=TS_FUNC;
	ret->return_type=return_type;
	ret->parameters=parameters;
	ret->function_prototype_scope=function_prototype_scope;

	return (struct Type*)ret;
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
#endif
