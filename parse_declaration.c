#ifndef GCC_PARSE_DECLARATION_CCOMMA
#define GCC_PARSE_DECLARATION_C GCC_PARSE_DECLARATION_C
#include"parse_declaration.h"

/*declaration-specifiers init-declarator (,init-declarator)* ;*/
/* init-declarator: declarator [ = initializer ] */
void parse_declaration(struct Queue *tokens,struct Scope *scope,struct Queue *where_to_push,char parse_function_definitions,struct Location *where)
{
	struct Denotation_Prototype *prototype;
	struct Denoted *hold;

	prototype=parse_declaration_specifiers(tokens,scope);
	while(!get_and_check(tokens,KW_SEMI_COLUMN))
	{
		hold=parse_declarator(tokens,scope,prototype,where);
		if(hold->denotation==DT_Function && parse_function_definitions==1)
		{
			if(get_and_check(tokens,KW_OPEN_CURLY))
			{
				((struct Denoted_Function*)hold)->body=parse_finish_compound_statement(tokens,scope);
				Queue_Push(where_to_push,get_function_declaration_tree(scope,(struct Denoted_Function*)hold));
				return;
			}
			Queue_Push(where_to_push,get_function_declaration_tree(scope,(struct Denoted_Function*)hold));
		}else if(hold->denotation==DT_Typedef)
		{
			Queue_Push(where_to_push,get_type_definition_tree((struct Denoted_Typedef*)hold,scope);
		}else if(hold->denotation==DT_Object)
		{
			Queue_Push(where_to_push,get_object_declaration_tree((struct Denoted_Object*)hold,NULL,scope));
		}else
		{
			/*TODO error*/
			Queue_Push(where_to_push,get_declaration_error_tree(hold));
			return;
		}
		parse_function_definitions=0;
	}

}


/*hack*/
struct Denotation_Prototype* parse_specifier_qualifier_list(struct Queue *tokens,struct Scope *scope)
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
					return get_denotation_error((struct Denoted*)ret);
				}
				ret->specifier=TS_INT;
				break;
			case KW_VOID:
				chomp(tokens);
				if(ret->specifier!=TS_NONE)
				{
					return get_denotation_error((struct Denoted*)ret);
				}
				ret->specifier=TS_VOID;
				break;
			case KW_CHAR:
				chomp(tokens);
				if(ret->specifier!=TS_NONE)
				{
					return get_denotation_error((struct Denoted*)ret);
				}
				ret->specifier=TS_CHAR;
				break;
			case KW_DOUBLE:
				chomp(tokens);
				if(ret->specifier!=TS_NONE)
				{
					return get_denotation_error((struct Denoted*)ret);
				}
				ret->specifier=TS_DOUBLE;
				break;
			case KW_FLOAT:
				chomp(tokens);
				if(ret->specifier!=TS_NONE)
				{
					return get_denotation_error((struct Denoted*)ret);
				}
				ret->specifier=TS_FLOAT;
				break;
			case KW_LONG:
				chomp(tokens);
				if(ret->constraint!=TC_NONE)
				{
					return get_denotation_error((struct Denoted*)ret);
				}
				ret->constraint=TC_LONG;
				break;
			case KW_SHORT:
				chomp(tokens);
				if(ret->constraint!=TC_NONE)
				{
					return get_denotation_error((struct Denoted*)ret);
				}
				ret->constraint=TC_SHORT;
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
							return get_denotation_error((struct Denoted*)ret);
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
							return get_denotation_error((struct Denoted*)ret);
						}
					}

				}else
				{
					ret->enumeration=parse_enum_specifier_finish(tokens,scope);
				}
				break;
			case KW_ID:
				if(ret->specifier==TS_NONE)
				{
					struct Denoted *hold;
					hold=check_ordinary(scope,(struct token*)tokens->first.data);
					if(hold!=NULL && hold->denotation==DT_Typedef)
					{
						chomp();
						ret->type=((struct Denoted_Typedef*)hold)->type;
						break;
					}
					/*falltrough - this has not been typedefed*/
				}
				/*falltrough (it is possible to overwrite typedef id from upper scope)*/
			default:
				if(ret->specifier==TS_ENUM)
				{
					ret->type=get_enum_type(ret);
				}else if(ret->specifier==TS_STRUCT || ret->specifier==TS_UNION)
				{
					ret->type=get_struct_union_type(ret);
				}else if(ret->type==NULL)
				{
					ret->type=get_basic_type(ret);
				}
				return ret;
		}
	}
}
}

