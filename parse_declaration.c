#ifndef GCC_PARSE_DECLARATION_CCOMMA
#define GCC_PARSE_DECLARATION_C GCC_PARSE_DECLARATION_C
#include"parse_declaration.h"




/*
	declaration-specifiers:
		storage-class-specifier [ declaration-specifiers ]
		type-specifier [ declaration-specifiers ]
		type-qualifier [ declaration-specifiers ]
		function-spedifier [ declaration-specifiers ]
		alignment-specifier [ declaration-specifiers ]
		
*/
struct Denoted* parse_declaration_specifiers(struct Queue *tokens,struct Scope *scope)
{
	struct Object_Prototype *hold;
	enum KEYWORDS kw;
	hold=(struct Object_Prototype*)get_object_prototype();

	for(kw=kw_get(tokens);tokens->size>0;chomp(tokens),kw=kw_get(tokens))
	{
		switch(kw)
		{
			case KW_CONST:	
				hold->is_const=1;
				break;
			case KW_VOLATILE:
				hold->is_volatile=1;
				break;
			case KW_STATIC:
				if(hold->storage_class==TSC_NONE)
				{
					hold->storage_class=TSC_STATIC;
				}else
				{
					return get_denoted_error(get_denoted_object((struct Type*)hold));
				}
				break;
			case KW_EXTERN:
				if(hold->storage_class==TSC_NONE)
				{
					hold->storage_class=TSC_EXTERN;
				}else
				{
					return get_denoted_error(get_denoted_object((struct Type*)hold));
				}
				break;
			case KW_INT:
				if(hold->specifier!=TS_NONE)
				{
					return get_denoted_error(get_denoted_object((struct Type*)hold));
				}
				hold->specifier=TS_INT;
				break;
			case KW_CHAR:
				if(hold->specifier!=TS_NONE)
				{
					return get_denoted_error(get_denoted_object((struct Type*)hold));
				}
				hold->specifier=TS_CHAR;
				break;
			case KW_VOID:
				if(hold->specifier!=TS_NONE)
				{
					return get_denoted_error(get_denoted_object((struct Type*)hold));
				}
				hold->specifier=TS_VOID;
				break;
			case KW_DOUBLE:
				if(hold->specifier!=TS_NONE)
				{
					return get_denoted_error(get_denoted_object((struct Type*)hold));
				}
				hold->specifier=TS_DOUBLE;
				break;
			case KW_FLOAT:
				if(hold->specifier!=TS_NONE)
				{
					return get_denoted_error(get_denoted_object((struct Type*)hold));
				}
				hold->specifier=TS_FLOAT;
				break;
			case KW_UNION:
			case KW_STRUCT:
				if(check(tokens,KW_ID,1) )
				{
					/*this struct might already have been defined*/
					struct Denoted_Struct_Union *hold_su;
					struct token *tag=tokens->first->prev->data;
					hold_su=check_tag(scope,tag);

					if(hold_su==NULL)
					{
						/*this tag isn't taken*/
						/*denote an incomplete struct/union type*/
						hold_su=get_denoted_struct_union(tag,get_struct_union());
						
					}
					/*consume struct/union tag*/
					chomp(tokens);
					return get_denoted_object(get_struct_union_type(hold,hold_su->struct_union));

				}else if(check(tokens,KW_OPEN_CURLY,1))
				{
					/*this is a definition of an anonymous struct/union*/
					return get_denoted_struct_union(NULL,parse_struct_union_specifier(tokens,scope))
				}else
				{
					return get_denoted_error(get_denoted_object((struct Type*)hold));
				}
			case KW_TYPEDEF:
				hold->is_typedef=1;
				break;
			case KW_ID:
				if(hold->specifier==TS_NONE && check_if_typedefed(scope,tokens->first->data))
				{
					hold->type_def=check_ordinary(scope,tokens->first->data);
					merge_type_nodes(base,check_base_type_component(hold->type_def));
					break;
				}else
				{
					return base;
				}
			default:
				return base;
		}
	}
}



