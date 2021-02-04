#ifndef GCC_PARSE_DECLARATION_C
#define GCC_PARSE_DECLARATION_C GCC_PARSE_DECLARATION_C
#include"parse_declaration.h"

/*declaration-specifiers init-declarator (,init-declarator)* ;*/
/* init-declarator: declarator [ = initializer ] */
void parse_declaration(struct Queue *tokens,struct Scope *scope,struct Queue *where_to_push,char parse_function_definitions)
{
	struct Denotation_Prototype *prototype;
	struct Denoted *hold;

	prototype=parse_declaration_specifiers(tokens,scope);
	while(!get_and_check(tokens,KW_SEMI_COLUMN))
	{
		hold=parse_declarator(tokens,scope,prototype);
		if(hold->denotation==DT_Function && parse_function_definitions==1)
		{
			if(get_and_check(tokens,KW_OPEN_CURLY))
			{
				((struct Denoted_Function*)hold)->body=(struct AST_Compound_Statement*)parse_finish_compound_statement(tokens,scope);
				Queue_Push(where_to_push,get_function_declaration_tree(scope,(struct Denoted_Function*)hold));
				Scope_Push(scope,hold);
				free(prototype);
				return;
			}

			Queue_Push(where_to_push,get_function_declaration_tree(scope,(struct Denoted_Function*)hold));
		}else if(hold->denotation==DT_Typedef)
		{
			Queue_Push(where_to_push,get_type_definition_tree((struct Denoted_Typedef*)hold,scope));
		}else if(hold->denotation==DT_Object)
		{
			Queue_Push(where_to_push,get_object_declaration_tree((struct Denoted_Object*)hold,NULL,scope));
		}else
		{
			/*TODO error*/
			Queue_Push(where_to_push,get_declaration_error_tree(hold));
			free(prototype);
			return;
		}

		Scope_Push(scope,hold);
		parse_function_definitions=0;
	}
	free(prototype);

}


struct Denotation_Prototype* parse_specifier_qualifier_list(struct Queue *tokens,struct Scope *scope)
{
	return parse_declaration_specifiers_inner(tokens,scope,0);
}
struct Denotation_Prototype* parse_declaration_specifiers(struct Queue *tokens,struct Scope *scope)
{
	return parse_declaration_specifiers_inner(tokens,scope,1);
}

/*declaration-specifiers:
 		( storage-class-specifier type-specifier type-qualifier function-specifier)* */
