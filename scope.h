#ifndef GCC_SCOPE_H
#define GCC_SCOPE_H GCC_SCOPE_H
#include "map.c"
#include "denoted.h"
#include "location.h"

struct Scope
{
	Map labels;

	Map tags;
	/*In go denoted*/
	Map ordinary;

	struct Scope *parent;
	struct Location *location;
};


struct Scope* get_scope(struct Scope *parent);

void* check_label(struct Scope *current,struct token *id);

void push_label(struct Scope *current,struct token *id);/*TODO*/

struct Denoted* check_tag(struct Scope *current,struct token *id);
void push_tag(struct Scope *current,struct token *id,struct Denoted *denot);

void* check_ordinary(struct Scope *current,struct token *id);
void push_ordinary(struct Scope *current,struct token *id,struct Denoted *denot);

void Scope_Push(struct Scope *scope,struct Denoted *declarator);
char check_if_typedefed(struct Scope* scope,struct token *id);
#endif
