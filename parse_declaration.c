#ifndef GCC_PARSE_DECLARATION_CCOMMA
#define GCC_PARSE_DECLARATION_C GCC_PARSE_DECLARATION_C
#include"parse_declaration.h"

/*declaration-specifiers init-declarator (,init-declarator)* ;*/
/* init-declarator: declarator [ = initializer ] */
void parse_declaration(struct Queue *tokens,struct Scope *scope,struct Queue *where_to_push,char parse_function_definitions)
{
	struct Type *hold_type;
	struct Denotation_Prototype *prototype;
	struct Denoted *hold;
	prototype=(struct Denotation_Prototype*)parse_declaration_specifiers(tokens,scope);

	if(((struct Denoted*)prototype)->denotation==DT_Error)
	{
		/*TODO error*/
			Queue_Push(where_to_push,get_error_tree(get_declaration_error_tree(prototype)));
		return ;
	}

	if(prototype->specifiers==TS_ENUM)
	{
		prototype->type=get_enum_type(prototype);
	}else if(prototype->specifiers==TS_STRUCT || prototype->specifiers==TS_UNION)
	{
		prototype->type=get_struct_union_type(prototype);
	}else 
	{
		prototype->type=get_basic_type(prototype);
	}
	

	do{
		hold=parse_declarator(tokens,scope,prototype);
		if(hold->denotation!=DT_Error)
		{
			if(hold->denotation==DT_Typedef)
			{
				Queue_Push(where_to_push,(struct AST*)get_type_definition_tree((struct Denoted_Typedef*)hold,scope));
			}else if(hold->denotation==DT_Object)
			{
				Queue_Push(where_to_push,(struct AST*),get_object_declaration_tree((struct Denoted_Object*)hold,parse_initializer(tokens,scope,(struct Denoted_Object*)hold),scope));
			}else if(hold->denotation==DT_Function)
			{
				if(parse_function_definitions==1 && get_and_check(tokens,KW_OPEN_CURLY))
				{
					((struct Denoted_Function*)hold)->body=parse_finish_compound_statement(tokens,scope);
					
					Queue_Push(where_to_push,(struct AST*)get_function_definition_tree(scope,(struct Denoted_Function*)hold));
					return ;
				}
				Queue_Push(where_to_push,(struct AST*)get_function_declaration(scope,(struct Denoted_Function*)hold));
			}else
			{
				/*todo error*/
				/*shouldnt be able to reach here*/
				Queue_Push(where_to_push,(struct AST*)get_error_tree((struct AST*)get_declaration_error_tree(hold)));
				return ;
			}

			Scope_Push(scope,hold);
		}else
		{
			/*todo error*/
			Queue_Push(where_to_push,get_error_tree(get_denoted_error(hold)));
			return ;
		}

		parse_function_definitions=0;

	}while(get_and_check(tokens,KW_COMMA));
	if(!get_and_check(tokens,KW_SEMI_COLUMN))
	{
		/*TODO error*/
		Queue_Push(where_to_push,get_error_tree(NULL));
	}

}



/*declaration-specifiers:
 		( storage-class-specifier type-specifier type-qualifier function-specifier)* */
struct Denotation* parse_declaration_specifiers(struct Queue *tokens,struct Scope *scope)
{
	enum KEYWORDS hold_kw;
	struct Denotation_Prototype *ret;
	ret=get_denotation_prototype();