/*
          pointer:
                  *  type-qualifier-list&lt;opt&gt;
                  *  type-qualifier-list&lt;opt&gt; pointer
*/
void parse_pointer(struct Queue *tokens,struct Declarator *upper,struct Scope *scope)
{
	struct Type_Node *temp;
	while(get_and_check(tokens,KW_STAR))
	{
		temp=parse_declaration_specifiers(tokens,scope);
		Queue_Push_Front(&(upper->type->components),temp);
		if(temp->type_specifier!=TS_NONE || temp->is_extern!=0 || temp->is_static!=0 || temp->is_signed!=0)
		{
			/*TODO error*/
			temp->error=1;
			upper->initialiser=(struct AST*)get_error_tree(NULL);
			return;
		}else
		{
			temp->type_specifier=TS_POINTER;
		}
	}
}

/*
	paramenter-list:
		parameter-declaration
		parameter-list, prameter-declaration
*/
void parse_parameter_list(struct Queue *tokens,struct Type_Node *function_base,struct Scope *scope)
{
	assert(function_base->type_specifier==TS_FUNC);
	do
	{
		parse_parameter_declaration(tokens,function_base,scope);
	}while(get_and_check(tokens,KW_COMMA));

}
/*
   ?
	parameter-type-list
		parameter-list , ...
*/
void parse_parameter_type_list(struct Queue *tokens,struct Type_Node *function_base,struct Scope *scope)
{
	parse_parameter_list(tokens,function_base,scope);
}
struct AST* parse_initialiser(struct Queue *tokens,struct Scope *scope)
{
	return parse_assignment_expression(tokens,scope);
}
/*
	init-declarator:
		declarator = initialiser
*/
struct Declarator* parse_init_declarator(struct Queue *tokens,struct Type_Node *base,struct Scope *scope)
{
	struct Declarator *ret;
	ret=get_declarator(base);
	parse_declarator(tokens,ret,scope);
	if(get_and_check(tokens,KW_EQ))
	{
		ret->initialiser=parse_initialiser(tokens,scope);
	}
	return ret;
}
/*
          declarator:
                  pointer
                  [ pointer ] direct-declarator
*/
void parse_declarator(struct Queue *tokens,struct Declarator *base,struct Scope *scope)
{
	parse_pointer(tokens,base,scope);
	parse_direct_declarator(tokens,base,scope);
}

/*
direct-declarator-finish:
	( direct-declarator-finish-step )*
*/
void parse_direct_declarator_finish(struct Queue *tokens,struct Type *upper,struct Scope *scope)
{
	while(parse_direct_declarator_finish_step(tokens,upper,scope));
	upper->size=size_of(upper);
}

/*
direct-declarator-finish-step:
	\[ constant-expression \]
	\[ \]
	\[ * \]
	\( parameter-type-list \)
*/
char parse_direct_declarator_finish_step(struct Queue *tokens,struct Type *upper,struct Scope *scope)
{
	struct Type_Node *temp;
	if(get_and_check(tokens,KW_OPEN_SQUARE))
	{
		temp=get_node();
		temp->type_specifier=TS_ARRAY;
		temp->type_constraints=TC_NONE;
		temp->specifics.arr.number_of_elements_expr=NULL;

		if(get_and_check(tokens,KW_CLOSE_SQUARE))
		{
			Queue_Push_Front(&upper->components,temp);
			return 1;
		}
		if(check(tokens,KW_STAR,0) && check(tokens,KW_CLOSE_SQUARE,1))
		{
			chomp(tokens);
			chomp(tokens);
			Queue_Push_Front(&upper->components,temp);
			return 1;
		}
		temp->specifics.arr.number_of_elements=evaluate_const_expression_integer( 
			 temp->specifics.arr.number_of_elements_expr=parse_const_expression(tokens,scope)
			);

		if(!get_and_check(tokens,KW_CLOSE_SQUARE) || temp->specifics.arr.number_of_elements_expr<=0)
		{
			temp->error=1;
			Queue_Push_Front(&upper->components,temp);
			return 0;
		}
		Queue_Push_Front(&upper->components,temp);
		return 1;
	}else if(get_and_check(tokens,KW_OPEN_NORMAL))
	{
		temp=get_node();
		temp->type_specifier=TS_FUNC;
		temp->type_constraints=TC_NONE;
		Queue_Init(&temp->specifics.arg_types);

		parse_parameter_list(tokens,temp,scope);


		if(!get_and_check(tokens,KW_CLOSE_NORMAL))
		{
			temp->error=1;
			Queue_Push_Front(&upper->components,temp);
			return 0;
		}
		Queue_Push_Front(&upper->components,temp);
		return 1;
	}else
	{
		return 0;
	}
}


