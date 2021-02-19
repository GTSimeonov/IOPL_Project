#ifndef GCC_PARSE_EXPR_C
#define GCC_PARSE_EXPR_C GCC_PARSE_EXPR_C
#include "parse_expression.h"



struct AST* parse_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	return parse_comma_expression(translation_data,scope);
}

struct AST* parse_const_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	return parse_comma_expression(translation_data,scope);
}
/*
primary-expression:
	number
	string
	id
	(e)
	generic-selection

*/
struct AST* parse_primary_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	struct token *hold_token;
	struct AST *hold;
	
	if(translation_data->tokens->size==0)
	{
		/*TODO error*/
		return (struct AST*)get_error_tree(NULL);
	}
	hold_token=Queue_Pop(translation_data->tokens);
	switch(hold_token->type)
	{
		case KW_NUMBER:
		case KW_STRING:
			return (struct AST*)get_rvalue_expression_tree(hold_token);
		case KW_ID:
			return (struct AST*)get_lvalue_expression_tree(hold_token,scope);
		case KW_OPEN_NORMAL:
			hold=parse_expression(translation_data,scope);
			if(get_and_check(translation_data,KW_CLOSE_NORMAL))
			{
				return (struct AST*)hold;
			}else
			{
				/*TODO error*/
				return (struct AST*)get_error_tree(hold);
			}
		default:
			/*TODO error*/
			return (struct AST*)get_error_tree(NULL);
	}

	/*just in case*/
	return (struct AST*)get_error_tree(NULL);

}


/*
arglist:
	(e)*\)
	
*/
struct AST_Function_Expression* parse_arglist(struct Translation_Data *translation_data,struct Scope *scope,struct AST* id)
{
	struct AST_Function_Expression *ret;
	ret=get_function_expression_tree(id,scope);
	ret->id=id;
	if(get_and_check(translation_data,KW_CLOSE_NORMAL))
	{
		return ret;
	}
	do
	{
		Queue_Push(&ret->arguments,parse_assignment_expression(translation_data,scope));
	} while(get_and_check(translation_data,KW_COMMA));
	if(get_and_check(translation_data,KW_CLOSE_NORMAL))
	{
		return ret;
	}else
	{
		return (struct AST_Function_Expression*)get_error_tree((struct AST*)ret);
	}

}

/*
postfix_expression:
	postfix_expression ++
	postfix_expression --
	postfix_expression [ expression ]
	postfix_expression.id
	postfix_expression->id
	postfix_expression ( arglist )


	primary_expression
postfix_expression:
	primary_expression ( ++ | -- | \[ expression \] | .id | ->id | \( arglist \) )*

*/
struct AST* parse_postfix_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	
	struct AST *hold;
	struct AST *hold_expr;

	
	hold=parse_primary_expression(translation_data,scope);

	while(translation_data->tokens->size!=0)
	{
		switch(((struct token*)translation_data->tokens->first->data)->type)
		{
			case KW_PLUSPLUS:
				chomp(translation_data);
				hold=(struct AST*)get_unary_expression_tree(hold,OP_POSTFIX_INC);
				break;
			case KW_MINUSMINUS:
				chomp(translation_data);
				hold=(struct AST*)get_unary_expression_tree(hold,OP_POSTFIX_DEC);
				break;
			case KW_DOT:
				chomp(translation_data);
				if(check(translation_data,KW_ID,0))
				{
					hold=(struct AST*)get_binary_expression_tree(hold,(struct AST*)get_lvalue_expression_tree(Queue_Pop(translation_data->tokens),scope),OP_MEMBER);
				}
				break;
			case KW_ARROW:
				chomp(translation_data);
				if(check(translation_data,KW_ID,0))
				{
					hold=(struct AST*)get_binary_expression_tree(hold,(struct AST*)get_lvalue_expression_tree(Queue_Pop(translation_data->tokens),scope),OP_MEMBER_TROUGH_PTR);
				}
				break;
			case KW_OPEN_SQUARE:

				chomp(translation_data);
				if(get_and_check(translation_data,KW_CLOSE_SQUARE))
				{
					hold=(struct AST*)get_binary_expression_tree(hold,NULL,OP_ARR_SUBSCRIPT);
				}else
				{
					hold_expr=parse_expression(translation_data,scope);
					hold=(struct AST*)get_binary_expression_tree(hold,hold_expr,OP_ARR_SUBSCRIPT);
					if(!get_and_check(translation_data,KW_CLOSE_SQUARE))
					{
						return (struct AST*)get_error_tree(hold);
					}
				}
				break;
			case KW_OPEN_NORMAL:
				chomp(translation_data);
				return (struct AST*)parse_arglist(translation_data,scope,hold);
				break;

			default:
				return hold;
		}
	}

	return hold;
}

