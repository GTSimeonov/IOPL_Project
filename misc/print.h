#ifndef GCC_PRINT_H
#define GCC_PRINT_H GCC_PRINT_H

#include<stdio.h>
#include<assert.h>
#include <all.h>


#define INDENT for(int j=0;j<indent;++j) fprintf(out," ");
#define TOK(s) ((struct token*)(s))
#define ASTPTR(s) ((struct AST*)(s))

int indent;

void print_token(FILE *out,struct token *token);
void print_tokens(FILE *out,struct Queue *tokens);
void print_ast_enum(FILE *out,enum AST_Type op);
void print_error_tree(FILE *out,struct AST_Error *error);
void print_binary_expression_tree(FILE *out,struct AST_Binary_Expression *bin);
void print_conditional_expression_tree(FILE *out,struct AST_Conditional_Expression *cond);
void print_function_expression_tree(FILE *out,struct AST_Function_Expression *function_call);
void print_unary_expression_tree(FILE *out,struct AST_Unary_Expression *unary_expression);
void print_rvalue_expression_tree(FILE *out,struct AST_Rvalue_Expression *rval);
void print_lvalue_expression_tree(FILE *out,struct AST_Lvalue_Expression *lval);
void print_labeled_statement_tree(FILE *out,struct AST_Labeled_Statement *lab);
void print_compound_statement_tree(FILE *out,struct AST_Compound_Statement *comp);
void print_if_statement_tree(FILE *out,struct AST_If_Statement *ifs);
void print_switch_statement_tree(FILE *out,struct AST_Switch_Statement *swi);
void print_while_statement_tree(FILE *out,struct AST_While_Statement *whi);
void print_do_while_statement_tree(FILE *out,struct AST_Do_While_Statement *whi);
void print_for_statement_tree(FILE *out,struct AST_For_Statement *fo);
void print_return_statement_tree(FILE *out,struct AST_Return_Statement *return_expression);
void print_goto_statement_tree(FILE *out,struct AST_Goto_Statement *got);
void print_type(FILE *out,struct Type *type,char print_struct_union);
void print_denoted(FILE *out,struct Denoted *denoted);
void print_list_of_denoted(FILE *out,struct Queue *denoted);
void print_enumeration(FILE *out,struct Enum *enumeration);
void print_struct_union(FILE *out,struct Struct_Union *struct_union);
void print_translation_unit_tree(FILE *out,struct AST_Translation_Unit *unit);
void print_ast(FILE *out,struct AST* tree);
void print_program_tokens(FILE *out,struct Program *program);
void print_program_ast(FILE *out,struct Program *program);
void print_keyword_enum(FILE *out,enum KEYWORDS kw);
void print_function_definition(FILE *out,struct Denoted_Function *function);


#endif
