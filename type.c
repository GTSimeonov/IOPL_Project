#ifndef GCC_TYPE_C 
#define GCC_TYPE_C GCC_TYPE_C
#include "type.h"



struct Type_Error* get_type_error(struct Type* error)
{
	struct Type_Error *ret;
	ret=malloc(sizeof(struct Type_Error));
	ret->specifier=TS_ERROR;
	ret->error=error;
	
	return ret;
}
struct Object_Prototype* get_object_prototype()
{
	struct Type_Prototype *ret;

	ret=malloc(sizeof(struct Type_Prototype));
	ret->specifier=TS_NONE;
	ret->size=0;
	ret->is_const=ret->is_volatile=0;
	ret->storage_class=TSC_NONE;
	ret->constraint=TC_NONE;
	ret->sign=TSIGN_NONE;

	return ret;
}
/*could return error */
struct Type* get_struct_union_type(struct Denotation_Prototype *prototype,struct Struct_Union *base)
{
	struct Type_Struct_Union *ret;
	
	ret=malloc(sizeof(struct Type_Struct_Union));
	ret->specifier=ts;
	ret->size=prototype->size;

	ret->struct_union=base;

	ret->inner_namespace=get_scope(NULL);
	ret->id=id;
	ret->is_const=prototype->is_const;
	ret->is_volatile=prototype->is_volatile;
	ret->storage_class=prototype->storage_class;

	if(prototype->constraint!=TC_NONE || prototype->size!=TSIGN_NONE)
	{
		return (struct Type*)get_type_error(ret);
	}else
	{
		return (struct Type*)ret;
	}
}
struct Struct_Union* get_struct_union_base()
{
	struct Struct_Union *ret;
	ret=malloc(sizeof(struct Struct_Union));
	ret->number_of_members=0;
	ret->members=NULL;
	ret->inner_namespace=NULL;

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
	ret->storage_class=prototype->storage_class;
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
				return (struct Type*)get_type_error(ret);
			}
			break;
		case TS_CHAR:
			if(prototype->constraint!=TC_NONE)
			{
				return (struct Type*)get_type_error(ret);
			}
			break;
		default:
			if(prototype->constraint!=TC_NONE || prototype->sign!=TSIGN_NONE)
			{
				return (struct Type*)get_type_error(ret);
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
	return ret;

}
struct Type* get_array_type(struct Type *is_array_of)
{
	struct Type_Array *ret;
	ret=malloc(sizeof(struct Type_Array));
	ret->specifier=TS_ARRAY;
	ret->size=0;
	ret->number_of_elements=0;

	return ret;
}
struct Type* get_enum_type(struct token *id)
{
	struct Type_Enum *ret;
	ret=malloc(sizeof(struct Type_Enum));
	ret->specifier=TS_ENUM;
	ret->number_of_constants=0;
	ret->consts=NULL;
	ret->id=id;
	
	return ret;
}
#endif