	while(1)
	{
		hold_kw=kw_get(tokens);
		switch(hold_kw)
		{
			case KW_CONST:
				chomp(tokens);
				ret->is_const=1;
				break;
			case KW_VOLATILE:
				chomp(tokens);
				ret->is_volatile=1;
				break;
			case KW_INT:
				chomp(tokens);
				if(ret->specifier!=TS_NONE)
				{
					return get_denotation_prototype((struct Denoted*)ret);
				}
				ret->specifier=TS_INT;
				break;
			case KW_VOID:
				chomp(tokens);
				if(ret->specifier!=TS_NONE)
				{
					return get_denotation_prototype((struct Denoted*)ret);
				}
				ret->specifier=TS_VOID;
				break;
			case KW_CHAR:
				chomp(tokens);
				if(ret->specifier!=TS_NONE)
				{
					return get_denotation_prototype((struct Denoted*)ret);
				}
				ret->specifier=TS_CHAR;
				break;
			case KW_DOUBLE:
				chomp(tokens);
				if(ret->specifier!=TS_NONE)
				{
					return get_denotation_prototype((struct Denoted*)ret);
				}
				ret->specifier=TS_DOUBLE;
				break;
			case KW_FLOAT:
				chomp(tokens);
				if(ret->specifier!=TS_NONE)
				{
					return get_denotation_prototype((struct Denoted*)ret);
				}
				ret->specifier=TS_FLOAT;
				break;
			case KW_LONG:
				chomp(tokens);
				if(ret->constraint!=TC_NONE)
				{
					return get_denotation_prototype((struct Denoted*)ret);
				}
				ret->constraint=TC_LONG;
				break;
			case KW_SHORT:
				chomp(tokens);
				if(ret->constraint!=TC_NONE)
				{
					return get_denotation_prototype((struct Denoted*)ret);
				}
				ret->constraint=TC_SHORT;
				break;
			case KW_EXTERN:
				chomp(tokens);
				if(ret->storage_class!=SC_NONE)
				{
					return get_denotation_prototype((struct Denoted*)ret);
				}
				ret->storage_class=SC_EXTERN;
				break;
			case KW_STATIC:
				chomp(tokens);
				if(ret->storage_class!=SC_NONE)
				{
					return get_denotation_prototype((struct Denoted*)ret);
				}
				ret->storage_class=SC_STATIC;
				break;
			case KW_TYPEDEF:
				chomp(tokens);
				if(ret->storage_class!=SC_NONE)
				{
					return get_denotation_prototype((struct Denoted*)ret);
				}
				ret->storage_class=SC_TYPEDEF;
				break;
			case KW_STRUCT:
				ret->specifier=TS_STRUCT;
				goto hack;
			case KW_UNION:
				ret->specifier=TS_UNION;
				hack:
				chomp(tokens);
				if(check(tokens,KW_ID))
				{
					struct token *id;
					struct Denoted_Struct_Union *tag;
					id=Queue_Pop(tokens);
					tag=check_tag(scope,id);

					if(tag==NULL)
					{
						struct Struct_Union *body;
						body=parse_struct_union_specifier_finish(tokens,scope);
						tag=get_denoted_struct_union(id,body);
						Scope_Push(scope,(struct Denoted*)tag);
						ret->struct_union=body;
					}else
					{
						ret->struct_union=tag->struct_union;
						if(ret->struct_union->specifier!=ret->specifier)
						{
							return get_denotation_prototype((struct Denoted*)ret);
						}
					}

				}else
				{
						ret->struct_union=parse_struct_union_specifier_finish(tokens,scope);
				}
				break;
			case KW_ENUM:
				chomp(tokens);
				ret->specifier=TS_ENUM;
				if(check(tokens,KW_ID))
				{
					struct token *id;
					struct Denoted_Enum *enumerator;
					id=Queue_Pop(tokens);
					enumerator=check_tag(scope,id);
					if(enumerator==NULL)
					{
						struct Enum body;
						body=parse_enum_specifier_finish(tokens,scope);
						enumerator=get_denoted_enum(id,body);
						Scope_Push(scope,(struct Denoted*)enumerator);
						ret->enumerator=body;
					}else
					{
						ret->enumerator=enumerator->enumeration;
						if(enumeration->enumeration->specifier!=TS_ENUM)
						{
							return get_denotation_prototype((struct Denoted*)ret);
						}
					}

				}else
				{
					ret->enumeration=parse_enum_specifier_finish(tokens,scope);
				}
				break;
			default:
				return ret;
		}
	}
}





/*
   	declarator:
		( pointer ( type-qualifier )* )* direct-declarator
 */
struct Denoted* parse_declarator(struct Queue *tokens,struct Scope *scope,struct Denotation_Prototype *prototype)
{
	enum KEYWORDS hold;
	struct Denotation_Prototype copy;
	
