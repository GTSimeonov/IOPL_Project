#ifndef GCC_SEMANTICS_H
#define GCC_SEMANTICS_H GCC_SEMANTICS_H
#include "semantics.h"

long long int evaluate_const_expression_integer(struct AST *expression);
long long int evaluate_literal_integer_dec(struct token *token);
#endif
