#ifndef GCC_SEMANTICS_C
#define GCC_SEMANTICS_C GCC_SEMANTICS_C
#include"semantics.h"



long long int evaluate_const_expression_integer(struct AST *expression)
{
#define RET_BIN_EXPR(x,operator) return \
	evaluate_const_expression_integer(BIN_EXPR_PTR(x)->left)\
	operator\
	evaluate_const_expression_integer(BIN_EXPR_PTR(x)->right);
#define RET_UNARY_EXPR(x,operator) return \
	operator evaluate_const_expression_integer(UN_EXPR_PTR(x)->operand);

	switch(expression->type)
	{
		case OP_COMMA:
		case OP_ADDITION:
			RET_BIN_EXPR(expression,+);
		case OP_SUBTRACTION:
			RET_BIN_EXPR(expression,-);
		case OP_MUL:
			RET_BIN_EXPR(expression,*);
		case OP_DIV:
			RET_BIN_EXPR(expression,/);
		case OP_REMAINDER:
			RET_BIN_EXPR(expression,%);
		case OP_COND:
			return ( 
				evaluate_const_expression_integer(((struct AST_Conditional_Expression*)expression)->left)?
				evaluate_const_expression_integer(((struct AST_Conditional_Expression*)expression)->center):
				evaluate_const_expression_integer(((struct AST_Conditional_Expression*)expression)->right)
			       );

		case OP_LOGICAL_OR:
			RET_BIN_EXPR(expression,||);
		case OP_LOGICAL_AND:
			RET_BIN_EXPR(expression,&&);
		case OP_LOGICAL_NOT:
			RET_UNARY_EXPR(expression,!);
		case OP_BITWISE_OR:
			RET_BIN_EXPR(expression,|);
		case OP_BITWISE_AND:
			RET_BIN_EXPR(expression,&);
		case OP_BITWISE_XOR:
			RET_BIN_EXPR(expression,^);
		case OP_BITWISE_NOT:
			RET_UNARY_EXPR(expression,~);
		case OP_UNARY_PLUS:
			RET_UNARY_EXPR(expression,+);
		case OP_UNARY_MINUS:
			RET_UNARY_EXPR(expression,-);
		case OP_SHIFT_LEFT:
			RET_BIN_EXPR(expression,<<);
		case OP_SHIFT_RIGHT:
			RET_BIN_EXPR(expression,>>);
		case OP_LESS_EQ:
			RET_BIN_EXPR(expression,<=);
		case OP_GREATER_EQ:
			RET_BIN_EXPR(expression,>=);
		case OP_LESS:
			RET_BIN_EXPR(expression,<);
		case OP_GREATER:
			RET_BIN_EXPR(expression,>);
		case OP_EQUAL:
			RET_BIN_EXPR(expression,==);
		case OP_NOT_EQUAL:
			RET_BIN_EXPR(expression,!=);
		case OP_RVALUE:
			if(RVAL_EXPR_PTR(expression)->id->type==KW_NUMBER)
			{
				return evaluate_literal_integer_dec(RVAL_EXPR_PTR(expression)->id);
			}
			break;
	}
	/*shouldnt reach here*/
	return 0;
	
}

long long int evaluate_literal_integer_dec(struct token *token)
{
	long long int accumulate=0;
	size_t i;
	for(i=0;i<token->data_size;++i)
	{
		accumulate*=10;
		accumulate+=token->data[i]-'0';
	}
	return accumulate;

}

long long int evaluate_number_literal(struct token *token)
{
	return evaluate_literal_integer_dec(token);
}

#endif