/*
	direct-declarator:
		id direct-declarator-finish
		( declarator ) direct-declarator-finish
*/
void parse_direct_declarator(struct Queue *tokens,struct Declarator *upper,struct Scope *scope)
{
	struct Type_Node *temp;
	struct Type *hold_type;
	struct Type *swap_type;

	if(check(tokens,KW_ID,0))
	{
		upper->id=Queue_Pop(tokens);
		parse_direct_declarator_finish(tokens,upper->type,scope);
	}else if(get_and_check(tokens,KW_OPEN_NORMAL))
	{

		hold_type=upper->type;

		upper->type=malloc(sizeof(struct Type));
		Queue_Init(&(upper->type->components));
		parse_declarator(tokens,upper,scope);

		swap_type=hold_type;
		hold_type=upper->type;
		upper->type=swap_type;

		if(get_and_check(tokens,KW_CLOSE_NORMAL))
		{
			parse_direct_declarator_finish(tokens,upper->type,scope);
			Queue_Append(&hold_type->components,&upper->type->components);

			free(upper->type);

			upper->type=hold_type;
		}else
		{
			/*error*/
			temp=malloc(sizeof(struct Type_Node));
			temp->type_specifier=TS_ERROR;
			temp->error=1;
			Queue_Push_Front(&hold_type->components,temp);

			Queue_Append(&hold_type->components,&upper->type->components);

			free(upper->type);

			upper->type=hold_type;
		}
	}else
	{
		temp=malloc(sizeof(struct Type_Node));
		temp->type_specifier=TS_ERROR;
		temp->error=1;
		Queue_Push_Front(&upper->type->components,temp);
	}
}


/*
delcaration:
	declaration-specifiers [ init-declarator-list ] ;
*/

struct AST* parse_declaration(struct Queue *tokens,struct Scope *scope)
{
	struct AST_Declaration *ret;
	struct Declarator *hold;
	ret=get_declaration_tree(scope);
	ret->base_type=parse_declaration_specifiers(tokens,scope);
	
	if(get_and_check(tokens,KW_SEMI_COLUMN))
	{
		return (struct AST*)ret;
	}
	do
	{
		hold=parse_init_declarator(tokens,ret->base_type,scope);
		Queue_Push(&ret->declarators,hold);
		if(hold->id==NULL || !Scope_Push(scope,hold))
			return (struct AST*)get_error_tree((struct AST*)ret);
	
	}while(get_and_check(tokens,KW_COMMA));

	if(get_and_check(tokens,KW_SEMI_COLUMN))
	{
		return (struct AST*)ret;
	}else
	{
		return (struct AST*)get_error_tree((struct AST*)ret);
	}


}
/*
	abstract-declarator:
		pointer
		[ pointer ] direct-abstract-declarator
*/
void parse_abstract_declarator(struct Queue *tokens,struct Declarator *base,struct Scope *scope)
{
	parse_pointer(tokens,base,scope);
	parse_direct_abstract_declarator(tokens,base,scope);
}
/*

	direct-abstract-declarator:
		direct-declarator-finish
		( abstract-declarator ) direct-declarator-finish
   */
