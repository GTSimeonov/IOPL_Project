#ifndef GCC_AST_C
#define GCC_AST_C GCC_AST_C
#include "ast.h"

struct AST_Error* get_error_tree(struct AST *error)
{
	struct AST_Error *ret;
	ret=malloc(sizeof(struct AST_Error));
	ret->type=ERROR;
	ret->error=error;
	return ret;
}
struct AST_Binary_Expression* get_binary_expression_tree(struct AST *left,struct AST *right,enum AST_Type type)
{
	struct AST_Binary_Expression *ret;
	ret=malloc(sizeof(struct AST_Binary_Expression));
	ret->type=type;
	ret->left=left;
	ret->right=right;

	return ret;
}
struct AST_Conditional_Expression* get_conditional_expression_tree(struct AST *left,struct AST *center,struct AST *right)
{
	struct AST_Conditional_Expression *ret;
	ret=malloc(sizeof(struct AST_Conditional_Expression));
	ret->type=OP_COND;
	ret->left=left;
	ret->center=center;
	ret->right=right;
	return ret;
}

struct AST_Function_Expression* get_function_expression_tree(struct AST* id,struct Scope *scope)
{
	struct AST_Function_Expression *ret;
	ret=malloc(sizeof(struct AST_Function_Expression));
	ret->type=OP_FUNCTION;
	ret->id=id;
	Queue_Init(&ret->arguments);
	return ret;
}
struct AST_Unary_Expression* get_unary_expression_tree(struct AST *operand,enum AST_Type type)
{

	struct AST_Unary_Expression *ret;
	ret=malloc(sizeof(struct AST_Unary_Expression));
	ret->operand=operand;
	ret->type=type;

	return ret;
}
struct AST_Rvalue_Expression* get_rvalue_expression_tree(struct token *id)
{
	struct AST_Rvalue_Expression *ret;
	ret=malloc(sizeof(struct AST_Rvalue_Expression));
	ret->type=OP_RVALUE;
	ret->id=id;
	return ret;
}
struct AST_Lvalue_Expression* get_lvalue_expression_tree(struct token *id,struct Scope* scope)
{
	struct AST_Lvalue_Expression *ret;
	ret=malloc(sizeof(struct AST_Lvalue_Expression));
	ret->type=OP_LVALUE;
	ret->id=id;
	ret->value_type=check_ordinary(scope,id);
	if(ret->value_type==NULL)
	{
		/*TODO error*/
	}
	return ret;
}





struct AST_Labeled_Statement* get_labeled_statement_tree(struct token *label,struct AST* statement,struct Scope *scope,enum AST_Type type)
{
	struct AST_Labeled_Statement *ret;
	ret=malloc(sizeof(struct AST_Labeled_Statement));
	ret->type=type;
	ret->label=label;
	ret->statement=statement;
	ret->scope=scope;

	return ret;
}
struct AST_Compound_Statement* get_compound_statement_tree(struct Scope *parent_scope)
{
	struct AST_Compound_Statement *ret;
	ret=malloc(sizeof(struct AST_Compound_Statement));
	ret->type=ST_COMPOUND;
	Queue_Init(&ret->components);
	ret->scope=get_scope(parent_scope);
	return ret;
}
struct AST_If_Statement* get_if_statement_tree()
{
	struct AST_If_Statement *ret;
	ret=malloc(sizeof(struct AST_If_Statement));
	ret->type=ST_IF;

	return ret;
}
struct AST_Switch_Statement* get_switch_statement_tree()
{
	struct AST_Switch_Statement *ret;
	ret=malloc(sizeof(struct AST_Switch_Statement));
	ret->type=ST_SWITCH;
	return ret;
}
struct AST_While_Statement* get_while_statement_tree()
{
	struct AST_While_Statement *ret;
	ret=malloc(sizeof(struct AST_While_Statement));
	ret->type=ST_WHILE;
	return ret;
}
struct AST_Do_While_Statement* get_do_while_statement_tree()
{
	struct AST_Do_While_Statement *ret;
	ret=malloc(sizeof(struct AST_Do_While_Statement));
	ret->type=ST_DO_WHILE;
	return ret;
}

struct AST_For_Statement* get_for_statement_tree()
{
	struct AST_For_Statement *ret;
	ret=malloc(sizeof(struct AST_For_Statement));
	ret->type=ST_FOR;
	return ret;
}
struct AST_Return_Statement* get_return_statement_tree(struct AST* return_expression)
{
	struct AST_Return_Statement *ret;
	ret=malloc(sizeof(struct AST_If_Statement));
	ret->type=ST_RETURN;
	ret->return_expression=return_expression;
	return ret;
}

struct AST_Goto_Statement* get_goto_statement_tree(struct token *label,struct Scope *scope)
{
	struct AST_Goto_Statement *ret;
	ret=malloc(sizeof(struct AST_Goto_Statement));
	ret->type=ST_GOTO;
	ret->label=label;
	return ret;
}


struct AST* get_nop_tree()
{
	struct AST* ret;
	ret=malloc(sizeof(struct AST*));
	ret->type=OP_NOP;
	return ret;
}



struct AST_Declaration* get_declaration_tree(struct Scope* scope)
{
	struct AST_Declaration *ret;
	ret=malloc(sizeof(struct AST_Declaration));
	ret->type=ST_DECLARATION;
	ret->base_type=malloc(sizeof(struct Type_Node));
	Queue_Init(&ret->declarators);
	ret->scope=scope;

	return ret;
}
struct AST_Function_Definition* get_function_definition_tree(struct Scope *scope)
{
	struct AST_Function_Definition *ret;
	ret=malloc(sizeof(struct AST_Function_Definition));
	ret->type=ST_FUNCTION_DEFINITION;
	return ret;
}

struct AST_Translation_Unit* get_translation_unit_tree(struct Scope* parent_scope)
{
	struct AST_Translation_Unit *ret;
	ret=malloc(sizeof(struct AST_Translation_Unit));
	ret->type=TRANSLATION_UNIT;
	Queue_Init(&ret->components);
	ret->scope=get_scope(parent_scope);
	return ret;
}








#endif