/*declaration-specifiers:
 		( storage-class-specifier type-specifier type-qualifier function-specifier)* */
struct Denotation_Prototype* parse_declaration_specifiers(struct Queue *tokens,struct Scope *scope)
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
					return get_denotation_error((struct Denoted*)ret);
				}
				ret->specifier=TS_INT;
				break;
			case KW_VOID:
				chomp(tokens);
				if(ret->specifier!=TS_NONE)
				{
					return get_denotation_error((struct Denoted*)ret);
				}
				ret->specifier=TS_VOID;
				break;
			case KW_CHAR:
				chomp(tokens);
				if(ret->specifier!=TS_NONE)
				{
					return get_denotation_error((struct Denoted*)ret);
				}
				ret->specifier=TS_CHAR;
				break;
			case KW_DOUBLE:
				chomp(tokens);
				if(ret->specifier!=TS_NONE)
				{
					return get_denotation_error((struct Denoted*)ret);
				}
				ret->specifier=TS_DOUBLE;
				break;
			case KW_FLOAT:
				chomp(tokens);
				if(ret->specifier!=TS_NONE)
				{
					return get_denotation_error((struct Denoted*)ret);
				}
				ret->specifier=TS_FLOAT;
				break;
			case KW_LONG:
				chomp(tokens);
				if(ret->constraint!=TC_NONE)
				{
					return get_denotation_error((struct Denoted*)ret);
				}
				ret->constraint=TC_LONG;
				break;
			case KW_SHORT:
				chomp(tokens);
				if(ret->constraint!=TC_NONE)
				{
					return get_denotation_error((struct Denoted*)ret);
				}
				ret->constraint=TC_SHORT;
				break;
			case KW_EXTERN:
				chomp(tokens);
				if(ret->storage_class!=SC_NONE)
				{
					return get_denotation_error((struct Denoted*)ret);
				}
				ret->storage_class=SC_EXTERN;
				break;
			case KW_STATIC:
				chomp(tokens);
				if(ret->storage_class!=SC_NONE)
				{
					return get_denotation_error((struct Denoted*)ret);
				}
				ret->storage_class=SC_STATIC;
				break;
			case KW_TYPEDEF:
				chomp(tokens);
				if(ret->storage_class!=SC_NONE)
				{
					return get_denotation_error((struct Denoted*)ret);
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
							return get_denotation_error((struct Denoted*)ret);
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
							return get_denotation_error((struct Denoted*)ret);
						}
					}

				}else
				{
					ret->enumeration=parse_enum_specifier_finish(tokens,scope);
				}
				break;
			case KW_ID:
				if(ret->specifier==TS_NONE)
				{
					struct Denoted *hold;
					hold=check_ordinary(scope,(struct token*)tokens->first.data);
					if(hold!=NULL && hold->denotation==DT_Typedef)
					{
						ret->type=((struct Denoted_Typedef*)hold)->type;
						chomp();
						break;
					}
					/*falltrough - this has not been typedefed*/
				}
				/*falltrough (it is possible to overwrite typedef id from upper scope)*/
			default:
				if(ret->specifier==TS_ENUM)
				{
					ret->type=get_enum_type(ret);
				}else if(ret->specifier==TS_STRUCT || ret->specifier==TS_UNION)
				{
					ret->type=get_struct_union_type(ret);
				}else if(ret->type==NULL)
				{
					ret->type=get_basic_type(ret);
				}
				return ret;
		}
	}
}





/*
   	declarator:
		( pointer ( type-qualifier )* )* direct-declarator
 */
struct Denoted* parse_declarator(struct Queue *tokens,struct Scope *scope,struct Denotation_Prototype *prototype,struct Location *where)
{
	struct Denoted_Base temp;
	temp.id=NULL;
	temp.denotation=DT_Prototype;
	temp.type=prototype->type;
	parse_declarator_inner(tokens,scope,&temp);
	if(temp.type->specifier==TS_FUNC)
	{
		if(temp.id==NULL)
		{
			return get_denotation_error(get_denoted_function(NULL,((struct Type_Function*)temp.type)->return_type,prototype->function_specifier));
		}else
		{
			return get_denotation_error(get_denoted_function(temp.id,((struct Type_Function*)temp.type)->return_type,prototype->function_specifier));
		}
	}else if(temp.type->specifier==TS_TYPEDEF)
	{
		if(temp.id==NULL)
		{
			return get_denoted_error(get_denoted_typedef(&temp));
		}else
		{
			return get_denoted_typedef(&temp);
		}
	}else
	{
		if(temp.id==NULL)
		{
			return get_denoted_error(get_denoted_object(temp.id,prototype->storage_class,where,temp.type));
		}else
		{
			return get_denoted_object(temp.id,prototype->storage_class,where,temp.type);
		}
	}

}

