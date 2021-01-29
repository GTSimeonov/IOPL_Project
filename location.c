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
#endif
