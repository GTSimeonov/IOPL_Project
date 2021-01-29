#ifndef GCC_CONTEXT
#define GCC_CONTEXT GCC_CONTEXT
#include"map.c"
#include<assert.h>


struct Scope
{
	unsigned used_types;
	unsigned used_vars;
	Map *types;
	Map *variables;
}

/*use a different function for global scope*/
struct Scope get_scope(struct Scope *upper_scope)
{
	struct Scope ret;
	assert(upper_scope==NULL);

	ret.used_types=upper_scope->used_types;
	ret.used_vars=upper_scope->used_vars;

	ret.types=upper_scope->types;
	ret.variables=upper_scope->variables;

	return ret;
}


#endif
