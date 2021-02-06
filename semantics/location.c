#ifndef GCC_LOCATION_C
#define GCC_LOCATION_C GCC_LOCATION_C
#include "location.h"


struct Location_Stack* get_location_on_stack(size_t offset)
{
	struct Location_Stack *ret;
	ret=malloc(sizeof(struct Location_Stack));
	ret->type=LT_ON_STACK;
	ret->offset=offset;

	return ret;
}
struct Location_Raw* get_location_raw(size_t address)
{
	struct Location_Raw *ret;
	ret=malloc(sizeof(struct Location_Raw));
	ret->type=LT_RAW;
	ret->address=address;

	return ret;
}

struct Location_Relative* get_relative_location(struct Location *base,size_t offset)
{
	struct Location_Relative *ret;
	ret=malloc(sizeof(struct Location_Relative));
	ret->type=LT_RELATIVE;
	ret->base=base;
	ret->offset=offset;

	return ret;
}
/*base is modified*/
struct Location *get_location_for_denoted_object(struct Location *base,struct Type *type,struct token *id)
{
	if(base->type==LT_ON_STACK)
	{
		struct Location_Stack *hold;
		hold=malloc(sizeof(struct Location_Stack));
		*hold=*(struct Location_Stack*)base;
		((struct Location_Stack*)base)->offset+=get_type_size(type);

		return (struct Location*)hold;
	}else if(base->type==LT_RELATIVE)
	{
		struct Location_Relative *hold;

		hold=malloc(sizeof(struct Location_Relative));
		*hold=*(struct Location_Relative*)base;

		return (struct Location*)hold;
	}else if(base->type==LT_GLOBAL)
	{
		struct Location_Labeled *hold;
		hold=malloc(sizeof(struct Location_Labeled));
		hold->id=id;

		return (struct Location*)hold;
	}
}
struct Location *get_global_location()
{
	struct Location *ret;
	ret=malloc(sizeof(struct Location));
	ret->type=LT_GLOBAL;

	return ret;
}
#endif
