#ifndef PARSE_GCC_STATEMENT_H
#define PARSE_GCC_STATEMENT_H PARSE_GCC_STATEMENT_H
#include "all.h"

struct Queue;
struct Scope;
enum AST_Type;

struct AST* parse_finish_compound_statement(struct Queue* tokens,struct Scope *scope);
struct AST* parse_op_e_cl_st(struct Queue* tokens,struct Scope *scope);
struct AST* parse_finish_if_statement(struct Queue* tokens,struct Scope *scope);
struct AST* parse_finish_switch_statement(struct Queue* tokens,struct Scope *scope);
struct AST* parse_finish_do_while_statement(struct Queue* tokens,struct Scope *scope);
struct AST* parse_finish_while_statement(struct Queue* tokens,struct Scope *scope);
struct AST* parse_finish_for_statement(struct Queue* tokens,struct Scope *scope);
struct AST* parse_finish_goto_statement(struct Queue* tokens,struct Scope *scope);
struct AST* parse_finish_continue_statement(struct Queue* tokens);
struct AST* parse_finish_break_statement(struct Queue* tokens);
struct AST* parse_finish_return_statement(struct Queue* tokens,struct Scope *scope);
struct AST* parse_expression_statement(struct Queue* tokens,struct Scope *scope);
struct AST* parse_finish_labeled_statement(struct Queue* tokens,struct Scope *scope,enum AST_Type type);
struct AST* parse_finish_default_statement(struct Queue* tokens,struct Scope *scope);



#endif