void parse_declarator_inner(struct Queue *tokens,struct Scope *scope,struct Denoted_Base *base)
{
	enum KEYWORDS hold;
	while(get_and_check(tokens,KW_STAR))
	{
		base->type=get_pointer_type(base->type);
		hold=kw_get(tokens);
		while(1)
		{
			if(hold==KW_CONST)
			{
				(struct Type_Pointer*)(base->type)->is_const=1;
			}else if(hold==KW_VOLATILE)
			{
				(struct Type_Pointer*)(base->type)->is_volatile=1;
			}else
			{
				break;
			}
		}
	}
	parse_direct_declarator(tokens,scope,base);

}
/*
	direct-declarator:
		id direct-declarator-finish
		( declarator ) direct-declarator-finish
*/
void parse_direct_declarator(struct Queue *tokens,struct Scope *scope,struct Denoted_Base *base)
{
	if(check(tokens,KW_ID))
	{
		base->id=Queue_Pop(tokens);
		parse_direct_declarator_finish(tokens,scope,base);
		
	}else if(get_and_check(tokens,KW_OPEN_NORMAL))
	{
		struct Queue *hack;
		hack=malloc(sizeof(struct Queue));
		Queue_Init(hack);
		while(!check(tokens,KW_CLOSE_NORMAL))
		{
			Queue_Push(hack,Queue_Pop(tokens));
		}
		/*remove closing )*/
		chomp(tokens);
		parse_direct_declarator_finish(tokens,scope,base);
		parse_declarator_inner(hack,scope,base);
		if(hack->size!=0)
		{
			/*TODO error*/
			while(hack->size)
			{
				free(Queue_Pop(hack));
			}
			free(hack);
			return;
		}
		free(hack);
		
	}else
	{
		/*TODO error*/
		return;
	}
}

/*
	direct-declarator-finish:
		( [ constant-expression ] | (parameter-list) | ( [id-list] ) )* 
*/
void parse_direct_declarator_finish(struct Queue *tokens,struct Scope *scope,struct Denoted_Base *base)
{
	while(1)
	{
		if(get_and_check(tokens,KW_OPEN_SQUARE))
		{
			base->type=get_array_type(base->type,parse_expression(tokens,scope));
			if(!get_and_check(tokens,KW_CLOSE_NORMAL))
			{
				base->type=get_type_error(base->type);
				return;
			}
		}else if(get_and_check(tokens,KW_OPEN_NORMAL))
		{
			struct Queue *parameters;
			parameters=malloc(sizeof(struct Queue));
			Queue_Init(parameters);
			parse_paramenter_list(tokens,scope,parameters);
			base->type=get_function_type(base->type,parameters);
			
		}else
		{
			break;
		}
		
	}

}


/*
	struct-union-specifier-finish:
		{ ( struct-declaration )* }
 */
struct Struct_Union* parse_struct_union_specifier_finish(struct Queue *tokens,struct Scope *scope)
{
	struct Struct_Union *ret;
	ret=get_struct_union_base();
	if(get_and_check(tokens,KW_OPEN_CURLY))
	{
		while(parse_struct_declaration(tokens,ret->inner_namespace,ret->members));
		if(get_and_check(tokens,KW_CLOSE_CURLY))
		{
			return ret;
		}else
		{
			/*TODO error*/
			return ret;
		}

		
	}else
	{
		/*if this isnt a struct definition return an incomplete struct-union*/
		return ret;
		
	}

}
/*
   struct-declaration:
		specifier-qualifier-list  ( struct-declarator )*  ;
   */
char parse_struct_declaration(struct Queue *tokens,struct Scope *struct_scope,struct Queue* members)
{
	struct Denotation_Prototype *prototype;
	prototype=parse_specifier_qualifier_list(tokens,scope);

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