void parse_direct_abstract_declarator(struct Queue *tokens,struct Declarator *upper,struct Scope *scope)
{
	struct Type_Node *temp;
	struct Type *hold_type;
	struct Type *swap_type;

	if(get_and_check(tokens,KW_OPEN_NORMAL))
	{

		hold_type=upper->type;

		upper->type=malloc(sizeof(struct Type));
		Queue_Init(&(upper->type->components));
		parse_abstract_declarator(tokens,upper,scope);

		swap_type=hold_type;
		hold_type=upper->type;
		upper->type=swap_type;

		if(get_and_check(tokens,KW_CLOSE_NORMAL))
		{
			parse_direct_declarator_finish(tokens,upper->type,scope);
			Queue_Append(&hold_type->components,&upper->type->components);

			free(upper->type);

			upper->type=hold_type;
		}else
		{
			/*error*/
			temp=malloc(sizeof(struct Type_Node));
			temp->type_specifier=TS_ERROR;
			temp->error=1;
			Queue_Push_Front(&hold_type->components,temp);

			Queue_Append(&hold_type->components,&upper->type->components);

			free(upper->type);

			upper->type=hold_type;
		}
	}else
	{
		parse_direct_declarator_finish(tokens,upper->type,scope);
	}
}
/*
	declaration-specifiers declarator
	declaration-specifiers [ abstract declaration ]
*/
void parse_parameter_declaration(struct Queue *tokens,struct Type_Node *function_base,struct Scope *scope)
{
	struct Type_Node *base;
	struct Declarator *hold;
	assert(function_base->type_specifier==TS_FUNC);
	base=parse_declaration_specifiers(tokens,scope);
	hold=get_declarator(base);
	parse_declarator(tokens,hold,scope);
	Queue_Push(&function_base->specifics.arg_types,hold);
}
struct Type_Node* parse_specifier_qualifier_list(struct Queue *tokens,struct Scope *scope)
{
	struct Type_Node *ret;
	ret=parse_declaration_specifiers(tokens,scope);
	if(ret->type_specifier==TS_NONE || ret->is_extern==1 || ret->is_static==1 || ret->is_signed==1)
	{
		ret->error=1;
	}
	return ret;
}
/*
type-name:
	specifier-qualifier-list abstract-declarator;

*/
struct Type* parse_type_name(struct Queue *tokens,struct Scope *scope)
{
	struct Declarator *declarator;
	struct Type *ret;
	struct Type_Node *base;

	declarator=malloc(sizeof(struct Declarator));
	declarator->type=malloc(sizeof(struct Type));
	declarator->id=NULL;
	Queue_Init(&declarator->type->components);

	base=parse_specifier_qualifier_list(tokens,scope);
	Queue_Push(&declarator->type->components,base);
	if(base->error)
	{
		ret=declarator->type;
		free(declarator);
		return ret;
	}
	parse_abstract_declarator(tokens,declarator,scope);

	if(declarator->id!=NULL)
	{
		base->error=1;
	}
	ret=declarator->type;
	free(declarator);
	return ret;

}

/*
struct-declarator:
	declarator
	[ declarator ] : constant-expression

  */
void parse_struct_declarator(struct Queue *tokens,struct Declarator *base,struct Scope *scope)
{
	struct Type_Node *hold;
	if(get_and_check(tokens,KW_COLUMN))
	{
		hold=get_node();
		hold->is_bit_field=1;
		hold->specifics.bit_field_length=parse_const_expression(tokens,scope);
		Queue_Push(&base->type->components,hold);
	}else
	{
		parse_declarator(tokens,base,scope);
		if(get_and_check(tokens,KW_COLUMN))
		{
			hold=get_node();
			hold->is_bit_field=1;
			hold->specifics.bit_field_length=parse_const_expression(tokens,scope);
			Queue_Push(&base->type->components,hold);
		}
	}
}
/*
	struct-declarator-list:
		struct-declarator (,struct-declarator)*
*/
void parse_struct_declarator_list(struct Queue *tokens,struct Queue *declarators,struct Type_Node *base_type,struct Scope *scope)
{
	struct Declarator *struct_declaration;
	do
	{
		struct_declaration=get_declarator(base_type);
		parse_struct_declarator(tokens,struct_declaration,scope);
		Queue_Push(declarators,struct_declaration);
	}while(get_and_check(tokens,KW_COMMA));
}

