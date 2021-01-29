#ifndef GCC_SCOPE_H
#define GCC_SCOPE_H GCC_SCOPE_H
#include "map.c"
#include "parse_declaration.h"

struct Scope
{
	Map labels;
	Map tags;
	Map ordinary;
	struct Scope *parent;
};


struct Scope* get_scope(struct Scope *parent);
void* check_label(struct Scope *current,struct token *id);
void* check_tag(struct Scope *current,struct token *id);
void* check_ordinary(struct Scope *current,struct token *id);
char Scope_Push(struct Scope *scope,struct Declarator *declarator);
char check_if_typedefed(struct Scope* scope,struct token *id);
#endif
