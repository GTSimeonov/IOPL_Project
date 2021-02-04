#ifndef GCC_AST_H
#define GCC_AST_H GCC_AST_H
#include "scope.h"
#include "parse_declaration.h"
#include "denoted.h"


struct Denoted;

enum AST_Type{
	 OP_COMMA
	,OP_ADDITION,OP_SUBTRACTION,OP_MUL,OP_DIV,OP_REMAINDER
	,OP_COND,OP_FUNCTION
	,OP_ASSIGN,OP_ADD_ASSIGN,OP_SUBTRACT_ASSIGN,OP_MULTIPLY_ASSIGN,OP_REMAINDER_ASSIGN,OP_DIV_ASSIGN
	,OP_SHIFT_LEFT_ASSIGN,OP_SHIFT_RIGHT_ASSIGN
	,OP_AND_ASSIGN,OP_XOR_ASSIGN,OP_PIPE_ASSIGN
	,OP_NOP
	,OP_LOGICAL_OR,OP_LOGICAL_AND,OP_LOGICAL_NOT
	,OP_BITWISE_OR,OP_BITWISE_AND,OP_BITWISE_XOR,OP_BITWISE_NOT
	,OP_ADDR_OF,OP_DEREFERENCE,OP_MEMBER_TROUGH_PTR,OP_MEMBER,OP_ARR_SUBSCRIPT
	,OP_POSTFIX_INC,OP_POSTFIX_DEC
	,OP_PREFIX_INC,OP_PREFIX_DEC
	,OP_UNARY_PLUS,OP_UNARY_MINUS
	,OP_CAST,OP_SIZEOF
	,OP_SHIFT_LEFT,OP_SHIFT_RIGHT
	,OP_LESS_EQ,OP_GREATER_EQ
	,OP_LESS,OP_GREATER
	,OP_EQUAL,OP_NOT_EQUAL
	,OP_LVALUE,OP_RVALUE
	,ST_COMPOUND,ST_EXPRESSION,ST_SWITCH,ST_IF,ST_WHILE,ST_DO_WHILE,ST_GOTO,ST_LABEL,ST_CASE,ST_DEFAULT
	,ST_CONTINUE,ST_BREAK,ST_RETURN,ST_FOR
	,ST_OBJECT_DECLARATION,ST_TYPE_DEFINITION,ST_FUNCTION_DEFINITION
	,ST_FUNCTION_DECLARATION
	,TRANSLATION_UNIT
	,ERROR,ERROR_DECLARATION
};




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
	void *object;
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
	struct Scope *scope;
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
	struct Scope *scope;
};
struct AST_Object_Declaration
{
	enum AST_Type type;
	struct Denoted_Object *object;
	struct AST *initializer;
	struct Scope *scope;
};
struct AST_Function_Definition
{
	enum AST_Type type;
	struct Denoted_Function *function;
	struct Scope *scope;
};
struct AST_Function_Declaration
{
	enum AST_Type type;
	struct Denoted_Function *function;
	struct Scope *scope;
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
struct AST_Labeled_Statement* get_labeled_statement_tree(struct token *label,struct AST* statement,struct Scope *scope,enum AST_Type type);
struct AST_Compound_Statement* get_compound_statement_tree(struct Scope *parent_scope);
struct AST_If_Statement* get_if_statement_tree();
struct AST_Switch_Statement* get_switch_statement_tree();
struct AST_While_Statement* get_while_statement_tree();
struct AST_Do_While_Statement* get_do_while_statement_tree();
struct AST_For_Statement* get_for_statement_tree();
struct AST_Return_Statement* get_return_statement_tree(struct AST* return_expression);
struct AST_Goto_Statement* get_goto_statement_tree(struct token *label,struct Scope *scope);
struct AST* get_nop_tree();
struct AST_Type_Definition* get_type_definition_tree(struct Denoted_Typedef *definition,struct Scope *scope);
struct AST_Object_Declaration* get_object_declaration_tree(struct Denoted_Object *object,struct AST *initializer,struct Scope *scope);
struct AST_Function_Declaration* get_function_declaration_tree(struct Scope *scope,struct Denoted_Function *function);
struct AST_Function_Definition* get_function_definition_tree(struct Scope *scope,struct Denoted_Function *function);
struct AST_Translation_Unit* get_translation_unit_tree(struct Scope* parent_scope);


#define BIN_EXPR_PTR(x) ((struct AST_Binary_Expression*)(x))
#define UN_EXPR_PTR(x) ((struct AST_Unary_Expression*)(x))
#define LVAL_EXPR_PTR(x) ((struct AST_Lvalue_Expression*)(x))
#define RVAL_EXPR_PTR(x) ((struct AST_Rvalue_Expression*)(x))
#define DECLR_PTR(x) ((struct AST_Declaration*)(x))
#define IF_ST_PTR(s) ((struct AST_If_Statement*)(x))



#endif
