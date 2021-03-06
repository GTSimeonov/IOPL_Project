#ifndef WONKY_AST_HH
#define WONKY_AST_HH WONKY_AST_HH

#define BIN_EXPR_PTR(x) ((struct AST_Binary_Expression*)(x))
#define UN_EXPR_PTR(x) ((struct AST_Unary_Expression*)(x))
#define LVAL_EXPR_PTR(x) ((struct AST_Lvalue_Expression*)(x))
#define DECLR_PTR(x) ((struct AST_Declaration*)(x))
#define IF_ST_PTR(s) ((struct AST_If_Statement*)(x))

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
	,OP_LVALUE,OP_CONSTANT,OP_STRING_LITERAL
	,ST_COMPOUND,ST_EXPRESSION,ST_SWITCH,ST_IF,ST_WHILE,ST_DO_WHILE,ST_GOTO,ST_LABEL,ST_CASE,ST_DEFAULT
	,ST_CONTINUE,ST_BREAK,ST_RETURN,ST_FOR
	,ST_OBJECT_DECLARATION,ST_TYPE_DEFINITION,ST_FUNCTION_DEFINITION
	,ST_FUNCTION_DECLARATION
	,TRANSLATION_UNIT
	,ERROR,ERROR_DECLARATION
};
struct AST;
struct AST_Error;
struct AST_Declaration_Error;
struct AST_Binary_Expression;
struct AST_Conditional_Expression;
struct AST_Function_Expression;
struct AST_Constant;
struct AST_String_Literal;
struct AST_Lvalue_Expression;
struct AST_Unary_Expression;
struct AST_Labeled_Statement;
struct AST_Compound_Statement;
struct AST_For_Statement;
struct AST_While_Statement;
struct AST_Do_While_Statement;
struct AST_If_Statement;
struct AST_Goto_Statement;
struct AST_Switch_Statement;
struct AST_Return_Statement;
struct AST_Type_Definition;
struct AST_Object_Declaration;
struct AST_Function_Definition;
struct AST_Function_Declaration;
struct AST_Translation_Unit;

#endif
