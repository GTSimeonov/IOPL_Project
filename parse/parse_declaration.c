#ifndef GCC_PARSE_DECLARATION_C
#define GCC_PARSE_DECLARATION_C GCC_PARSE_DECLARATION_C
#include"parse_declaration.h"

/*declaration-specifiers init-declarator (,init-declarator)* ;*/
/* init-declarator: declarator [ = initializer ] */
void parse_declaration(struct Translation_Data *translation_data,struct Scope *scope,struct Queue *where_to_push,char parse_function_definitions)
{
	struct Denotation_Prototype *prototype;
	struct Denoted *hold;

	prototype=parse_declaration_specifiers(translation_data,scope);
	while(1)
	{
		if(get_and_check(translation_data,KW_SEMI_COLUMN))
			return;

		hold=parse_declarator(translation_data,scope,prototype);



		if(hold->denotation==DT_Function && parse_function_definitions==1)
		{
			if(get_and_check(translation_data,KW_OPEN_CURLY))
			{
				((struct Denoted_Function*)hold)->body=(struct AST_Compound_Statement*)parse_finish_compound_statement(translation_data,scope);
				((struct Denoted_Function*)hold)->storage_class=(prototype->storage_class==SC_NONE?
										SC_EXTERN:
										prototype->storage_class);




				Queue_Push(where_to_push,get_function_definition_tree(scope,(struct Denoted_Function*)hold));
				Scope_Push(scope,hold);
				free(prototype);
				return;
			}

			Queue_Push(where_to_push,get_function_declaration_tree(scope,(struct Denoted_Function*)hold));
		}else if(hold->denotation==DT_Typedef)
		{
			Queue_Push(where_to_push,get_type_definition_tree((struct Denoted_Typedef*)hold));
		}else if(hold->denotation==DT_Object)
		{
			struct AST_Object_Declaration *od;
			od=get_object_declaration_tree((struct Denoted_Object*)hold,NULL);
			Queue_Push(where_to_push,od);
			if(get_and_check(translation_data,KW_EQ))
			{
				od->initializer=parse_initializer(translation_data,scope,od->object);
			}
		}else
		{
			/*TODO error*/
			Queue_Push(where_to_push,get_declaration_error_tree(hold));
			push_translation_error("declaration expected",translation_data);
			free(prototype);
			/*search for end of erronous declaration*/
			while(!get_and_check(translation_data,KW_SEMI_COLUMN))
			{
				free(Queue_Pop(translation_data->tokens));
			}
			return;
		}

		Scope_Push(scope,hold);
		parse_function_definitions=0;
		if(!get_and_check(translation_data,KW_COMMA))
		{
			if(get_and_check(translation_data,KW_SEMI_COLUMN))
			{
				return;
			}else
			{
				/*TODO error*/
				Queue_Push(where_to_push,get_declaration_error_tree(NULL));
				push_translation_error("semi column expected",translation_data);
				return;
			}
		}
	}
	free(prototype);

}


struct Denotation_Prototype* parse_specifier_qualifier_list(struct Translation_Data *translation_data,struct Scope *scope)
{
	return parse_declaration_specifiers_inner(translation_data,scope,0);
}
struct Denotation_Prototype* parse_declaration_specifiers(struct Translation_Data *translation_data,struct Scope *scope)
{
	return parse_declaration_specifiers_inner(translation_data,scope,1);
}

/*declaration-specifiers:
 		( storage-class-specifier type-specifier type-qualifier function-specifier)* */
struct Denotation_Prototype* parse_declaration_specifiers_inner(struct Translation_Data *translation_data,struct Scope *scope,char parse_storage_class)
{
	enum KEYWORDS hold_kw;
	struct Denotation_Prototype *ret;
	ret=(struct Denotation_Prototype*)get_denotation_prototype(translation_data->types);