/*
	cast-expression:
		unary-expression
		(type)cast-expression
*/
struct AST* parse_cast_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	struct AST_Unary_Expression *ret;
	if(get_and_check(translation_data,KW_OPEN_NORMAL))
	{
		if(is_type(translation_data,scope))
		{
			ret=get_unary_expression_tree(NULL,OP_CAST);
			ret->value_type=parse_type_name(translation_data,scope);
			if(get_and_check(translation_data,KW_CLOSE_NORMAL))
			{
				ret->operand=parse_cast_expression(translation_data,scope);
				return (struct AST*)ret;
			}else
			{
				return (struct AST*)get_error_tree((struct AST*)ret);

			}

		}else
		{
			ret=(struct AST_Unary_Expression*)parse_expression(translation_data,scope);
			if(get_and_check(translation_data,KW_CLOSE_NORMAL))
			{
				return (struct AST*)ret;
			}else
			{
				return (struct AST*)get_error_tree((struct AST*)ret);
			}

		}
	}else
	{

		return parse_unary_expression(translation_data,scope);
	}
}
/*
unary-expression:
	++unary-expression
	--unary-expression
	+unary-expression
	-unary-expression
	!cast-expression
	~cast-expression
	*cast-expression
	&cast-expression
	sizeof ( typename )
	sizeof unary-expression
	postfix-expression

*/

struct AST* parse_unary_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	struct AST_Unary_Expression *hold;

	if(translation_data->tokens->size==0)
	{
		/*TODO error*/
		return (struct AST*)get_error_tree(NULL);
	}

	/*TODO make it iterative*/
	switch(((struct token*)translation_data->tokens->first->data)->type)
	{
		case KW_PLUSPLUS:
			chomp(translation_data);
			return (struct AST*)get_unary_expression_tree(parse_unary_expression(translation_data,scope),OP_PREFIX_INC);
		case KW_MINUSMINUS:
			chomp(translation_data);
			return (struct AST*)get_unary_expression_tree(parse_unary_expression(translation_data,scope),OP_PREFIX_DEC);
		case KW_PLUS:
			chomp(translation_data);
			return (struct AST*)get_unary_expression_tree(parse_cast_expression(translation_data,scope),OP_UNARY_PLUS);
		case KW_MINUS:
			chomp(translation_data);
			return (struct AST*)get_unary_expression_tree(parse_cast_expression(translation_data,scope),OP_UNARY_MINUS);
		case KW_EXCLAMATION:
			chomp(translation_data);
			return (struct AST*)get_unary_expression_tree(parse_cast_expression(translation_data,scope),OP_LOGICAL_NOT);
		case KW_TILDE:
			chomp(translation_data);
			return (struct AST*)get_unary_expression_tree(parse_cast_expression(translation_data,scope),OP_BITWISE_NOT);
		case KW_STAR:
			chomp(translation_data);
			return (struct AST*)get_unary_expression_tree(parse_cast_expression(translation_data,scope),OP_DEREFERENCE);
		case KW_AND:
			chomp(translation_data);
			return (struct AST*)get_unary_expression_tree(parse_cast_expression(translation_data,scope),OP_ADDR_OF);
		case KW_SIZEOF:
			chomp(translation_data);
			if(get_and_check(translation_data,KW_OPEN_NORMAL))
			{
				hold=get_unary_expression_tree(NULL,OP_CAST);
				hold->value_type=parse_type_name(translation_data,scope);
				hold->operand=parse_unary_expression(translation_data,scope);
				return (struct AST*)hold;
			}else
			{
				return (struct AST*)get_unary_expression_tree(parse_unary_expression(translation_data,scope),OP_SIZEOF);
			}
		default:
			return parse_postfix_expression(translation_data,scope);
	}

}
/*
multiplicative-expression:
	cast-expression ( ( * | / | % ) cast-expression )*
*/
struct AST* parse_multiplicative_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	struct AST *hold;
	hold=parse_cast_expression(translation_data,scope);
	while(translation_data->tokens->size!=0)
	{
		switch(((struct token*)translation_data->tokens->first->data)->type)
		{
			case KW_STAR:
				chomp(translation_data);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_cast_expression(translation_data,scope),OP_MUL);
				break;
			case KW_FORWARD_SLASH:
				chomp(translation_data);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_cast_expression(translation_data,scope),OP_DIV);
				break;
			case KW_PERCENT:
				chomp(translation_data);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_cast_expression(translation_data,scope),OP_REMAINDER);
				break;
			default:
				return hold;
		}
	}

	return hold;
}
/*
additive-expression:
	multiplicative-expression ( ( + | - ) multiplicative )*
*/
struct AST* parse_additive_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	
	struct AST *hold;
	hold=parse_multiplicative_expression(translation_data,scope);

	while(translation_data->tokens->size!=0)
	{
		switch(((struct token*)translation_data->tokens->first->data)->type)
		{
			case KW_PLUS:
				chomp(translation_data);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_multiplicative_expression(translation_data,scope),OP_ADDITION);
				break;
			case KW_MINUS:
				chomp(translation_data);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_multiplicative_expression(translation_data,scope),OP_SUBTRACTION);
				break;
			default:
				return hold;
		}
	}

	return hold;
}
/*
bitwise-shift:
	additive-expression ( ( << | >> ) additive-expression)*
*/
struct AST* parse_shift_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	
	struct AST *hold;
	hold=parse_additive_expression(translation_data,scope);

	while(translation_data->tokens->size!=0)
	{
		switch(((struct token*)translation_data->tokens->first->data)->type)
		{
			case KW_SHIFT_LEFT:
				chomp(translation_data);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_additive_expression(translation_data,scope),OP_SHIFT_LEFT);
				break;
			case KW_SHIFT_RIGHT:
				chomp(translation_data);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_additive_expression(translation_data,scope),OP_SHIFT_RIGHT);
				break;
			default:
				return hold;
		}
	}
	return hold;
}

