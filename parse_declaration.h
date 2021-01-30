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

struct Denoted;
struct AST;
struct Scope;

struct AST* parse_declaration(struct Queue *tokens,struct Scope *scope);
void parse_declarator(struct Queue *tokens,struct Denoted *base,struct Scope *scope);
void parse_direct_declarator(struct Queue *tokens,struct Denoted *upper,struct Scope *scope);
void parse_abstract_declarator(struct Queue *tokens,struct Denoted *base,struct Scope *scope);
void parse_direct_abstract_declarator(struct Queue *tokens,struct Denoted *upper,struct Scope *scope);
void parse_pointer(struct Queue *tokens,struct Denoted *upper,struct Scope *scope);
struct Denoted* parse_init_declarator(struct Queue *tokens,struct Type_Node *base,struct Scope *scope);
struct AST* parse_initialiser(struct Queue *tokens,struct Scope *scope);
struct Denoted* parse_declaration_specifiers(struct Queue *tokens,struct Scope *scope);
struct Type_Node* parse_specifier_qualifier_list(struct Queue *tokens,struct Scope *scope);
void parse_parameter_type_list(struct Queue *tokens,struct Type_Node *function_base,struct Scope *scope);
void parse_parameter_list(struct Queue *tokens,struct Type_Node *function_base,struct Scope *scope);
void parse_parameter_declaration(struct Queue *tokens,struct Type_Node *function_base,struct Scope *scope);
void parse_direct_declarator_finish(struct Queue *tokens,struct Type *upper,struct Scope *scope);
char parse_direct_declarator_finish_step(struct Queue *tokens,struct Type *upper,struct Scope *scope);
struct Type* parse_type_name(struct Queue *tokens,struct Scope *scope);


void parse_struct_declarator(struct Queue *tokens,struct Denoted *base,struct Scope *scope);
void parse_struct_declarator_list(struct Queue *tokens,struct Queue *declarators,struct Type_Node *base_type,struct Scope *scope);
void parse_struct_declaration(struct Queue *tokens,struct Type_Node *struct_union_base,struct Scope *scope);
void parse_struct_declaration_list(struct Queue *tokens,struct Type_Node *struct_union_base,struct Scope *scope);
struct Struct_Union* parse_struct_union_specifier(struct Queue *tokens,struct Scope *scope);

char is_type(struct Queue *tokens,struct Scope *scope);
struct Type_Node* get_node();
struct Type* get_type();
struct Denoted* get_declarator(struct Type_Node *base_type);

#endif