/*
   struct-declaration:
   	spedifier-qualifier-list  [ struct-declarator-list ] ;
*/
void parse_struct_declaration(struct Queue *tokens,struct Type_Node *struct_union_base,struct Scope *scope)
{
	struct AST_Declaration *hold;
	hold=get_declaration_tree(scope);

	hold->base_type=parse_specifier_qualifier_list(tokens,scope);
	parse_struct_declarator_list(tokens,&hold->declarators,hold->base_type,scope);

	Queue_Push(&struct_union_base->specifics.struct_union->declarations,hold);

	if(!get_and_check(tokens,KW_SEMI_COLUMN))
	{
		struct_union_base->error=1;
	}
	
}
void parse_struct_declaration_list(struct Queue *tokens,struct Type_Node *struct_union_base,struct Scope *scope)
{
	do
	{
		parse_struct_declaration(tokens,struct_union_base,scope);
	}while(!( check(tokens,KW_CLOSE_CURLY,0) || struct_union_base->error));
}

/*
	struct-or-union-specifier:
		struct-or-union [ identifier ] { struct-declaration-list }
		struct-or-union identifier
*/
struct Type_Node* parse_struct_union_specifier(struct Queue *tokens,struct Scope *scope)
{
	struct Type_Node *hold;
	hold=get_node();
	if(get_and_check(tokens,KW_STRUCT))
	{
		hold->type_specifier=TS_STRUCT;
	}else if(get_and_check(tokens,KW_UNION))
	{
		hold->type_specifier=TS_UNION;
	}else
	{
		hold->type_specifier=TS_NONE;
		hold->error=1;
		return hold;
	}
	
	hold->specifics.struct_union=get_struct_union();
	if(check(tokens,KW_ID,0))
	{
		hold->specifics.struct_union->id=Queue_Pop(tokens);
	}
	if(get_and_check(tokens,KW_OPEN_CURLY))
	{
		parse_struct_declaration_list(tokens,hold,scope);
		if(get_and_check(tokens,KW_CLOSE_CURLY))
		{
			return hold;
		}else
		{
			hold->error=1;
			return hold;
		}
	}else
	{

		hold->error=1;
		return hold;
	}
}
struct Struct_Union* get_struct_union()
{
	struct Struct_Union* ret;
	ret=malloc(sizeof(struct Struct_Union));
	Queue_Init(&ret->declarations);
	Map_Init(&ret->inner_namespace);
	ret->id=NULL;
	return ret;
}
char is_type(struct Queue *tokens,struct Scope *scope)
{
	
	enum KEYWORDS kw;
	kw=kw_get(tokens);
	switch(kw)
	{
		case KW_CONST:	
		case KW_VOLATILE:
		case KW_STATIC:
		case KW_TYPEDEF:
		case KW_EXTERN:
		case KW_INT:
		case KW_CHAR:
		case KW_VOID:
		case KW_DOUBLE:
		case KW_FLOAT:
		case KW_UNION:
		case KW_STRUCT:
		case KW_ENUM:
			return 1;
		case KW_ID:
			return check_if_typedefed(scope,tokens->first->data);
		default:
			return 0;
	}
}

struct Declarator* get_declarator(struct Type_Node *base_type)
{
	struct Declarator *ret;
	ret=malloc(sizeof(struct Declarator));
	ret->id=NULL;
	ret->initialiser=NULL;
	ret->type=malloc(sizeof(struct Type));
	Queue_Init(&ret->type->components);
	Queue_Push(&ret->type->components,base_type);
	ret->location=NULL;

	return ret;
}
#endif
