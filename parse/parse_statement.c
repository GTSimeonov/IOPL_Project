#ifndef PARSE_GCC_STATEMENT_C
#define PARSE_GCC_STATEMENT_C PARSE_GCC_STATEMENT_C
#include "parse_statement.h"
#include "parse_declaration.h"


struct AST* parse_statement(struct Queue* tokens,struct Scope *scope)
{

	if(tokens->size==0)
		return NULL;

	

	switch(kw_get(tokens))
	{
		case KW_NOTYPE:
			return (struct AST*)get_error_tree(NULL);

		case KW_OPEN_CURLY:
			chomp(tokens);
			return parse_finish_compound_statement(tokens,scope);
		case KW_IF:
			chomp(tokens);
			return parse_finish_if_statement(tokens,scope);		
		case KW_SWITCH:
			chomp(tokens);
			return parse_finish_switch_statement(tokens,scope);
		case KW_WHILE:
			chomp(tokens);
			return parse_finish_while_statement(tokens,scope);
		case KW_DO:
			chomp(tokens);
			return parse_finish_do_while_statement(tokens,scope);
		case KW_FOR:
			chomp(tokens);
			return parse_finish_for_statement(tokens,scope);
		case KW_GOTO:
			chomp(tokens);
			return parse_finish_goto_statement(tokens,scope);
		case KW_CASE:
			chomp(tokens);
			return parse_finish_labeled_statement(tokens,scope,ST_CASE);
		case KW_DEFAULT:
			chomp(tokens);
			return parse_finish_default_statement(tokens,scope);
		case KW_ID:
			/*TODO check if id is in function scope*/
			if(0)
			{
				return parse_finish_labeled_statement(tokens,scope,ST_LABEL);
			}else
			{
				return parse_expression_statement(tokens,scope);
			}
		case KW_CONTINUE:
			chomp(tokens);
			return parse_finish_continue_statement(tokens);
		case KW_BREAK:
			chomp(tokens);
			return parse_finish_break_statement(tokens);
		case KW_RETURN:
			chomp(tokens);
			return parse_finish_return_statement(tokens,scope);
		default:
			return parse_expression_statement(tokens,scope);

	}

}
/*
	( declaration | statement )* }
*/
struct AST* parse_finish_compound_statement(struct Queue* tokens,struct Scope *scope)
{
	struct AST_Compound_Statement *hold;
	hold=get_compound_statement_tree(scope);
	while(!get_and_check(tokens,KW_CLOSE_CURLY))
	{
		if(is_type(tokens,hold->scope))
		{
			parse_declaration(tokens,hold->scope,&hold->components,0);	
		}else
		{
			Queue_Push(&hold->components,parse_statement(tokens,hold->scope));	
		}
	}

	return (struct AST*)hold;
}
/*
   ( expression ) statement
   ( expression ) statement else statement
*/
struct AST* parse_finish_if_statement(struct Queue* tokens,struct Scope *scope)
{
	struct AST_If_Statement *hold;
	hold=get_if_statement_tree();
	if(get_and_check(tokens,KW_OPEN_NORMAL))
	{
		hold->condition=parse_expression(tokens,scope);
		if(get_and_check(tokens,KW_CLOSE_NORMAL))
		{
			hold->body_statement=parse_statement(tokens,scope);
		}else
		{
			return (struct AST*)get_error_tree((struct AST*)hold);
		}
		if(get_and_check(tokens,KW_ELSE)) 
		{
			hold->else_statement=parse_statement(tokens,scope);
			return (struct AST*)hold;
		}else
		{
			hold->else_statement=NULL;
			return (struct AST*)hold;
		}
	}else
	{
		return (struct AST*)get_error_tree((struct AST*)hold);
	}
	return (struct AST*)hold;
}
/*
	( expression ) statement
*/
struct AST* parse_finish_switch_statement(struct Queue* tokens,struct Scope *scope)
{
	struct AST_Switch_Statement *hold;
	hold=get_switch_statement_tree();
	if(get_and_check(tokens,KW_OPEN_NORMAL))
	{
		hold->condition=parse_expression(tokens,scope);
		if(get_and_check(tokens,KW_CLOSE_NORMAL))
		{
			hold->body_statement=parse_statement(tokens,scope);
		}else
		{
			return (struct AST*)get_error_tree((struct AST*)hold);
		}
	}else
	{
		return (struct AST*)get_error_tree((struct AST*)hold);
	}

	return (struct AST*)hold;
}
/*
 	( expression ) statement

*/
struct AST* parse_finish_while_statement(struct Queue* tokens,struct Scope *scope)
{
	struct AST_While_Statement *hold;
	hold=get_while_statement_tree();
	if(get_and_check(tokens,KW_OPEN_NORMAL))
	{
		hold->condition=parse_expression(tokens,scope);
		if(get_and_check(tokens,KW_CLOSE_NORMAL))
		{
			hold->body_statement=parse_statement(tokens,scope);
		}else
		{
			return (struct AST*)get_error_tree((struct AST*)hold);
		}
	}else
	{
		return (struct AST*)get_error_tree((struct AST*)hold);
	}