/*
relational-expression:
	shift-expression ( ( < | > | <= | >= ) shift-expression )*
*/

struct AST* parse_relational_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	struct AST *hold;
	hold=parse_shift_expression(translation_data,scope);

	while(translation_data->tokens->size!=0)
	{
		switch(((struct token*)translation_data->tokens->first->data)->type)
		{
			case KW_LESS:
				chomp(translation_data);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_shift_expression(translation_data,scope),OP_LESS);
				break;
			case KW_LESS_EQ:
				chomp(translation_data);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_shift_expression(translation_data,scope),OP_LESS_EQ);
				break;
			case KW_MORE:
				chomp(translation_data);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_shift_expression(translation_data,scope),OP_GREATER);
				break;
			case KW_MORE_EQ:
				chomp(translation_data);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_shift_expression(translation_data,scope),OP_GREATER_EQ);
				break;
			default:
				return hold;
		}
	}
	return hold;
}


/*
equality-expression:
	realtional-expression ( ( == | != ) relational-expression )*
*/
struct AST* parse_equality_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	struct AST *hold;
	hold=parse_relational_expression(translation_data,scope);

	while(translation_data->tokens->size!=0)
	{
		switch(((struct token*)translation_data->tokens->first->data)->type)
		{
			case KW_EQEQ:
				chomp(translation_data);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_relational_expression(translation_data,scope),OP_EQUAL);
				break;
			case KW_NOT_EQ:
				chomp(translation_data);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_relational_expression(translation_data,scope),OP_NOT_EQUAL);
				break;
			default:
				return hold;
		}
	}
	return hold;
}
/*
and-expression:
	equality-expression ( & equality-expression ) *
*/
struct AST* parse_and_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	
	struct AST *hold;
	hold=parse_equality_expression(translation_data,scope);
	while(get_and_check(translation_data,KW_AND))
	{
		hold=(struct AST*)get_binary_expression_tree(hold,parse_equality_expression(translation_data,scope),OP_BITWISE_AND);
	}
	return hold;
}
/*
exclusive-or-expression:
	and-expression (^ and-expression)*

*/
struct AST* parse_exclusive_or_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	struct AST *hold;
	hold=parse_and_expression(translation_data,scope);
	while(get_and_check(translation_data,KW_HAT))
	{
		hold=(struct AST*)get_binary_expression_tree(hold,parse_and_expression(translation_data,scope),OP_BITWISE_XOR);
	}
	return hold;
	
}
/*
inclusive-or-expression:
	exclusive-or-expression (|exclusive-or-expression)*
*/
struct AST* parse_inclusive_or_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	
	struct AST *hold;
	hold=parse_exclusive_or_expression(translation_data,scope);
	while(get_and_check(translation_data,KW_PIPE))
	{
		hold=(struct AST*)get_binary_expression_tree(hold,parse_exclusive_or_expression(translation_data,scope),OP_BITWISE_OR);
	}
	return hold;
}
/*
logical-and-expression:
	inclusive-or-expression(&&inclusive-or-expression)*
*/
struct AST* parse_logical_and_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	
	struct AST *hold;
	hold=parse_inclusive_or_expression(translation_data,scope);
	while(get_and_check(translation_data,KW_AND_AND))
	{
		hold=(struct AST*)get_binary_expression_tree(hold,parse_inclusive_or_expression(translation_data,scope),OP_LOGICAL_AND);
	}
	return hold;
}
/*
logical-or-expression:
	logical-and-expression ( || logical-and-expression )*
*/
struct AST* parse_logical_or_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	
	struct AST *hold;
	hold=parse_logical_and_expression(translation_data,scope);
	while(get_and_check(translation_data,KW_AND_AND))
	{
		hold=(struct AST*)get_binary_expression_tree(hold,parse_logical_and_expression(translation_data,scope),OP_LOGICAL_OR);
	}
	return hold;
}