	copy=*prototype;
	parse_declarator_inner(tokens,scope,&copy);
	if(copy.type->specifier==TS_FUNC)
	{
		return get_denoted_function(prototype);
	}else if(copy.type->specifier==TS_TYPEDEF

	return ;
}

void parse_declarator_inner(struct Queue *tokens,struct Scope *scope,struct Denotation_Prototype *copy)
{
	
	while(get_and_check(tokens,KW_STAR))
	{
		base=get_pointer_type(base);
		hold=kw_get(tokens);
		while(1)
		{
			if(hold==KW_CONST)
			{
				base->is_const=1;
			}else if(hold==KW_VOLATILE)
			{
				base->is_volatile=1;
			}else
			{
				break;
			}
		}
	}

}
/*
	direct-declarator:
		id direct-declarator-finish
		( declarator ) direct-declarator-finish
*/
void parse_direct_declarator(struct Queue *tokens,struct Scope *scope,struct Denotation_Prototype *prototype)
{
	struct Denoted *hold;
	if(check(tokens,KW_ID))
	{
		struct token *id;
		id=Queue_Pop(tokens);
		base=parse_direct_declarator_finish(tokens,scope,base);
		switch(base->specifier)
		{
			case VOID:
			case :
			case VOID:
			case VOID:
			case VOID:
		};
		
	}else if(get_and_check(tokens,KW_OPEN_NORMAL))
	{
	}else
	{
		/*TODO error*/
	}
}

/*
	direct-declarator-finish:
		( [ constant-expression ] | (parameter-list) | ( [id-list] ) )* 
*/
void parse_direct_declarator_finish(struct Queue *tokens,struct Scope *scope,struct Denotation_Prototype *prototype)
{

}


/*
	struct-union-specifier-finish:
		{ ( struct-declaration )* }
 */
struct Struct_Union* parse_struct_union_specifier_finish(struct Queue *tokens,struct Scope *scope)
{

}
/*
   struct-declaration:
		specifier-qualifier-list  ( struct-declarator )*  ;
   */
void parse_struct_declaration(struct Queue *tokens,struct Scope *struct_scope,struct Queue* members)
{

}
/*
	struct-declarator:
		declarator
		[ declarator ] : constant-expression
 */
struct Denoted* parse_struct_declarator(struct Queue *tokens,struct Scope *struct_scope,struct Denoted *origin)
{

}
/*
	enum-specifier-finish
	{ ( enumeration-constant [ = constant-expression ] )* }
*/
struct Enum* parse_enum_specifier_finish(struct Queue *tokens,struct Scope *scope)
{

}

/*
	parameter-list:
		(declaratoion-specifiers (declarator | abstract-declarator),)+
*/
void parse_paramenter_list(struct Queue *tokens,struct Scope *scope,struct Queue *parameters)
{

}
/*
   id-list:
   	id,...
  */
void parse_id_list(struct Queue *tokens,struct Scope *scope,struct Queue *ids)
{

}

/*
	type-name:
		specifier-qualifier-list [abstract-declarator]
*/
struct Type* parse_type_name(struct Queue *tokens,struct Scope *scope)
{

}
/*
   	abstract-declarator:
		( pointer )* abstract-direct-declarator
*/
struct Type* parse_abstract_declarator(struct Queue *tokens,struct Scope *scope,struct Type *base)
{

}

/*
	abstract-direct-declarator:
		( abstract-declarator )
		abstract-declarator-finish
*/
struct Type* parse_abstract_direct_declarator(struct Queue *tokens,struct Scope *scope,struct Type *base)
{

}

/*
	abstract-declarator-finish:
		( [ constant-expression] | ( parameter-type-list) )*
*/
struct Type* parse_abstract_declarator_finish(struct Queue *tokens,struct Scope *scope,struct Type *base)
{

}
/*
	initializer:
		assignment-expression
		{ initializer , ... [,] }
*/
struct AST* parse_initializer(struct Queue *tokens,struct Scope *scope,struct Denoted_Object *base)
{

}
#endif
