#ifndef GCC_AST_H
#define GCC_AST_H GCC_AST_H
#include <ast.hh>
#include <scope.h>
#include <parse_declaration.h>
#include <denoted.h>



enum AST_Type;



/*
struct AST
{
	enum AST_Type type;

	unsigned long value_type;
	void *data;

	struct Queue arguments;

};
*/
struct AST
{
	enum AST_Type type;
};
struct AST_Error
{
	enum AST_Type type;
	struct AST *error;
};
struct AST_Declaration_Error
{
	enum AST_Type type;
	struct Denoted *error;
};
struct AST_Binary_Expression
{
	enum AST_Type type;

	struct Type *value_type;
	struct AST *left;
	struct AST *right;

};
struct AST_Conditional_Expression
{
	enum AST_Type type;

	struct Type *value_type;

	struct AST *left;
	struct AST *center;
	struct AST *right;

};
struct AST_Function_Expression
{
	enum AST_Type type;
	struct Type *value_type;

	struct AST *id;
	/*queue of astrees*/
	struct Queue arguments;
};
struct AST_Rvalue_Expression
{
	enum AST_Type type;
	struct Type *value_type;
	struct token *id;
};
struct AST_Lvalue_Expression
{
	enum AST_Type type;
	struct Type *value_type;
	struct token *id;
	/*TODO*/
	struct Object *object;
};
struct AST_Unary_Expression
{
	enum AST_Type type;
	struct Type *value_type;
	struct AST *operand;
};

struct AST_Labeled_Statement
{
	enum AST_Type type;
	struct token *label;
	struct AST *statement;
};
struct AST_Compound_Statement
{
	enum AST_Type type;

	struct Scope *scope;
	struct Queue components;
};

struct AST_For_Statement
{
	enum AST_Type type;
	struct AST *condition;
	struct AST *initialisation;
	struct AST *update;
	struct AST *body_statement;
};
struct AST_While_Statement
{
	enum AST_Type type;
	struct AST *condition;
	struct AST *body_statement;
};
struct AST_Do_While_Statement
{
	enum AST_Type type;
	struct AST *body_statement;
	struct AST *condition;
};
struct AST_If_Statement
{
	enum AST_Type type;
	struct AST* condition;
	struct AST* body_statement;
	struct AST* else_statement;

};
struct AST_Goto_Statement
{
	enum AST_Type type;
	struct token *label;
};
struct AST_Switch_Statement
{
	enum AST_Type type;
	struct AST* condition;
	struct AST* body_statement;
};
struct AST_Return_Statement
{
	enum AST_Type type;
	struct AST* return_expression;

};


struct AST_Type_Definition
{
	enum AST_Type type;
	struct Denoted_Typedef *definition;
};
struct AST_Object_Declaration
{
	enum AST_Type type;
	struct Denoted_Object *object;
	struct AST *initializer;
};
struct AST_Function_Definition
{
	enum AST_Type type;
	struct Denoted_Function *function;
};
struct AST_Function_Declaration
{
	enum AST_Type type;
	struct Denoted_Function *function;
};
struct AST_Translation_Unit
{
	enum AST_Type type;
	struct Queue components;
	struct Scope *scope;
};




struct AST_Error* get_error_tree(struct AST *error);
struct AST_Declaration_Error* get_declaration_error_tree(struct Denoted *error);
struct AST_Binary_Expression* get_binary_expression_tree(struct AST *left,struct AST *right,enum AST_Type type);
struct AST_Conditional_Expression* get_conditional_expression_tree(struct AST *left,struct AST *center,struct AST *right);
struct AST_Function_Expression* get_function_expression_tree(struct AST* id,struct Scope *scope);
struct AST_Unary_Expression* get_unary_expression_tree(struct AST *operand,enum AST_Type type);
struct AST_Rvalue_Expression* get_rvalue_expression_tree(struct token *id);
struct AST_Lvalue_Expression* get_lvalue_expression_tree(struct token *id,struct Scope* scope);
struct AST_Labeled_Statement* get_labeled_statement_tree(struct token *label,struct AST* statement,enum AST_Type type);
struct AST_Compound_Statement* get_compound_statement_tree(struct Scope *parent_scope);
struct AST_If_Statement* get_if_statement_tree();
struct AST_Switch_Statement* get_switch_statement_tree();
struct AST_While_Statement* get_while_statement_tree();
struct AST_Do_While_Statement* get_do_while_statement_tree();
struct AST_For_Statement* get_for_statement_tree();
struct AST_Return_Statement* get_return_statement_tree(struct AST* return_expression);
struct AST_Goto_Statement* get_goto_statement_tree(struct token *label,struct Scope *scope);
struct AST* get_nop_tree();
struct AST_Type_Definition* get_type_definition_tree(struct Denoted_Typedef *definition);
struct AST_Object_Declaration* get_object_declaration_tree(struct Denoted_Object *object,struct AST *initializer);
struct AST_Function_Declaration* get_function_declaration_tree(struct Scope *scope,struct Denoted_Function *function);
struct AST_Function_Definition* get_function_definition_tree(struct Scope *scope,struct Denoted_Function *function);
struct AST_Translation_Unit* get_translation_unit_tree(struct Scope* parent_scope);



void delete_ast(struct AST* ast);
void delete_ast_error(struct AST_Error *error);
void delete_ast_declaration_error(struct AST_Declaration_Error *error);
void delete_ast_binary_expression(struct AST_Binary_Expression *binary_expression);
void delete_ast_conditional_expression(struct AST_Conditional_Expression *cond_expression);
void delete_ast_function_expression(struct AST_Function_Expression *function_expression);
void delete_ast_rvalue_expression(struct AST_Rvalue_Expression *rval_expression);
void delete_ast_lvalue_expression(struct AST_Lvalue_Expression *lval_expression);
void delete_ast_unary_expression(struct AST_Unary_Expression *unary_expression);
void delete_ast_labeled_statement(struct AST_Labeled_Statement *labeled_statement);
void delete_ast_compound_statement(struct AST_Compound_Statement *compound_statement);
void delete_ast_for_statement(struct AST_For_Statement *for_statement);
void delete_ast_while_statemtent(struct AST_While_Statement *while_statement);
void delete_ast_do_while_statement(struct AST_Do_While_Statement *do_while_statement);
void delete_ast_if_statemtent(struct AST_If_Statement *if_statement);
void delete_ast_goto_statemtent(struct AST_Goto_Statement *goto_statement);
void delete_ast_switch_statement(struct AST_Switch_Statement *switch_statement);
void delete_ast_return_statement(struct AST_Return_Statement *return_statement);
void delete_ast_type_definition(struct AST_Type_Definition *type_definition);
void delete_ast_object_declaration(struct AST_Object_Declaration *object_declaration);
void delete_ast_function_definition(struct AST_Function_Definition *function_definition);
void delete_ast_function_declaration(struct AST_Function_Declaration *function_declaration);
void delete_ast_translation_unit(struct AST_Translation_Unit *translation_unit);


#define BIN_EXPR_PTR(x) ((struct AST_Binary_Expression*)(x))
#define UN_EXPR_PTR(x) ((struct AST_Unary_Expression*)(x))
#define LVAL_EXPR_PTR(x) ((struct AST_Lvalue_Expression*)(x))
#define RVAL_EXPR_PTR(x) ((struct AST_Rvalue_Expression*)(x))
#define DECLR_PTR(x) ((struct AST_Declaration*)(x))
#define IF_ST_PTR(s) ((struct AST_If_Statement*)(x))



#endif
