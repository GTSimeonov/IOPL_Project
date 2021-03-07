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
struct Denoted* check_tag(struct Scope *current,struct token *id);
void* check_ordinary(struct Scope *current,struct token *id);


void push_label(struct Scope *current,struct token *id);/*TODO*/
void push_object(struct Scope *current,struct Translation_Data *translation_data,struct Denoted_Object *denoted_object);
void push_function(struct Scope *current,struct Translation_Data *translation_data,struct Denoted_Function *denoted_function);
void push_typedef(struct Scope *current,struct Translation_Data *translation_data,struct Denoted_Typedef *denoted_typedef);
void push_denoted_enum_tag(struct Scope *current,struct Translation_Data *translation_data,struct Denoted_Enum *denoted_enum);
void push_denoted_enum_constant(struct Scope *current,struct Translation_Data *translation_data,struct Denoted_Enum_Const *denoted_enum_constant);
void push_denoted_struct_union_tag(struct Scope *current,struct Translation_Data *translation_data,struct Denoted_Struct_Union *denoted_struct_union);
void Scope_Push(struct Scope *scope,struct Denoted *declarator,struct Translation_Data *translation_data);


void delete_scope(struct Scope *scope);
void delete_normal_scope(struct Normal_Scope *scope);
void delete_function_scope(struct Function_Scope *scope);




char check_if_typedefed(struct Scope* scope,struct token *id);
#endif
