#ifndef GCC_SEMANTICS_H
#define GCC_SEMANTICS_H GCC_SEMANTICS_H
#include "semantics.h"

enum Location_Type
{
	LT_ON_STACK,
	LT_RAW,
	LT_LABEL
};
struct Location 
{
	enum Location_Type type;
	union
	{
		size_t offset;
		struct token *label;
	}specifics;
};
struct Object
{
	struct Type *type;
	struct Location location;
};


long long int evaluate_const_expression_integer(struct AST *expression);
long long int evaluate_literal_integer_dec(struct token *token);
#endif
