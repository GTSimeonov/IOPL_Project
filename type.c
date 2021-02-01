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

	if(prototype->constraint!=TC_NONE || prototype->size!=TSIGN_NONE || (prototype->specifier!=TS_ENUM && prototype->specifier!=TS_STRUCT))
	{
		return (struct Type*)get_type_error((struct Type*)ret);
	}else
	{
		return (struct Type*)ret;
	}
}
struct Struct_Union* get_struct_union_base(enum Type_Specifier struct_or_union)
{
	struct Struct_Union *ret;
	ret=malloc(sizeof(struct Struct_Union));
	ret->specifier=struct_or_union;
	ret->members=malloc(sizeof(struct Queue));
	Queue_Init(ret->members);

	ret->inner_namespace=get_scope(NULL);

	return ret;
}
struct Enum *get_enum_base()
{
	struct Enum *ret;
	ret=malloc(sizeof(struct Enum));
	ret->specifier=TS_ENUM;
	ret->number_of_constants=0;
	ret->consts=NULL;

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
	ret->number_of_elements=evaluate_const_expression_integer(number_of_elements);
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
struct Type* get_type_bitfield(struct Type* base,size_t number_of_bits)
{
	struct Type_Bit_Field *ret;
	ret=malloc(sizeof(struct Type_Bit_Field));
	ret->specifier=TS_BITFIELD;
	ret->number_of_bits=number_of_bits;
	ret->base=base;

	return (struct Type*)ret;
}
struct Type* get_function_type(struct Type* return_type,struct Queue *parameters)
{
	struct Type_Function *ret;
	ret=malloc(sizeof(struct Type_Function));
	ret->specifier=TS_FUNC;
	ret->return_type=return_type;
	ret->parameters=parameters;

	return (struct Type*)ret;
}
#endif
