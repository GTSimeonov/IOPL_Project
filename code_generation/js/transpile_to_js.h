#ifndef GCC_TRANSPILE_TO_JS_H
#define GCC_TRANSPILE_TO_JS_H GCC_TRANSPILE_TO_JS_H
#include <program.h>
#include <gcc_arguments.h>
#include <stdio.h>
#include <print.h>


void transpile_to_javascript(FILE* out,struct Program *program,struct Command_Arguments *command_arguments);

void _to_js_print_externs(void *denoted,void *args);
void to_js_print_externs(FILE* out,struct Program *program,struct Command_Arguments *command_arguments);

void _to_js_print_statics(void *denoted,void *args);
void to_js_print_statics(FILE* out,struct AST_Translation_Unit *translation_unit,struct Command_Arguments* command_arguments);

void to_js_print_ast(FILE* out,struct AST *tree,struct Program *program);
void to_js_print_translation_unit_tree(FILE* out,struct AST_Translation_Unit *translation_unit,struct Program *program);
void to_js_print_binary_expression_tree(FILE* out,struct AST_Binary_Expression *bin,struct Program *program);
void to_js_print_conditional_expression_tree(FILE* out,struct AST_Conditional_Expression *cond,struct Program *program);
void to_js_print_function_expression_tree(FILE* out,struct AST_Function_Expression *function_call,struct Program *program);
void to_js_print_rvalue_expression_tree(FILE* out,struct AST_Rvalue_Expression *rval,struct Program *program);
void to_js_print_lvalue_expression_tree(FILE* out,struct AST_Lvalue_Expression *lval,struct Program *program);
void to_js_print_unary_expression_tree(FILE* out,struct AST_Unary_Expression *unary,struct Program *program);
void to_js_print_labeled_statement_tree(FILE* out,struct AST_Labeled_Statement *label,struct Program *program);
void to_js_print_compound_statement_tree(FILE* out,struct AST_Compound_Statement *compound,struct Program *program);
void to_js_print_for_statement_tree(FILE* out,struct AST_For_Statement *for_statement,struct Program *program);
void to_js_print_while_statement_tree(FILE* out,struct AST_While_Statement *while_statement,struct Program *program);
void to_js_print_do_while_statement_tree(FILE* out,struct AST_Do_While_Statement *do_while_statement,struct Program *program);
void to_js_print_if_statement_tree(FILE* out,struct AST_If_Statement *if_statement,struct Program *program);
void to_js_print_goto_statement_tree(FILE* out,struct AST_Goto_Statement *goto_statement,struct Program *program);
void to_js_print_switch_statement_tree(FILE* out,struct AST_Switch_Statement *switch_statement,struct Program *program);
void to_js_print_return_statement_tree(FILE* out,struct AST_Return_Statement *return_statement,struct Program *program);
void to_js_print_object_declaration_tree(FILE* out,struct AST_Object_Declaration *object_declaration,struct Program *program);
void to_js_print_function_definition(FILE* out,struct AST_Function_Definition *function_definition,struct Program *program);
void to_js_print_function_declaration_tree(FILE* out,struct AST_Function_Declaration *function_declaration,struct Program *program);
void to_js_print_denoted(FILE* out,struct Denoted* denoted,struct Program *program);



#endif