struct Denotation_Prototype* parse_declaration_specifiers_inner(struct Queue *tokens,struct Scope *scope,char parse_storage_class)
{
	enum KEYWORDS hold_kw;
	struct Denotation_Prototype *ret;
	ret=(struct Denotation_Prototype*)get_denotation_prototype();

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
					return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
				}
				ret->specifier=TS_INT;
				break;
			case KW_VOID:
				chomp(tokens);
				if(ret->specifier!=TS_NONE)
				{
					return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
				}
				ret->specifier=TS_VOID;
				break;
			case KW_CHAR:
				chomp(tokens);
				if(ret->specifier!=TS_NONE)
				{
					return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
				}
				ret->specifier=TS_CHAR;
				break;
			case KW_DOUBLE:
				chomp(tokens);
				if(ret->specifier!=TS_NONE)
				{
					return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
				}
				ret->specifier=TS_DOUBLE;
				break;
			case KW_FLOAT:
				chomp(tokens);
				if(ret->specifier!=TS_NONE)
				{
					return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
				}
				ret->specifier=TS_FLOAT;
				break;
			case KW_LONG:
				chomp(tokens);
				if(ret->constraint!=TC_NONE)
				{
					return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
				}
				ret->constraint=TC_LONG;
				break;
			case KW_SHORT:
				chomp(tokens);
				if(ret->constraint!=TC_NONE)
				{
					return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
				}
				ret->constraint=TC_SHORT;
				break;
			case KW_EXTERN:
				if(!parse_storage_class)
					goto exit;
				chomp(tokens);
				if(ret->storage_class!=SC_NONE)
				{
					return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
				}
				ret->storage_class=SC_EXTERN;
				break;
			case KW_STATIC:
				if(!parse_storage_class)
					goto exit;
				chomp(tokens);
				if(ret->storage_class!=SC_NONE)
				{
					return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
				}
				ret->storage_class=SC_STATIC;
				break;
			case KW_TYPEDEF:
				if(!parse_storage_class)
					goto exit;
				chomp(tokens);
				if(ret->storage_class!=SC_NONE)
				{
					return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
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
				if(check(tokens,KW_ID,0))
				{
					struct token *id;
					struct Denoted_Struct_Union *tag;
					id=Queue_Pop(tokens);
					tag=(struct Denoted_Struct_Union*)check_tag(scope,id);

					if(tag==NULL)
					{
						struct Struct_Union *body;
						body=get_struct_union_base(ret->specifier);
						parse_struct_union_specifier_finish(tokens,scope,body);
						Scope_Push(scope,get_denoted_struct_union(id,body));
						ret->struct_union=body;
					}else
					{
						ret->struct_union=tag->struct_union;
						if(ret->struct_union->specifier!=ret->specifier)
						{
							return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
						}
						if(ret->struct_union->members->size==0)
						{
							/*then this could be a definition*/
							parse_struct_declaration(tokens,scope,ret->struct_union->members);
						}
					}

				}else
				{
						ret->struct_union=get_struct_union_base(ret->specifier);
						parse_struct_union_specifier_finish(tokens,scope,ret->struct_union);
				}
				break;
			case KW_ENUM:
				chomp(tokens);
				ret->specifier=TS_ENUM;
				if(check(tokens,KW_ID,0))
				{
					struct token *id;
					struct Denoted_Enum *enumerator;
					id=Queue_Pop(tokens);
					enumerator=(struct Denoted_Enum*)check_tag(scope,id);
					if(enumerator==NULL)
					{
						struct Enum *body;
						body=get_enum_base();
						parse_enum_specifier_finish(tokens,scope,body);
						Scope_Push(scope,get_denoted_enum(id,body));
						ret->enumerator=body;
					}else
					{
						ret->enumerator=enumerator->enumeration;
						if(enumerator->denotation!=DT_Enum)
						{
							return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
						}
						if(ret->enumerator->consts->size==0)
						{
							/*this could be an enum definition*/
							parse_enum_specifier_finish(tokens,scope,ret->enumerator);
						}
					}

				}else
				{
					parse_enum_specifier_finish(tokens,scope,ret->enumerator);
				}
				break;
			case KW_ID:
				if(ret->specifier==TS_NONE)
				{
					struct Denoted *hold;
					hold=check_ordinary(scope,(struct token*)tokens->first->data);
					if(hold!=NULL && hold->denotation==DT_Typedef)
					{
						ret->type=((struct Denoted_Typedef*)hold)->type;
						chomp(tokens);
						break;
					}
					/*falltrough - this has not been typedefed*/
				}
				/*falltrough (it is possible to overwrite typedef id from upper scope)*/
			default:
			exit:
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
struct Denoted* parse_declarator(struct Queue *tokens,struct Scope *scope,struct Denotation_Prototype *prototype)
{
	struct Denoted_Base temp;
	temp.id=NULL;
	temp.denotation=DT_Prototype;
	temp.type=prototype->type;
	parse_declarator_inner(tokens,scope,&temp);
	return extract_denoted(&temp,prototype,0);

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
				((struct Type_Pointer*)(base->type))->is_const=1;
			}else if(hold==KW_VOLATILE)
			{
				((struct Type_Pointer*)(base->type))->is_volatile=1;
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
	if(check(tokens,KW_ID,0))
	{
		base->id=Queue_Pop(tokens);
		parse_direct_declarator_finish(tokens,scope,base);
		
	}else if(get_and_check(tokens,KW_OPEN_NORMAL))
	{
		struct Queue *hack;
		hack=malloc(sizeof(struct Queue));
		Queue_Init(hack);
		while(!check(tokens,KW_CLOSE_NORMAL,0))
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
			base->denotation=DT_Error;
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
		/*this might be an abstract declarator*/
		parse_direct_declarator_finish(tokens,scope,base);
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
				base->denotation=DT_Error;
				return;
			}
		}else if(get_and_check(tokens,KW_OPEN_NORMAL))
		{
			struct Queue *parameters;
			struct Scope *function_prototype_scope;

			function_prototype_scope=get_scope(scope);

			parameters=malloc(sizeof(struct Queue));
			Queue_Init(parameters);

			parse_paramenter_list(tokens,function_prototype_scope,parameters);
			base->type=get_function_type(base->type,parameters,function_prototype_scope);
			
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
void parse_struct_union_specifier_finish(struct Queue *tokens,struct Scope *scope,struct Struct_Union *base)
{
	if(get_and_check(tokens,KW_OPEN_CURLY))
	{
		while(parse_struct_declaration(tokens,base->inner_namespace,base->members))
		{
			
			if(get_and_check(tokens,KW_CLOSE_CURLY))
			{
				return ;
			}
		}

		/*TODO error*/
		return ;

		
	}else
	{
		/*if this isnt a struct definition return an incomplete struct-union*/
		return ;
		
	}

}
/*
   struct-declaration:
		specifier-qualifier-list  ( struct-declarator )*  ;
   */
char parse_struct_declaration(struct Queue *tokens,struct Scope *struct_scope,struct Queue* members)
{
	struct Denotation_Prototype *prototype;
	struct Denoted *hold;
	prototype=parse_specifier_qualifier_list(tokens,struct_scope);
	while(!get_and_check(tokens,KW_SEMI_COLUMN))
	{
		hold=parse_struct_declarator(tokens,struct_scope,prototype);
		if(hold!=NULL && hold->denotation!=DT_Error)
		{
			Scope_Push(struct_scope,hold);
			Queue_Push(members,hold);

		}else
		{
			free(prototype);
			/*todo error*/
			return 0;
		}
	}
	free(prototype);
	return 1;

}
/*
	struct-declarator:
		declarator
		[ declarator ] : constant-expression
 */
struct Denoted* parse_struct_declarator(struct Queue *tokens,struct Scope *scope,struct Denotation_Prototype *prototype)
{
	struct Denoted *hold;
	if(get_and_check(tokens,KW_COLUMN))
	{
		/*unnamed bitfields are possible*/
		struct Denoted_Object *obj;
		obj=(struct Denoted_Object*)get_denoted_object(NULL,SC_NONE,prototype->type);
		obj->object->type=get_type_bitfield(prototype->type,parse_expression(tokens,scope));
		return (struct Denoted*)obj;

	}else
	{
		hold=parse_declarator(tokens,scope,prototype);
		if(get_and_check(tokens,KW_COLUMN))
		{
			if(hold->denotation==DT_Object)
			{
				((struct Denoted_Object*)hold)->object->type=get_type_bitfield(((struct Denoted_Object*)hold)->object->type,parse_expression(tokens,scope));
				return hold;
			}else
			{
				/*TODO error*/
				return get_denoted_error(hold);
			}
		}
	}
	
}
/*
	enum-specifier-finish
	{ ( enumeration-constant [ = constant-expression ] , )* }
*/
void parse_enum_specifier_finish(struct Queue *tokens,struct Scope *scope,struct Enum *enumeration)
{
	struct token *id;
	struct Denoted_Enum_Const *hold;
	int where_in_enumeration=0;
	if(get_and_check(tokens,KW_OPEN_CURLY))
	{
		do
		{
			if(check(tokens,KW_ID,0))
			{
				id=Queue_Pop(tokens);
				if(get_and_check(tokens,KW_EQ))
				{
					hold=(struct Denoted_Enum_Const*)get_denoted_enum_const_expr(id,enumeration,parse_expression(tokens,scope));
					Queue_Push(enumeration->consts,hold);
					where_in_enumeration=hold->value+1;
				}else
				{
					Queue_Push(enumeration->consts,get_denoted_enum_const_num(id,enumeration,where_in_enumeration));
					++where_in_enumeration;
				}
				if(!get_and_check(tokens,KW_COMMA) && get_and_check(tokens,KW_CLOSE_CURLY))
				{
					return;
				}else
				{
					/*TODO error*/
					Queue_Push(enumeration->consts,get_denoted_error(NULL));
					return ;
				}
			}else
			{
				/*TODO error*/
				Queue_Push(enumeration->consts,get_denoted_error(NULL));
				return ;
			}
		}while(!get_and_check(tokens,KW_CLOSE_CURLY));

	}
}

/*
	parameter-list:
		(declaratoion-specifiers (declarator | abstract-declarator),)+
*/
void parse_paramenter_list(struct Queue *tokens,struct Scope *function_prototype_scope,struct Queue *parameters)
{
	struct Denotation_Prototype *prototype;
	struct Denoted_Base temp;
	struct Denoted *hold;
	temp.denotation=DT_Prototype;
	do
	{
		prototype=parse_declaration_specifiers(tokens,function_prototype_scope);

		temp.id=NULL;
		temp.type=prototype->type;

		parse_declarator_inner(tokens,function_prototype_scope,&temp);

		hold=extract_denoted(&temp,prototype,1);

		Scope_Push(function_prototype_scope,hold);
		Queue_Push(parameters,hold);

		free(prototype);
	}while(get_and_check(tokens,KW_COMMA));

}
/*
	type-name:
		specifier-qualifier-list [abstract-declarator]
*/
struct Type* parse_type_name(struct Queue *tokens,struct Scope *scope)
{
	struct Denotation_Prototype *prototype;
	struct Type *ret;
	prototype=parse_specifier_qualifier_list(tokens,scope);
	ret=parse_abstract_declarator(tokens,scope,prototype->type);
	free(prototype);
	return ret;
}
/*
   	abstract-declarator:
		( pointer )* abstract-direct-declarator
*/
struct Type* parse_abstract_declarator(struct Queue *tokens,struct Scope *scope,struct Type *base)
{
	struct Denoted_Base hold;
	hold.denotation=DT_Prototype;
	hold.id=NULL;
	hold.type=base;
	parse_declarator_inner(tokens,scope,&hold);

	if(hold.denotation==DT_Error || hold.id!=NULL)
	{
		/*TODO error*/
		return get_type_error(hold.type);
	}
	return hold.type;
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
