#ifndef PARSE_GCC_STATEMENT_C
#define PARSE_GCC_STATEMENT_C PARSE_GCC_STATEMENT_C
#include "parse_statement.h"


struct AST* parse_statement(struct Translation_Data* translation_data,struct Scope *scope)
{

	if(translation_data->tokens->size==0)
		return NULL;

	

	switch(kw_get(translation_data))
	{
		case KW_NOTYPE:
			return (struct AST*)get_error_tree(NULL);

		case KW_OPEN_CURLY:
			chomp(translation_data);
			return parse_finish_compound_statement(translation_data,scope);
		case KW_IF:
			chomp(translation_data);
			return parse_finish_if_statement(translation_data,scope);		
		case KW_SWITCH:
			chomp(translation_data);
			return parse_finish_switch_statement(translation_data,scope);
		case KW_WHILE:
			chomp(translation_data);
			return parse_finish_while_statement(translation_data,scope);
		case KW_DO:
			chomp(translation_data);
			return parse_finish_do_while_statement(translation_data,scope);
		case KW_FOR:
			chomp(translation_data);
			return parse_finish_for_statement(translation_data,scope);
		case KW_GOTO:
			chomp(translation_data);
			return parse_finish_goto_statement(translation_data,scope);
		case KW_CASE:
			chomp(translation_data);
			return parse_finish_labeled_statement(translation_data,scope,ST_CASE);
		case KW_DEFAULT:
			chomp(translation_data);
			return parse_finish_default_statement(translation_data,scope);
		case KW_ID:
			/*TODO check if id is in function scope*/
			if(0)
			{
				return parse_finish_labeled_statement(translation_data,scope,ST_LABEL);
			}else
			{
				return parse_expression_statement(translation_data,scope);
			}
		case KW_CONTINUE:
			chomp(translation_data);
			return parse_finish_continue_statement(translation_data);
		case KW_BREAK:
			chomp(translation_data);
			return parse_finish_break_statement(translation_data);
		case KW_RETURN:
			chomp(translation_data);
			return parse_finish_return_statement(translation_data,scope);
		default:
			return parse_expression_statement(translation_data,scope);

	}

}
/*
	( declaration | statement )* }
*/
struct AST* parse_finish_compound_statement(struct Translation_Data* translation_data,struct Scope *scope)
{
	struct AST_Compound_Statement *hold;
	hold=get_compound_statement_tree(scope);
	while(!get_and_check(translation_data,KW_CLOSE_CURLY))
	{
		if(is_type(translation_data,hold->scope))
		{
			parse_declaration(translation_data,hold->scope,&hold->components,0);	
		}else
		{
			Queue_Push(&hold->components,parse_statement(translation_data,hold->scope));	
		}
		if(has_new_errors(translation_data))
			chase_next_semicolumn(translation_data);
	}