	while(1)
	{
		hold_kw=kw_get(translation_data);
		switch(hold_kw)
		{
			case KW_CONST:
				chomp(translation_data);
				ret->is_const=1;
				break;
			case KW_VOLATILE:
				chomp(translation_data);
				ret->is_volatile=1;
				break;
			case KW_INT:
				chomp(translation_data);
				if(ret->specifier!=TS_NONE)
				{
					push_translation_error("more than one type specifier given",translation_data);
					return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
				}
				ret->specifier=TS_INT;
				break;
			case KW_VOID:
				chomp(translation_data);
				if(ret->specifier!=TS_NONE)
				{
					push_translation_error("more than one type specifier given",translation_data);
					return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
				}
				ret->specifier=TS_VOID;
				break;
			case KW_CHAR:
				chomp(translation_data);
				if(ret->specifier!=TS_NONE)
				{
					push_translation_error("more than one type specifier given",translation_data);
					return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
				}
				ret->specifier=TS_CHAR;
				break;
			case KW_DOUBLE:
				chomp(translation_data);
				if(ret->specifier!=TS_NONE)
				{
					push_translation_error("more than one type specifier given",translation_data);
					return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
				}
				ret->specifier=TS_DOUBLE;
				break;
			case KW_FLOAT:
				chomp(translation_data);
				if(ret->specifier!=TS_NONE)
				{
					push_translation_error("more than one type specifier given",translation_data);
					return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
				}
				ret->specifier=TS_FLOAT;
				break;
			case KW_LONG:
				chomp(translation_data);
				if(ret->constraint!=TC_NONE)
				{
					if(ret->constraint==TC_LONG)
					{
						ret->constraint=TC_LONG_LONG;
						break;
					}else
					{
						if(ret->constraint==TC_LONG_LONG)
							push_translation_error("yeah ... it's big",translation_data);
						else if(ret->constraint==TC_SHORT)
							push_translation_error("long and short constraints contradict",translation_data);
						else 
							assert(0);
						return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
					}
				}
				ret->constraint=TC_LONG;
				break;
			case KW_SHORT:
				chomp(translation_data);
				if(ret->constraint!=TC_NONE)
				{
					switch(ret->constraint)
					{
						case TC_LONG:
						push_translation_error("long and short constraints contradict",translation_data);
						break;
						case TC_SHORT:
						push_translation_error("it's not about the size, it's about how you use it",translation_data);
						break;
						case TC_LONG_LONG:
						push_translation_error("long long and short constraints contradict",translation_data);
						break;
						default:
						/*should not be able to enter here*/
						assert(0);
					}
					return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
				}
				ret->constraint=TC_SHORT;
				break;
			case KW_EXTERN:
				if(!parse_storage_class)
					goto exit;
				chomp(translation_data);
				if(ret->storage_class!=SC_NONE)
				{
					switch(ret->storage_class)
					{
						case SC_EXTERN:
						push_translation_error("only one extern allowed >:|",translation_data);
						break;

						case SC_TYPEDEF:
						case SC_STATIC:
						push_translation_error("only one storage class allowed >:|",translation_data);
						break;
						default:
						assert(0);
					}

					return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
				}
				ret->storage_class=SC_EXTERN;
				break;
			case KW_STATIC:
				if(!parse_storage_class)
					goto exit;
				chomp(translation_data);
				if(ret->storage_class!=SC_NONE)
				{
					switch(ret->storage_class)
					{
						case SC_STATIC:
						push_translation_error("only one static allowed >:|",translation_data);
						break;

						case SC_EXTERN:
						case SC_TYPEDEF:
						push_translation_error("only one storage class allowed >:|",translation_data);
						break;
						default:
						assert(0);
					}
					return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
				}
				ret->storage_class=SC_STATIC;
				break;
			case KW_TYPEDEF:
				if(!parse_storage_class)
					goto exit;
				chomp(translation_data);
				if(ret->storage_class!=SC_NONE)
				{
					switch(ret->storage_class)
					{
						case SC_STATIC:
						case SC_EXTERN:
						case SC_TYPEDEF:
						push_translation_error("only one storage class allowed >:|",translation_data);
						break;
						default:
						assert(0);
					}
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
				chomp(translation_data);
				if(check(translation_data,KW_ID,0))
				{
					struct token *id;
					struct Denoted_Struct_Union *tag;
					id=Queue_Pop(translation_data->tokens);
					tag=(struct Denoted_Struct_Union*)check_tag(scope,id);

					if(tag==NULL)
					{
						struct Struct_Union *body;
						body=get_struct_union_base(scope,ret->specifier);
						Scope_Push(scope,get_denoted_struct_union(id,body));

						parse_struct_union_specifier_finish(translation_data,scope,body);
						ret->struct_union=body;
					}else
					{
						ret->struct_union=tag->struct_union;
						if(ret->struct_union->specifier!=ret->specifier)
						{
							push_translation_error("more than one type specifier",translation_data);
							return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
						}
						if(ret->struct_union->is_finished==0)
						{
							/*then this could be a definition*/
							parse_struct_union_specifier_finish(translation_data,scope,ret->struct_union);
						}
					}

				}else
				{
						ret->struct_union=get_struct_union_base(scope,ret->specifier);
						parse_struct_union_specifier_finish(translation_data,scope,ret->struct_union);
						if(ret->struct_union->is_finished==0)
						{
							push_translation_error("expected a struct tag or a struct definition",translation_data);
							return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
						}
				}
				break;
			case KW_ENUM:
				chomp(translation_data);
				ret->specifier=TS_ENUM;
				if(check(translation_data,KW_ID,0))
				{
					struct token *id;
					struct Denoted_Enum *enumerator;
					id=Queue_Pop(translation_data->tokens);
					enumerator=(struct Denoted_Enum*)check_tag(scope,id);
					if(enumerator==NULL)
					{
						struct Enum *body;
						body=get_enum_base();
						Scope_Push(scope,get_denoted_enum(id,body));
						parse_enum_specifier_finish(translation_data,scope,body);
						ret->enumerator=body;
					}else
					{
						ret->enumerator=enumerator->enumeration;
						if(enumerator->denotation!=DT_Enum)
						{
							return (struct Denotation_Prototype*)get_denoted_error((struct Denoted*)ret);
						}
						if(ret->enumerator->is_finished==0)
						{
							/*this could be an enum definition*/
							parse_enum_specifier_finish(translation_data,scope,ret->enumerator);
						}
					}

				}else
				{
					parse_enum_specifier_finish(translation_data,scope,ret->enumerator);
				}
				break;
			case KW_ID:
				if(ret->specifier==TS_NONE)
				{
					struct Denoted *hold;
					hold=check_ordinary(scope,(struct token*)translation_data->tokens->first->data);
					if(hold!=NULL && hold->denotation==DT_Typedef)
					{
						ret->type=((struct Denoted_Typedef*)hold)->type;
						chomp(translation_data);
						return ret;
					}
					/*falltrough - this has not been typedefed*/
				}
				/*falltrough (it is possible to overwrite typedef id from upper scope)*/
			default:
			exit:
				if(ret->specifier==TS_ENUM)
				{
					ret->type=(struct Type*)get_enum_type(ret);
				}else if(ret->specifier==TS_STRUCT || ret->specifier==TS_UNION)
				{
					ret->type=(struct Type*)get_struct_union_type(ret);
				}else if(ret->type==NULL)
				{
					ret->type=(struct Type*)get_basic_type(ret);
				}
				return ret;
		}
	}
}





/*
   	declarator:
		( pointer ( type-qualifier )* )* direct-declarator
 */
struct Denoted* parse_declarator(struct Translation_Data *translation_data,struct Scope *scope,struct Denotation_Prototype *prototype)
{
	struct Denoted_Base *temp;
	temp=get_denoted_base(prototype);
	parse_declarator_inner(translation_data,scope,temp);
	return extract_denoted(temp,prototype,0);

}

void parse_declarator_inner(struct Translation_Data *translation_data,struct Scope *scope,struct Denoted_Base *base)
{
	enum KEYWORDS hold;
	while(get_and_check(translation_data,KW_STAR))
	{
		base->type=(struct Type*)get_pointer_type(base->type);
		while(1)
		{
			hold=kw_get(translation_data);
			if(hold==KW_CONST)
			{
				((struct Type_Pointer*)base)->is_const=1;
			}else if(hold==KW_VOLATILE)
			{
				((struct Type_Pointer*)base)->is_volatile=1;
			}else
			{
				break;
			}
		}
	}
	parse_direct_declarator(translation_data,scope,base);

}
/*
	direct-declarator:
		id direct-declarator-finish
		( declarator ) direct-declarator-finish
*/
void parse_direct_declarator(struct Translation_Data *translation_data,struct Scope *scope,struct Denoted_Base *base)
{
	if(check(translation_data,KW_ID,0))
	{
		base->id=Queue_Pop(translation_data->tokens);
		parse_direct_declarator_finish(translation_data,scope,base);
		
	}else if(get_and_check(translation_data,KW_OPEN_NORMAL))
	{
		struct Queue *hack;
		struct Queue *hold;
		hack=malloc(sizeof(struct Queue));
		Queue_Init(hack);

		while(!check(translation_data,KW_CLOSE_NORMAL,0))
		{
			Queue_Push(hack,Queue_Pop(translation_data->tokens));
		}
		/*remove closing )*/
		chomp(translation_data);
		parse_direct_declarator_finish(translation_data,scope,base);



		hold=translation_data->tokens;
		translation_data->tokens=hack;
		parse_declarator_inner(translation_data,scope,base);
		translation_data->tokens=hold;
		if(hack->size!=0)
		{
			/*TODO error*/
			base->denotation=DT_Error;
			while(hack->size)
			{
				free(Queue_Pop(hack));
			}
			free(hack);
			push_translation_error("declarator error",translation_data);
			return;
		}
		free(hack);
		
	}else
	{
		/*this might be an abstract declarator*/
		parse_direct_declarator_finish(translation_data,scope,base);
	}
}

/*
	direct-declarator-finish:
		( [ constant-expression ] | (parameter-list) | ( [id-list] ) )* 
*/
void parse_direct_declarator_finish(struct Translation_Data *translation_data,struct Scope *scope,struct Denoted_Base *base)
{
	while(1)
	{
		if(get_and_check(translation_data,KW_OPEN_SQUARE))
		{
			if(get_and_check(translation_data,KW_CLOSE_SQUARE))
			{
				base->type=(struct Type*)get_array_type(base->type,NULL);
			}else
			{
				base->type=(struct Type*)get_array_type(base->type,parse_expression(translation_data,scope));
				if(!get_and_check(translation_data,KW_CLOSE_SQUARE))
				{
					/*TODO error*/
					push_translation_error("']' expected",translation_data);
					base->type=(struct Type*)get_type_error(base->type);
					return;
				}
			}
		}else if(get_and_check(translation_data,KW_OPEN_NORMAL))
		{
			struct Queue *parameters;
			struct Normal_Scope *function_prototype_scope;

			function_prototype_scope=(struct Normal_Scope*)get_normal_scope(scope,FUNCTION_PROTOTYPE_SCOPE);

			parameters=malloc(sizeof(struct Queue));
			Queue_Init(parameters);

			parse_paramenter_list(translation_data,function_prototype_scope,parameters);
			base->type=(struct Type*)get_function_type(base->type,parameters,function_prototype_scope);
			
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
void parse_struct_union_specifier_finish(struct Translation_Data *translation_data,struct Scope *scope,struct Struct_Union *base)
{
	if(get_and_check(translation_data,KW_OPEN_CURLY))
	{
		base->is_finished=1;
		while(parse_struct_declaration(translation_data,(struct Scope*)base->inner_namespace,base->members))
		{
			
			if(get_and_check(translation_data,KW_CLOSE_CURLY))
			{
				return ;
			}
		}

		/*TODO error*/
		push_translation_error("expected closing curly bracket from struct declaration",translation_data);
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
char parse_struct_declaration(struct Translation_Data *translation_data,struct Scope *struct_scope,struct Queue* members)
{
	struct Denotation_Prototype *prototype;
	struct Denoted *hold;
	prototype=parse_specifier_qualifier_list(translation_data,struct_scope);
	while(1)
	{
		hold=parse_struct_declarator(translation_data,struct_scope,prototype);
		if(hold!=NULL && hold->denotation!=DT_Error)
		{
			Scope_Push(struct_scope,hold);
			Queue_Push(members,hold);

		}else
		{
			free(prototype);
			/*todo error*/
			push_translation_error("there is a problem with the declarator",translation_data);
			return 0;
		}
		if(!get_and_check(translation_data,KW_COMMA))
		{
			if(get_and_check(translation_data,KW_SEMI_COLUMN))
			{
				break;
			}else
			{
				free(prototype);
				push_translation_error("semi column expected in struct declaration",translation_data);
				/*todo error*/
				return 0;
			}
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
struct Denoted* parse_struct_declarator(struct Translation_Data *translation_data,struct Scope *scope,struct Denotation_Prototype *prototype)
{
	struct Denoted *hold;
	if(get_and_check(translation_data,KW_COLUMN))
	{
		/*unnamed bitfields are possible*/
		struct Denoted_Object *obj;
		obj=(struct Denoted_Object*)get_denoted_object(NULL,SC_NONE,prototype->type);
		obj->object->type=(struct Type*)get_type_bitfield(prototype->type,parse_expression(translation_data,scope));
		return (struct Denoted*)obj;

	}else
	{
		hold=parse_declarator(translation_data,scope,prototype);
		if(get_and_check(translation_data,KW_COLUMN))
		{
			if(hold->denotation==DT_Object)
			{
				/*TODO make bitfield*/
				return hold;
			}else
			{
				/*TODO error*/
				push_translation_error("bitfield must be ",translation_data);
				return get_denoted_error(hold);
			}
		}else
		{
			return hold;
		}
	}
	
}
/*
	enum-specifier-finish
	{ ( enumeration-constant [ = constant-expression ] , )* }
*/
void parse_enum_specifier_finish(struct Translation_Data *translation_data,struct Scope *scope,struct Enum *enumeration)
{
	struct token *id;
	struct Denoted_Enum_Const *hold;
	int where_in_enumeration=0;
	if(get_and_check(translation_data,KW_OPEN_CURLY))
	{
		enumeration->is_finished=1;
		do
		{
			if(check(translation_data,KW_ID,0))
			{
				id=Queue_Pop(translation_data->tokens);
				if(get_and_check(translation_data,KW_EQ))
				{
					hold=(struct Denoted_Enum_Const*)get_denoted_enum_const_expr(id,enumeration,parse_expression(translation_data,scope));
					Queue_Push(enumeration->consts,hold);
					where_in_enumeration=hold->value+1;
				}else
				{
					Queue_Push(enumeration->consts,get_denoted_enum_const_num(id,enumeration,where_in_enumeration));
					++where_in_enumeration;
				}
				if(!get_and_check(translation_data,KW_COMMA) && get_and_check(translation_data,KW_CLOSE_CURLY))
				{
					return;
				}else
				{
					/*TODO error*/
					push_translation_error("enum definition error",translation_data);
					Queue_Push(enumeration->consts,get_denoted_error(NULL));
					return ;
				}
			}else
			{
				/*TODO error*/
				push_translation_error("enum definition error, expected an id",translation_data);
				Queue_Push(enumeration->consts,get_denoted_error(NULL));
				return ;
			}
		}while(!get_and_check(translation_data,KW_CLOSE_CURLY));

	}
}

/*
	parameter-list:
		(declaratoion-specifiers (declarator | abstract-declarator),)+
*/
void parse_paramenter_list(struct Translation_Data *translation_data,struct Normal_Scope *function_prototype_scope,struct Queue *parameters)
{
	if(get_and_check(translation_data,KW_CLOSE_NORMAL))
		return;

	struct Denotation_Prototype *prototype;
	struct Denoted_Base *base;
	struct Denoted *hold;
	do
	{
		

		prototype=parse_declaration_specifiers(translation_data,(struct Scope*)function_prototype_scope);
		base=get_denoted_base(prototype);


		parse_declarator_inner(translation_data,(struct Scope*)function_prototype_scope,base);
		if(base->denotation!=DT_Object)
		{
			/*TODO error*/
			push_translation_error("expected object declaration in function prototype",translation_data);
		//	delete_denoted(hold);
			delete_denoted_prototype(prototype);
			delete_denoted_base(base);	
			return;
		}

		hold=extract_denoted(base,prototype,1);

		Scope_Push((struct Scope*)function_prototype_scope,hold);
		Queue_Push(parameters,((struct Denoted_Object*)hold));

		delete_denoted_prototype(prototype);
		delete_denoted_base(base);	

	}while(get_and_check(translation_data,KW_COMMA));
	if(!get_and_check(translation_data,KW_CLOSE_NORMAL))
	{
		/*TODO error*/
		push_translation_error("expected a ')' finishing the parameter list",translation_data);
		Queue_Push(parameters,get_denoted_error(NULL));
		return;
	}

}
/*
	type-name:
		specifier-qualifier-list [abstract-declarator]
*/
struct Type* parse_type_name(struct Translation_Data *translation_data,struct Scope *scope)
{
	struct Denotation_Prototype *prototype;
	struct Type *ret;
	prototype=parse_specifier_qualifier_list(translation_data,scope);
	ret=parse_abstract_declarator(translation_data,scope,prototype);
	delete_denoted_prototype(prototype);
	return ret;
}
/*
   	abstract-declarator:
		( pointer )* abstract-direct-declarator
*/
struct Type* parse_abstract_declarator(struct Translation_Data *translation_data,struct Scope *scope,struct Denotation_Prototype *prototype)
{
	struct Denoted_Base *base;
	struct Type *ret;


	get_denoted_base(prototype);
	parse_declarator_inner(translation_data,scope,base);

	if(base->denotation==DT_Error || base->id!=NULL)
	{
		/*TODO error*/
		push_translation_error("unexpedted id in abstract declarator",translation_data);
		delete_denoted_prototype(prototype);
		ret=base->type;
		delete_denoted_base(base);
		return ret;
	}
	delete_denoted_prototype(prototype);
	ret=base->type;
	delete_denoted_base(base);
	return ret;

}

/*
	initializer:
		assignment-expression
		{ initializer , ... [,] }
*/
struct AST* parse_initializer(struct Translation_Data *translation_data,struct Scope *scope,struct Denoted_Object *base)
{
	return parse_assignment_expression(translation_data,scope);
}

const const const const const const const const const const const const const const const const const const const const const const const char const const const constant;
#endif
