#ifndef GCC_SCOPE_H
#define GCC_SCOPE_H GCC_SCOPE_H
#include <scope.hh>
#include <map.h>
#include <denoted.h>
#include <location.h>


enum Scope_Type;
struct Scope
{
	enum Scope_Type type;
	struct Scope *parent;
};
struct Normal_Scope
{
	enum Scope_Type type;
	struct Scope *parent;

	Map tags;
	Map ordinary;
};
struct Function_Scope
{
	enum Scope_Type type;
	struct Scope *parent;
	Map labels;
};


struct Scope* get_normal_scope(struct Scope *parent,enum Scope_Type type);
struct Scope* get_function_scope(struct Scope *parent);

void* check_label(struct Scope *current,struct token *id);
void push_label(struct Scope *current,struct token *id);/*TODO*/

struct Denoted* check_tag(struct Scope *current,struct token *id);
void push_tag(struct Scope *current,struct token *id,struct Denoted *denot);

void* check_ordinary(struct Scope *current,struct token *id);
void push_ordinary(struct Scope *current,struct token *id,struct Denoted *denot);

void Scope_Push(struct Scope *scope,struct Denoted *declarator);

char check_if_typedefed(struct Scope* scope,struct token *id);




void delete_scope(struct Scope *scope);
void delete_normal_scope(struct Normal_Scope *scope);
void delete_function_scope(struct Function_Scope *scope);
#endif