/*
conditional-expression:
	logical-or-expression
	logical-or-expression?expression:conditional-expression
*/
struct AST* parse_conditional_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	
	struct AST *hold;
	hold=parse_logical_or_expression(translation_data,scope);
	if(get_and_check(translation_data,KW_QUESTION))
	{
		hold=(struct AST*)get_conditional_expression_tree(hold,parse_expression(translation_data,scope),NULL);
		if(get_and_check(translation_data,KW_COLUMN))
		{
			((struct AST_Conditional_Expression*)hold)->right=parse_conditional_expression(translation_data,scope);
			return hold;
		}else
		{
			return (struct AST*)get_error_tree(hold);
		}
	}else
	{
		return hold;
	}
}
/*
assignment-expression:
	conditional-expression
	unary-expression ( ( = | += | -= | %= | /= | *= | >>= | <<= | &= | |= | ^= ) assignment-expression
*/
struct AST* parse_assignment_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	struct AST *hold;

	if(translation_data->tokens->size==0)
	{
		/*TODO error*/
		return (struct AST*)get_error_tree(NULL);
	}

	hold=parse_conditional_expression(translation_data,scope);
	if(translation_data->tokens->size==0)
		return hold;
												/*TODO make it iterative*/
	switch(((struct token*)translation_data->tokens->first->data)->type)
	{
		case KW_EQ:
			chomp(translation_data);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(translation_data,scope),OP_ASSIGN);
		case KW_PLUS_EQ:
			chomp(translation_data);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(translation_data,scope),OP_ADD_ASSIGN);
		case KW_MINUS_EQ:
			chomp(translation_data);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(translation_data,scope),OP_SUBTRACT_ASSIGN);
		case KW_PERCENT_EQ:
			chomp(translation_data);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(translation_data,scope),OP_REMAINDER_ASSIGN);
		case KW_DIV_EQ:
			chomp(translation_data);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(translation_data,scope),OP_DIV_ASSIGN);
		case KW_STAR_EQ:
			chomp(translation_data);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(translation_data,scope),OP_MULTIPLY_ASSIGN);
		case KW_SHIFT_RIGHT_EQ:
			chomp(translation_data);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(translation_data,scope),OP_SHIFT_RIGHT_ASSIGN);
		case KW_SHIFT_LEFT_EQ:
			chomp(translation_data);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(translation_data,scope),OP_SHIFT_LEFT_ASSIGN);
		case KW_AND_EQ:
			chomp(translation_data);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(translation_data,scope),OP_AND_ASSIGN);
		case KW_PIPE_EQ:
			chomp(translation_data);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(translation_data,scope),OP_PIPE_ASSIGN);
		case KW_HAT_EQ:
			chomp(translation_data);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(translation_data,scope),OP_XOR_ASSIGN);
		default:
			return hold;
	}

}
/*
comma-expression:
	assignment-expression(,assignment-expression)*
*/
struct AST* parse_comma_expression(struct Translation_Data *translation_data,struct Scope *scope)
{
	struct AST *hold;
	hold=parse_assignment_expression(translation_data,scope);
	while(get_and_check(translation_data,KW_COMMA))
	{
		hold=(struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(translation_data,scope),OP_COMMA);
	}
	return hold;
}

#endif
