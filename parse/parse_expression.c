#ifndef GCC_PARSE_EXPR_C
#define GCC_PARSE_EXPR_C GCC_PARSE_EXPR_C
#include "parse_expression.h"



struct AST* parse_expression(struct Queue *tokens,struct Scope *scope)
{
	return parse_comma_expression(tokens,scope);
}

struct AST* parse_const_expression(struct Queue *tokens,struct Scope *scope)
{
	return parse_comma_expression(tokens,scope);
}
/*
primary-expression:
	number
	string
	id
	(e)
	generic-selection

*/
struct AST* parse_primary_expression(struct Queue *tokens,struct Scope *scope)
{
	struct token *hold_token;
	struct AST *hold;
	
	if(tokens->size==0)
	{
		/*TODO error*/
		return (struct AST*)get_error_tree(NULL);
	}
	hold_token=Queue_Pop(tokens);
	switch(hold_token->type)
	{
		case KW_NUMBER:
		case KW_STRING:
			return (struct AST*)get_rvalue_expression_tree(hold_token);
		case KW_ID:
			return (struct AST*)get_lvalue_expression_tree(hold_token,scope);
		case KW_OPEN_NORMAL:
			hold=parse_expression(tokens,scope);
			if(get_and_check(tokens,KW_CLOSE_NORMAL))
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
struct AST_Function_Expression* parse_arglist(struct Queue *tokens,struct Scope *scope,struct AST* id)
{
	struct AST_Function_Expression *ret;
	ret=get_function_expression_tree(id,scope);
	ret->id=id;
	if(get_and_check(tokens,KW_CLOSE_NORMAL))
	{
		return ret;
	}
	do
	{
		Queue_Push(&ret->arguments,parse_assignment_expression(tokens,scope));
	} while(get_and_check(tokens,KW_COMMA));
	if(get_and_check(tokens,KW_CLOSE_NORMAL))
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
struct AST* parse_postfix_expression(struct Queue *tokens,struct Scope *scope)
{
	
	struct AST *hold;
	struct AST *hold_expr;

	
	hold=parse_primary_expression(tokens,scope);

	while(tokens->size!=0)
	{
		switch(((struct token*)tokens->first->data)->type)
		{
			case KW_PLUSPLUS:
				chomp(tokens);
				hold=(struct AST*)get_unary_expression_tree(hold,OP_POSTFIX_INC);
				break;
			case KW_MINUSMINUS:
				chomp(tokens);
				hold=(struct AST*)get_unary_expression_tree(hold,OP_POSTFIX_DEC);
				break;
			case KW_DOT:
				chomp(tokens);
				if(check(tokens,KW_ID,0))
				{
					hold=(struct AST*)get_binary_expression_tree(hold,(struct AST*)get_lvalue_expression_tree(Queue_Pop(tokens),scope),OP_MEMBER);
				}
				break;
			case KW_ARROW:
				chomp(tokens);
				if(check(tokens,KW_ID,0))
				{
					hold=(struct AST*)get_binary_expression_tree(hold,(struct AST*)get_lvalue_expression_tree(Queue_Pop(tokens),scope),OP_MEMBER_TROUGH_PTR);
				}
				break;
			case KW_OPEN_SQUARE:

				chomp(tokens);
				if(get_and_check(tokens,KW_CLOSE_SQUARE))
				{
					hold=(struct AST*)get_binary_expression_tree(hold,NULL,OP_ARR_SUBSCRIPT);
				}else
				{
					hold_expr=parse_expression(tokens,scope);
					hold=(struct AST*)get_binary_expression_tree(hold,hold_expr,OP_ARR_SUBSCRIPT);
					if(!get_and_check(tokens,KW_CLOSE_SQUARE))
					{
						return (struct AST*)get_error_tree(hold);
					}
				}
				break;
			case KW_OPEN_NORMAL:
				chomp(tokens);
				return (struct AST*)parse_arglist(tokens,scope,hold);
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
struct AST* parse_cast_expression(struct Queue *tokens,struct Scope *scope)
{
	struct AST_Unary_Expression *ret;
	if(get_and_check(tokens,KW_OPEN_NORMAL))
	{
		if(is_type(tokens,scope))
		{
			ret=get_unary_expression_tree(NULL,OP_CAST);
			ret->value_type=parse_type_name(tokens,scope);
			if(get_and_check(tokens,KW_CLOSE_NORMAL))
			{
				ret->operand=parse_cast_expression(tokens,scope);
				return (struct AST*)ret;
			}else
			{
				return (struct AST*)get_error_tree((struct AST*)ret);

			}

		}else
		{
			return (struct AST*)parse_unary_expression(tokens,scope);
		}
	}else
	{

		return parse_unary_expression(tokens,scope);
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

struct AST* parse_unary_expression(struct Queue *tokens,struct Scope *scope)
{
	struct AST_Unary_Expression *hold;

	if(tokens->size==0)
	{
		/*TODO error*/
		return (struct AST*)get_error_tree(NULL);
	}

	/*TODO make it iterative*/
	switch(((struct token*)tokens->first->data)->type)
	{
		case KW_PLUSPLUS:
			chomp(tokens);
			return (struct AST*)get_unary_expression_tree(parse_unary_expression(tokens,scope),OP_PREFIX_INC);
		case KW_MINUSMINUS:
			chomp(tokens);
			return (struct AST*)get_unary_expression_tree(parse_unary_expression(tokens,scope),OP_PREFIX_DEC);
		case KW_PLUS:
			chomp(tokens);
			return (struct AST*)get_unary_expression_tree(parse_cast_expression(tokens,scope),OP_UNARY_PLUS);
		case KW_MINUS:
			chomp(tokens);
			return (struct AST*)get_unary_expression_tree(parse_cast_expression(tokens,scope),OP_UNARY_MINUS);
		case KW_EXCLAMATION:
			chomp(tokens);
			return (struct AST*)get_unary_expression_tree(parse_cast_expression(tokens,scope),OP_LOGICAL_NOT);
		case KW_TILDE:
			chomp(tokens);
			return (struct AST*)get_unary_expression_tree(parse_cast_expression(tokens,scope),OP_BITWISE_NOT);
		case KW_STAR:
			chomp(tokens);
			return (struct AST*)get_unary_expression_tree(parse_cast_expression(tokens,scope),OP_DEREFERENCE);
		case KW_AND:
			chomp(tokens);
			return (struct AST*)get_unary_expression_tree(parse_cast_expression(tokens,scope),OP_ADDR_OF);
		case KW_SIZEOF:
			chomp(tokens);
			if(get_and_check(tokens,KW_OPEN_NORMAL))
			{
				hold=get_unary_expression_tree(NULL,OP_CAST);
				hold->value_type=parse_type_name(tokens,scope);
				hold->operand=parse_unary_expression(tokens,scope);
				return (struct AST*)hold;
			}else
			{
				return (struct AST*)get_unary_expression_tree(parse_unary_expression(tokens,scope),OP_SIZEOF);
			}
		default:
			return parse_postfix_expression(tokens,scope);
	}

}
/*
multiplicative-expression:
	cast-expression ( ( * | / | % ) cast-expression )*
*/
struct AST* parse_multiplicative_expression(struct Queue *tokens,struct Scope *scope)
{
	struct AST *hold;
	hold=parse_cast_expression(tokens,scope);
	while(tokens->size!=0)
	{
		switch(((struct token*)tokens->first->data)->type)
		{
			case KW_STAR:
				chomp(tokens);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_cast_expression(tokens,scope),OP_MUL);
				break;
			case KW_FORWARD_SLASH:
				chomp(tokens);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_cast_expression(tokens,scope),OP_DIV);
				break;
			case KW_PERCENT:
				chomp(tokens);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_cast_expression(tokens,scope),OP_REMAINDER);
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
struct AST* parse_additive_expression(struct Queue *tokens,struct Scope *scope)
{
	
	struct AST *hold;
	hold=parse_multiplicative_expression(tokens,scope);

	while(tokens->size!=0)
	{
		switch(((struct token*)tokens->first->data)->type)
		{
			case KW_PLUS:
				chomp(tokens);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_multiplicative_expression(tokens,scope),OP_ADDITION);
				break;
			case KW_MINUS:
				chomp(tokens);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_multiplicative_expression(tokens,scope),OP_SUBTRACTION);
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
struct AST* parse_shift_expression(struct Queue *tokens,struct Scope *scope)
{
	
	struct AST *hold;
	hold=parse_additive_expression(tokens,scope);

	while(tokens->size!=0)
	{
		switch(((struct token*)tokens->first->data)->type)
		{
			case KW_SHIFT_LEFT:
				chomp(tokens);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_additive_expression(tokens,scope),OP_SHIFT_LEFT);
				break;
			case KW_SHIFT_RIGHT:
				chomp(tokens);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_additive_expression(tokens,scope),OP_SHIFT_RIGHT);
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

struct AST* parse_relational_expression(struct Queue *tokens,struct Scope *scope)
{
	struct AST *hold;
	hold=parse_shift_expression(tokens,scope);

	while(tokens->size!=0)
	{
		switch(((struct token*)tokens->first->data)->type)
		{
			case KW_LESS:
				chomp(tokens);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_shift_expression(tokens,scope),OP_LESS);
				break;
			case KW_LESS_EQ:
				chomp(tokens);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_shift_expression(tokens,scope),OP_LESS_EQ);
				break;
			case KW_MORE:
				chomp(tokens);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_shift_expression(tokens,scope),OP_GREATER);
				break;
			case KW_MORE_EQ:
				chomp(tokens);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_shift_expression(tokens,scope),OP_GREATER_EQ);
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
struct AST* parse_equality_expression(struct Queue *tokens,struct Scope *scope)
{
	struct AST *hold;
	hold=parse_relational_expression(tokens,scope);

	while(tokens->size!=0)
	{
		switch(((struct token*)tokens->first->data)->type)
		{
			case KW_EQEQ:
				chomp(tokens);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_relational_expression(tokens,scope),OP_EQUAL);
				break;
			case KW_NOT_EQ:
				chomp(tokens);
				hold=(struct AST*)get_binary_expression_tree(hold,parse_relational_expression(tokens,scope),OP_NOT_EQUAL);
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
struct AST* parse_and_expression(struct Queue *tokens,struct Scope *scope)
{
	
	struct AST *hold;
	hold=parse_equality_expression(tokens,scope);
	while(get_and_check(tokens,KW_AND))
	{
		hold=(struct AST*)get_binary_expression_tree(hold,parse_equality_expression(tokens,scope),OP_BITWISE_AND);
	}
	return hold;
}
/*
exclusive-or-expression:
	and-expression (^ and-expression)*

*/
struct AST* parse_exclusive_or_expression(struct Queue *tokens,struct Scope *scope)
{
	struct AST *hold;
	hold=parse_and_expression(tokens,scope);
	while(get_and_check(tokens,KW_HAT))
	{
		hold=(struct AST*)get_binary_expression_tree(hold,parse_and_expression(tokens,scope),OP_BITWISE_XOR);
	}
	return hold;
	
}
/*
inclusive-or-expression:
	exclusive-or-expression (|exclusive-or-expression)*
*/
struct AST* parse_inclusive_or_expression(struct Queue *tokens,struct Scope *scope)
{
	
	struct AST *hold;
	hold=parse_exclusive_or_expression(tokens,scope);
	while(get_and_check(tokens,KW_PIPE))
	{
		hold=(struct AST*)get_binary_expression_tree(hold,parse_exclusive_or_expression(tokens,scope),OP_BITWISE_OR);
	}
	return hold;
}
/*
logical-and-expression:
	inclusive-or-expression(&&inclusive-or-expression)*
*/
struct AST* parse_logical_and_expression(struct Queue *tokens,struct Scope *scope)
{
	
	struct AST *hold;
	hold=parse_inclusive_or_expression(tokens,scope);
	while(get_and_check(tokens,KW_AND_AND))
	{
		hold=(struct AST*)get_binary_expression_tree(hold,parse_inclusive_or_expression(tokens,scope),OP_LOGICAL_AND);
	}
	return hold;
}
/*
logical-or-expression:
	logical-and-expression ( || logical-and-expression )*
*/
struct AST* parse_logical_or_expression(struct Queue *tokens,struct Scope *scope)
{
	
	struct AST *hold;
	hold=parse_logical_and_expression(tokens,scope);
	while(get_and_check(tokens,KW_AND_AND))
	{
		hold=(struct AST*)get_binary_expression_tree(hold,parse_logical_and_expression(tokens,scope),OP_LOGICAL_OR);
	}
	return hold;
}

/*
conditional-expression:
	logical-or-expression
	logical-or-expression?expression:conditional-expression
*/
struct AST* parse_conditional_expression(struct Queue *tokens,struct Scope *scope)
{
	
	struct AST *hold;
	hold=parse_logical_or_expression(tokens,scope);
	if(get_and_check(tokens,KW_QUESTION))
	{
		hold=(struct AST*)get_conditional_expression_tree(hold,parse_expression(tokens,scope),NULL);
		if(get_and_check(tokens,KW_COLUMN))
		{
			((struct AST_Conditional_Expression*)hold)->right=parse_conditional_expression(tokens,scope);
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
struct AST* parse_assignment_expression(struct Queue *tokens,struct Scope *scope)
{
	struct AST *hold;

	if(tokens->size==0)
	{
		/*TODO error*/
		return (struct AST*)get_error_tree(NULL);
	}

	hold=parse_conditional_expression(tokens,scope);
	if(tokens->size==0)
		return hold;
												/*TODO make it iterative*/
	switch(((struct token*)tokens->first->data)->type)
	{
		case KW_EQ:
			chomp(tokens);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(tokens,scope),OP_ASSIGN);
		case KW_PLUS_EQ:
			chomp(tokens);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(tokens,scope),OP_ADD_ASSIGN);
		case KW_MINUS_EQ:
			chomp(tokens);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(tokens,scope),OP_SUBTRACT_ASSIGN);
		case KW_PERCENT_EQ:
			chomp(tokens);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(tokens,scope),OP_REMAINDER_ASSIGN);
		case KW_DIV_EQ:
			chomp(tokens);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(tokens,scope),OP_DIV_ASSIGN);
		case KW_STAR_EQ:
			chomp(tokens);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(tokens,scope),OP_MULTIPLY_ASSIGN);
		case KW_SHIFT_RIGHT_EQ:
			chomp(tokens);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(tokens,scope),OP_SHIFT_RIGHT_ASSIGN);
		case KW_SHIFT_LEFT_EQ:
			chomp(tokens);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(tokens,scope),OP_SHIFT_LEFT_ASSIGN);
		case KW_AND_EQ:
			chomp(tokens);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(tokens,scope),OP_AND_ASSIGN);
		case KW_PIPE_EQ:
			chomp(tokens);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(tokens,scope),OP_PIPE_ASSIGN);
		case KW_HAT_EQ:
			chomp(tokens);
			return (struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(tokens,scope),OP_XOR_ASSIGN);
		default:
			return hold;
	}

}
/*
comma-expression:
	assignment-expression(,assignment-expression)*
*/
struct AST* parse_comma_expression(struct Queue *tokens,struct Scope *scope)
{
	struct AST *hold;
	hold=parse_assignment_expression(tokens,scope);
	while(get_and_check(tokens,KW_COMMA))
	{
		hold=(struct AST*)get_binary_expression_tree(hold,parse_assignment_expression(tokens,scope),OP_COMMA);
	}
	return hold;
}

#endif