	return (struct AST*)hold;
}
/*
   ( expression ) statement
   ( expression ) statement else statement
*/
struct AST* parse_finish_if_statement(struct Translation_Data* translation_data,struct Scope *scope)
{
	struct AST_If_Statement *hold;
	hold=get_if_statement_tree();
	if(get_and_check(translation_data,KW_OPEN_NORMAL))
	{
		hold->condition=parse_expression(translation_data,scope);
		if(get_and_check(translation_data,KW_CLOSE_NORMAL))
		{
			hold->body_statement=parse_statement(translation_data,scope);
		}else
		{
			return (struct AST*)get_error_tree((struct AST*)hold);
		}
		if(get_and_check(translation_data,KW_ELSE)) 
		{
			hold->else_statement=parse_statement(translation_data,scope);
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
struct AST* parse_finish_switch_statement(struct Translation_Data* translation_data,struct Scope *scope)
{
	struct AST_Switch_Statement *hold;
	hold=get_switch_statement_tree();
	if(get_and_check(translation_data,KW_OPEN_NORMAL))
	{
		hold->condition=parse_expression(translation_data,scope);
		if(get_and_check(translation_data,KW_CLOSE_NORMAL))
		{
			hold->body_statement=parse_statement(translation_data,scope);
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
struct AST* parse_finish_while_statement(struct Translation_Data* translation_data,struct Scope *scope)
{
	struct AST_While_Statement *hold;
	hold=get_while_statement_tree();
	if(get_and_check(translation_data,KW_OPEN_NORMAL))
	{
		hold->condition=parse_expression(translation_data,scope);
		if(get_and_check(translation_data,KW_CLOSE_NORMAL))
		{
			hold->body_statement=parse_statement(translation_data,scope);
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
struct AST* parse_finish_do_while_statement(struct Translation_Data* translation_data,struct Scope *scope)
{
	struct AST_Do_While_Statement *hold;
	hold=get_do_while_statement_tree();
	hold->body_statement=parse_statement(translation_data,scope);
	if(get_and_check(translation_data,KW_WHILE) && get_and_check(translation_data,KW_OPEN_NORMAL))
	{
		hold->condition=parse_expression(translation_data,scope);

		if(get_and_check(translation_data,KW_CLOSE_NORMAL) && get_and_check(translation_data,KW_SEMI_COLUMN))
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
struct AST* parse_finish_for_statement(struct Translation_Data* translation_data,struct Scope *scope)
{
	struct AST_For_Statement *hold;
	hold=get_for_statement_tree();
	if(!get_and_check(translation_data,KW_OPEN_NORMAL))
		return (struct AST*)get_error_tree((struct AST*)hold);

	hold->initialisation=parse_expression_statement(translation_data,scope);
	hold->condition=parse_expression_statement(translation_data,scope);
	if(get_and_check(translation_data,KW_CLOSE_NORMAL))
	{
		hold->update=get_nop_tree();
	}else
	{
		hold->update=parse_expression(translation_data,scope);
		if(!get_and_check(translation_data,KW_CLOSE_NORMAL))
			return (struct AST*)get_error_tree((struct AST*)hold);
	}
	hold->body_statement=parse_statement(translation_data,scope);
	return (struct AST*)hold;
}
/*
	id ;
*/
struct AST* parse_finish_goto_statement(struct Translation_Data* translation_data,struct Scope *scope)
{
	struct AST* ret;

	if(check(translation_data,KW_ID,0))
	{
		ret=(struct AST*)get_goto_statement_tree(Queue_Pop(translation_data->tokens),scope);
		if(get_and_check(translation_data,KW_SEMI_COLUMN))
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
struct AST* parse_finish_continue_statement(struct Translation_Data* translation_data)
{
	struct AST *hold;
	hold=malloc(sizeof(struct AST));
	hold->type=ST_CONTINUE;
	if(get_and_check(translation_data,KW_SEMI_COLUMN))
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
struct AST* parse_finish_break_statement(struct Translation_Data* translation_data)
{
	struct AST *hold;
	hold=malloc(sizeof(struct AST));
	hold->type=ST_BREAK;
	if(get_and_check(translation_data,KW_SEMI_COLUMN))
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
struct AST* parse_finish_labeled_statement(struct Translation_Data* translation_data,struct Scope *scope,enum AST_Type type)
{
	struct AST_Labeled_Statement *ret;
	if(check(translation_data,KW_ID,0))
	{
		ret=get_labeled_statement_tree(Queue_Pop(translation_data->tokens),NULL,scope,type);
		if(get_and_check(translation_data,KW_COLUMN))
		{
			ret->statement=parse_statement(translation_data,scope);
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
struct AST* parse_finish_default_statement(struct Translation_Data* translation_data,struct Scope *scope)
{
	struct AST_Labeled_Statement *ret;
	ret=get_labeled_statement_tree(NULL,NULL,scope,ST_DEFAULT);
	if(get_and_check(translation_data,KW_COLUMN))
	{
		ret->statement=parse_statement(translation_data,scope);
		return (struct AST*)ret;
	}else
	{
		return (struct AST*)get_error_tree((struct AST*)ret);
	}
}
/*
	[ expression ] ;

*/
struct AST* parse_finish_return_statement(struct Translation_Data* translation_data,struct Scope *scope)
{
	struct AST_Return_Statement *hold;
	if(get_and_check(translation_data,KW_SEMI_COLUMN))
	{
		hold=get_return_statement_tree(get_nop_tree());
		return (struct AST*)hold;
	}


	hold=get_return_statement_tree(parse_expression(translation_data,scope));
	if(get_and_check(translation_data,KW_SEMI_COLUMN))
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
struct AST* parse_expression_statement(struct Translation_Data* translation_data,struct Scope *scope)
{
	struct AST *hold;
	if(get_and_check(translation_data,KW_SEMI_COLUMN))
	{
		hold=get_nop_tree();
		return (struct AST*)hold;
	}
	hold=parse_expression(translation_data,scope);
	if(get_and_check(translation_data,KW_SEMI_COLUMN))
	{
		return hold;
	}else
	{
		return (struct AST*)get_error_tree(hold);
	}
}
void chase_next_semicolumn(struct Translation_Data *translation_data)
{
/*chase ; and start parsing next declaration*/
	while(!get_and_check(translation_data,KW_SEMI_COLUMN) &&
		       	translation_data->tokens->size>0)
	{
		chomp(translation_data);
	}
}
#endif
