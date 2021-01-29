#ifndef GCC_PARSE_TRANSLATION_C
#define GCC_PARSE_TRANSLATION_C GCC_PARSE_TRANSLATION_C
#include "parse_declaration.h"
#include "parse_statement.h"
/*

  function-definition:
	  [ declaration-specifiers ] declarator
		    [ declaration-list ] compound-statement
*/

struct AST* parse_function_definition(struct Queue *tokens,struct Scope *scope)
{
	struct AST_Function_Definition *ret;
	ret=get_function_definition_tree(scope);
	
	if(is_type(tokens,scope))
		ret->base_type=parse_declaration_specifiers(tokens,scope);
	else
		ret->base_type=NULL;


	ret->declarator=get_declarator(ret->base_type);
	parse_declarator(tokens,ret->declarator,scope);
	/*TODO*/
	    //[ declaration-list ] compound-statement
	if(get_and_check(tokens,KW_OPEN_CURLY))
	{
		ret->body_statement=parse_finish_compound_statement(tokens,scope);
	}else
	{
		return (struct AST*)get_error_tree((struct AST*)ret);
	}

	return (struct AST*)ret;
}

/*
	translation_unit_step:
		declaration
		function-definition

	delcaration:
		declaration-specifiers [ init-declarator-list ] ;

*/
struct AST* parse_translation_unit_step(struct Queue *tokens,struct Scope *scope)
{
	struct Type_Node *base_type;
	struct Declarator *declarator;

	if(!is_type(tokens,scope))
	{
		/*this is a function definition with default base_type of int*/
		return (struct AST*)parse_function_definition(tokens,scope);
	}
	base_type=parse_declaration_specifiers(tokens,scope);
	declarator=get_declarator(base_type);
	parse_declarator(tokens,declarator,scope);
	if(get_and_check(tokens,KW_COMMA))
	{
		/*this is a declaration*/
		struct AST_Declaration *ret;
		ret=get_declaration_tree(scope);
		ret->base_type=base_type;
		Queue_Push(&ret->declarators,declarator);
		Scope_Push(scope,declarator);

		do
		{	
			parse_declarator(tokens,declarator,scope);
			Queue_Push(&ret->declarators,declarator);
			Scope_Push(scope,declarator);
		}while(get_and_check(tokens,KW_COMMA));

		if(get_and_check(tokens,KW_SEMI_COLUMN))
		{
			return (struct AST*)ret;
		}else
		{
			return (struct AST*)get_error_tree((struct AST*)ret);
		}

	}else if(get_and_check(tokens,KW_SEMI_COLUMN))
	{
		struct AST_Declaration *ret;
		ret=get_declaration_tree(scope);
		ret->base_type=base_type;
		Queue_Push(&ret->declarators,declarator);
		Scope_Push(scope,declarator);
		return (struct AST*)ret;

	}else
	{
		/*this is a function call*/
		struct AST_Function_Definition *ret;
		ret=get_function_definition_tree(scope);
		ret->base_type=base_type;
		ret->declarator=declarator;
		if(get_and_check(tokens,KW_OPEN_CURLY))
		{
			ret->body_statement=parse_finish_compound_statement(tokens,scope);
			ret->scope=scope;
			return (struct AST*)ret;
		}else
		{
			return (struct AST*)get_error_tree((struct AST*)ret);
		}

	}


}

/*
	translation-unit:
		declaration [ translation-unit ]
		function-definition [ translation-unit ]
*/
struct AST* parse_translation_unit(struct Queue *tokens,struct Scope *scope)
{
	struct AST_Translation_Unit *hold;
	struct AST *temp;
	hold=get_translation_unit_tree(scope);
	while(tokens->size>0)
	{
		temp=parse_translation_unit_step(tokens,hold->scope);
		Queue_Push(&hold->components,temp);
		if(temp->type==ERROR)
		{
			return (struct AST*)get_error_tree((struct AST*)hold);
		}
		
	}
	return (struct AST*)hold;

}

#endif
