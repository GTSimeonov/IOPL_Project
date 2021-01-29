#ifndef GCC_SEMANTICS_C
#define GCC_SEMANTICS_C GCC_SEMANTICS_C



long long int evaluate_const_expression_integer(struct AST *expression)
{
	switch(expression->type)
	{
		case OP_ADDITION:
			return
			evaluate_const_expression_integer(BIN_EXPR_PTR(expression)->left)
			+
			evaluate_const_expression_integer(BIN_EXPR_PTR(expression)->right);
			break;
		case OP_SUBTRACTION:
			return
			evaluate_const_expression_integer(BIN_EXPR_PTR(expression)->left)
			-
			evaluate_const_expression_integer(BIN_EXPR_PTR(expression)->right);
			break;
		case OP_DIV:
			return
			evaluate_const_expression_integer(BIN_EXPR_PTR(expression)->left)
			/
			evaluate_const_expression_integer(BIN_EXPR_PTR(expression)->right);
			break;
		case OP_MUL:
			return
			evaluate_const_expression_integer(BIN_EXPR_PTR(expression)->left)
			*
			evaluate_const_expression_integer(BIN_EXPR_PTR(expression)->right);
			break;
		case OP_RVALUE:
			if(RVAL_EXPR_PTR(expression)->id->type==KW_NUMBER)
			{
				return evaluate_literal_integer_dec(RVAL_EXPR_PTR(expression)->id);
			}
			break;
	}
	
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


#endif
