#ifndef GCC_PARSE_DECLARATION_H
#define GCC_PARSE_DECLARATION_H GCC_PARSE_DECLARATION_H
#include <ast.h>
#include <parse_expression.h>
#include <parse_statement.h>
#include <type.h>
#include <denoted.h>
#include <scope.h>
#include <queue.h>
#include <map.h>
#include <lexer.h>
#include <assert.h>

struct Denoted_Base;
struct Denotation_Prototype;
struct Struct_Union;
struct Denoted_Object;
struct Enum;


void parse_declaration(struct Queue *tokens,struct Scope *scope,struct Queue *where_to_push,char parse_function_definitions);
struct Denotation_Prototype* parse_specifier_qualifier_list(struct Queue *tokens,struct Scope *scope);
struct Denotation_Prototype* parse_declaration_specifiers(struct Queue *tokens,struct Scope *scope);
struct Denotation_Prototype* parse_declaration_specifiers_inner(struct Queue *tokens,struct Scope *scope,char parse_storage_class);
struct Denoted* parse_declarator(struct Queue *tokens,struct Scope *scope,struct Denotation_Prototype *prototype);
void parse_declarator_inner(struct Queue *tokens,struct Scope *scope,struct Denoted_Base *base);
void parse_direct_declarator(struct Queue *tokens,struct Scope *scope,struct Denoted_Base *base);
void parse_direct_declarator_finish(struct Queue *tokens,struct Scope *scope,struct Denoted_Base *base);
void parse_struct_union_specifier_finish(struct Queue *tokens,struct Scope *scope,struct Struct_Union *base);
char parse_struct_declaration(struct Queue *tokens,struct Scope *struct_scope,struct Queue* members);
struct Denoted* parse_struct_declarator(struct Queue *tokens,struct Scope *scope,struct Denotation_Prototype *prototype);
void parse_enum_specifier_finish(struct Queue *tokens,struct Scope *scope,struct Enum *enumeration);
void parse_paramenter_list(struct Queue *tokens,struct Scope *function_prototype_scope,struct Queue *parameters);
struct Type* parse_type_name(struct Queue *tokens,struct Scope *scope);
struct Type* parse_abstract_declarator(struct Queue *tokens,struct Scope *scope,struct Type *base);
struct AST* parse_initializer(struct Queue *tokens,struct Scope *scope,struct Denoted_Object *base);

#endif