	return (struct AST*)hold;
}
/*
	statement while ( expression ) ;
*/
struct AST* parse_finish_do_while_statement(struct Queue* tokens,struct Scope *scope)
{
	struct AST_Do_While_Statement *hold;
	hold=get_do_while_statement_tree();
	hold->body_statement=parse_statement(tokens,scope);
	if(get_and_check(tokens,KW_WHILE) && get_and_check(tokens,KW_OPEN_NORMAL))
	{
		hold->condition=parse_expression(tokens,scope);

		if(get_and_check(tokens,KW_CLOSE_NORMAL) && get_and_check(tokens,KW_SEMI_COLUMN))
		{
			return (struct AST*)hold;
		}else
		{
			return (struct AST*)get_error_tree((struct AST*)hold);
		}
	}else
	{
		return (struct AST*)get_error_tree((struct AST*)hold);
	}

}
/*
	( [ expression ] ; [ expression ] ; [ expression ] ) statement
*/
struct AST* parse_finish_for_statement(struct Queue* tokens,struct Scope *scope)
{
	struct AST_For_Statement *hold;
	hold=get_for_statement_tree();
	if(!get_and_check(tokens,KW_OPEN_NORMAL))
		return (struct AST*)get_error_tree((struct AST*)hold);

	hold->initialisation=parse_expression_statement(tokens,scope);
	hold->condition=parse_expression_statement(tokens,scope);
	if(get_and_check(tokens,KW_CLOSE_NORMAL))
	{
		hold->update=get_nop_tree();
	}else
	{
		hold->update=parse_expression(tokens,scope);
		if(!get_and_check(tokens,KW_CLOSE_NORMAL))
			return (struct AST*)get_error_tree((struct AST*)hold);
	}
	hold->body_statement=parse_statement(tokens,scope);
	return (struct AST*)hold;
}
/*
	id ;
*/
struct AST* parse_finish_goto_statement(struct Queue* tokens,struct Scope *scope)
{
	struct AST* ret;

	if(check(tokens,KW_ID,0))
	{
		ret=(struct AST*)get_goto_statement_tree(Queue_Pop(tokens),scope);
		if(get_and_check(tokens,KW_SEMI_COLUMN))
		{
			return ret;
		}else
		{
			return (struct AST*)get_error_tree(ret);
		}
	}
	else
	{
		return (struct AST*)get_error_tree((struct AST*)get_goto_statement_tree(NULL,NULL));
	}

}
/*
   ;
*/
struct AST* parse_finish_continue_statement(struct Queue* tokens)
{
	struct AST *hold;
	hold=malloc(sizeof(struct AST));
	hold->type=ST_CONTINUE;
	if(get_and_check(tokens,KW_SEMI_COLUMN))
	{
		return hold;
	}else
	{
		return (struct AST*)get_error_tree(hold);
	}
}
/*
   ;
*/
struct AST* parse_finish_break_statement(struct Queue* tokens)
{
	struct AST *hold;
	hold=malloc(sizeof(struct AST));
	hold->type=ST_BREAK;
	if(get_and_check(tokens,KW_SEMI_COLUMN))
	{
		return hold;
	}else
	{
		return (struct AST*)get_error_tree(hold);
	}
}
/*
   id:
   statement
*/
struct AST* parse_finish_labeled_statement(struct Queue* tokens,struct Scope *scope,enum AST_Type type)
{
	struct AST_Labeled_Statement *ret;
	if(check(tokens,KW_ID,0))
	{
		ret=get_labeled_statement_tree(Queue_Pop(tokens),NULL,scope,type);
		if(get_and_check(tokens,KW_COLUMN))
		{
			ret->statement=parse_statement(tokens,scope);
			return (struct AST*)ret;
		}else
		{
			return (struct AST*)get_error_tree((struct AST*)ret);
		}
	}else
	{
		return (struct AST*)get_error_tree(NULL);
	}
}
/*
	:
	statement
   */
struct AST* parse_finish_default_statement(struct Queue* tokens,struct Scope *scope)
{
	struct AST_Labeled_Statement *ret;
	ret=get_labeled_statement_tree(NULL,NULL,scope,ST_DEFAULT);
	if(get_and_check(tokens,KW_COLUMN))
	{
		ret->statement=parse_statement(tokens,scope);
		return (struct AST*)ret;
	}else
	{
		return (struct AST*)get_error_tree((struct AST*)ret);
	}
}
/*
	[ expression ] ;

*/
struct AST* parse_finish_return_statement(struct Queue* tokens,struct Scope *scope)
{
	struct AST_Return_Statement *hold;
	if(get_and_check(tokens,KW_SEMI_COLUMN))
	{
		hold=get_return_statement_tree(get_nop_tree());
		return (struct AST*)hold;
	}


	hold=get_return_statement_tree(parse_expression(tokens,scope));
	if(get_and_check(tokens,KW_SEMI_COLUMN))
	{
		return (struct AST*)hold;
	}else
	{
		return (struct AST*)get_error_tree((struct AST*)hold);
	}
}
/*
   	
	[ expression ] ;

*/
struct AST* parse_expression_statement(struct Queue* tokens,struct Scope *scope)
{
	struct AST *hold;
	if(get_and_check(tokens,KW_SEMI_COLUMN))
	{
		hold=get_nop_tree();
		return (struct AST*)hold;
	}
	hold=parse_expression(tokens,scope);
	if(get_and_check(tokens,KW_SEMI_COLUMN))
	{
		return hold;
	}else
	{
		return (struct AST*)get_error_tree(hold);
	}
}
#endif
