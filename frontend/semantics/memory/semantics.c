#ifndef GCC_SEMANTICS_C
#define GCC_SEMANTICS_C GCC_SEMANTICS_C
#include"semantics.h"



long long int evaluate_const_expression_integer(struct AST *expression,struct Translation_Data *translation_data)
{
#define RET_BIN_EXPR(x,operator) return \
	evaluate_const_expression_integer(BIN_EXPR_PTR(x)->left,translation_data)\
	operator\
	evaluate_const_expression_integer(BIN_EXPR_PTR(x)->right,translation_data);
#define RET_UNARY_EXPR(x,operator) return \
	operator evaluate_const_expression_integer(UN_EXPR_PTR(x)->operand,translation_data);

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
				evaluate_const_expression_integer(((struct AST_Conditional_Expression*)expression)->left,translation_data)?
				evaluate_const_expression_integer(((struct AST_Conditional_Expression*)expression)->center,translation_data):
				evaluate_const_expression_integer(((struct AST_Conditional_Expression*)expression)->right,translation_data)
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
	}
	/*shouldnt reach here*/
	return 0;
	
}

unsigned long long int evaluate_literal_integer_dec(struct token *token,struct Translation_Data *translation_data)
{
	unsigned long long int accumulate=0;
	size_t i;
	for(i=0;i<token->data_size;++i)
		accumulate*=10 , accumulate+=token->data[i]-'0';
	return accumulate;

}

unsigned long long int evaluate_literal_integer_hex(struct token *token,struct Translation_Data *translation_data)
{
	unsigned long long int accumulate=0;
	unsigned char digit;
	size_t i;
	/* skip the leading 0x */
	for(i=2;i<token->data_size;++i)
	{
		if(token->data[i]<='9')
			digit=token->data[i]-'0';
		else if(token->data[i]<='f' && token->data[i]>='a')
			digit=10+(token->data[i]-'a');
		else
			digit=10+(token->data[i]-'A');
		accumulate|=digit,accumulate<<=4;
	}
	return accumulate;
}

/*Various flavours of TODO*/
long double evaluate_literal_double_dec(struct token *token,struct Translation_Data *translation_data)
{
	return .0l;
}
long double evaluate_literal_double_hex(struct token *token,struct Translation_Data *translation_data)
{
	return .0l;
}

char* evaluate_literal_string(struct token *token,struct Translation_Data *translation_data)
{
	char *hold_data;
	hold_data=token->data+1;
	token->data[token->data_size]='\0';
	return hold_data;
}
wchar_t* evaluate_literal_wide_string(struct token *token,struct Translation_Data *translation_data)
{
	wchar_t *hold_data;
	hold_data=(wchar_t*)(token->data+1);
	token->data[token->data_size]='\0';
	return hold_data;
}

char evaluate_literal_char(struct token *token,struct Translation_Data *translation_data)
{
	return token->data[1];
}
wchar_t evaluate_literal_wide_char(struct token *token,struct Translation_Data *translation_data)
{
	return *(wchar_t*)(token->data+1);
}

wchar_t resolve_char_escape_sequence(struct token *token,struct Translation_Data *translation_data)
{
	return L'a';
}
/*TODO */
unsigned long long int evaluate_integer_constant(struct token *token,struct Translation_Data *translation_data)
{
	return evaluate_literal_integer_dec(token,translation_data);
}
#endif
