#ifndef GCC_PARSE_DECLARATION_H
#define GCC_PARSE_DECLARATION_H GCC_PARSE_DECLARATION_H
#include "ast.h"
#include "parse_expression.h"
#include "type.h"
#include "denoted.h"
#include "scope.h"
#include "queue.c"
#include "map.c"
#include "lexer.h"
#include <assert.h>

struct AST* parse_declaration(struct Queue *tokens,struct Scope *scope);

#endif
