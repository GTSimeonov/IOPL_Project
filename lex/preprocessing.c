#ifndef GCC_PREPROCESSING_C
#define GCC_PREPROCESSING_C GCC_PREPROCESSING_C
#include <preprocessing.h>


/*we have skipped the leading #*/
/*
 	#include string
	#include <qchar>
	#define [ id(list) replacement
	#undef [ id ]
	#line number [string]
	#if
	#ifdef
	#ifndef
	#pragma
	#error
	#

	#elif
	#else
	#endif

	
*/
void parse_preproc_line(struct Source_File *src,struct Translation_Data *translation_data)
{
	struct token *hold;
	hold=get_next_token(src,&chonky_jr[0],0);
	switch(hold->type)
	{
		case PKW_INCLUDE:
			free(hold);
			parse_include_line(src,translation_data);
			return;
		case PKW_DEFINE:
			free(hold);
			parse_define_line(src,translation_data);
			return;
		case PKW_IF:
			free(hold);
			parse_preproc_if_line(src,translation_data);
			return;
		case PKW_IFDEF:
			free(hold);
			parse_preproc_ifdef_line(src,translation_data);
			return;
		case PKW_IFNDEF:
			free(hold);
			parse_preproc_ifndef_line(src,translation_data);
			return;
		case PKW_UNDEF:
			free(hold);
			parse_preproc_undef_line(src,translation_data);
			return;
		case PKW_ENDIF:
			free(hold);
			push_lexing_error("unmatched endif",src,translation_data);
			return;
		case PKW_ELSE:
			free(hold);
			push_lexing_error("unmatched else",src,translation_data);
			return;
		case PKW_ELIF:
			free(hold);
			push_lexing_error("unmatched elif",src,translation_data);
			return;
		default:
			free(hold);
			/*TODO error*/
			push_lexing_error("expected a preprocessing directive",src,translation_data);
			return;

	}
}
void parse_include_line(struct Source_File *src,struct Translation_Data *translation_data)
{
	struct token *hold;
	hold=get_next_token(src,&chonky[0],0);
	if(hold->type==KW_STRING)
	{
		char *where_to_search[]={src->src_name->base,NULL};
		struct Source_File *hold_file;

		hold->data[hold->data_size-1]='\0';
		hold->data_size-=2;
		++hold->data;
		handle_splicing(hold);


		/*search in the directory of the file from which we include*/
		hold_file=get_source_file(hold->data,where_to_search);
		/*fallback to well known locations == <>*/
		if(hold_file==NULL)
		{
			hold_file=get_source_file(hold->data,well_known_locations_base);
			if(hold_file==NULL)
			{
				/*TODO error*/
				push_lexing_error("file in include directive not found",src,translation_data);
				free(hold);
				return;
			}
		}
		lex_program(translation_data,hold_file);
		free(hold);
	}else if(hold->type==KW_LESS)/*hack*/
	{
		struct Source_File *hold_file;
		++hold->data;
		while(src->src[src->where_in_src]!='>' && src->where_in_src<src->src_size)
		{
			++src->where_in_src;
			++hold->data_size;
		}
		if(src->where_in_src==src->src_size)
		{
			/*TODO error*/
			free(hold);
			return;
		}
		/*skip the >*/
		++src->where_in_src;
		hold->data[hold->data_size-1]='\0';
		handle_splicing(hold);
		
		hold_file=get_source_file(hold->data,well_known_locations_base);
		if(hold_file==NULL)
		{
			/*TODO error*/
			push_lexing_error("file in include directive not found",src,translation_data);
			free(hold);
			return;
		}

		lex_program(translation_data,hold_file);
		free(hold);

	}else
	{
		/*TODO error*/
		push_lexing_error("include error",src,translation_data);
		free(hold);
		return;
	}


	chase_new_line(src,translation_data);
}

/*skipped # and 'define'*/
void parse_define_line(struct Source_File *src,struct Translation_Data *translation_data)
{
	struct token *hold_token;
	struct token *macro_name;
	struct define_directive *new_macro;
	struct Queue *hold_tokens;
	size_t number_of_arguments=0;
	int *hold_index;


	macro_name=get_next_token(src,&chonky[0],0);
	if(macro_name->type!=KW_ID)
	{
		free(macro_name);
		push_lexing_error("expected id after #define",src,translation_data);
		return;
	}

	new_macro=get_define_directive(macro_name);
	/*white space*/
	hold_token=get_next_token(src,&chonky[0],0);
	if(hold_token->type==KW_OPEN_NORMAL)
	{
		free(hold_token);
		while(1)
		{
			hold_token=get_next_token(src,&chonky[0],0);
			if(hold_token->type!=KW_ID)
			{
				push_lexing_error("expected id in define argument list",src,translation_data);
				free(hold_token);
				break;
			}
			hold_index=malloc(sizeof(int));
			*hold_index=number_of_arguments;
			++number_of_arguments;
			Map_Push(new_macro->arguments,hold_token->data,hold_token->data_size,hold_index);
			free(hold_token);
			hold_token=get_next_token(src,&chonky[0],0);
			if(hold_token->type!=KW_COMMA)
			{
				if(hold_token->type==KW_CLOSE_NORMAL)
				{
					free(hold_token);
					break;
				}else
				{
					push_lexing_error("expected ',' in define argument list",src,translation_data);
					free(hold_token);
					break;
				}
			}
			free(hold_token);
		}	
		hold_token=get_next_token(src,&chonky[0],0);

	}else if(hold_token->type==KW_NOTYPE)
	{
		//push_lexing_error("empty define directive",src,translation_data);
		free(hold_token);
		/*TODO destroy new define directive*/
		/*TODO there is a memory leak here*/
	//	return ;	
	}

/*push things*/

	hold_tokens=translation_data->tokens;
	translation_data->tokens=new_macro->macro_tokens;

	new_macro->number_of_arguments=number_of_arguments;
/*there is something in hold_token*/
	do{
		expand_macro(hold_token,src,translation_data);
	}while( (hold_token=get_next_token(src,&chonky[0],0))->type != KW_NOTYPE);

/*removing the notype token*/
	free(hold_token);

	translation_data->tokens=hold_tokens;
/*push the directive into the macro map*/
	Map_Push(translation_data->macros,macro_name->data,macro_name->data_size,new_macro);
	//free(macro_name);
	chase_new_line(src,translation_data);

}
/*
	id[(list)] tokens \n 
 */
struct define_directive* get_define_directive(struct token* macro_name)
{
	struct define_directive *ret;
	ret=malloc(sizeof(struct token));
	ret->macro_name=macro_name;

	ret->macro_tokens=malloc(sizeof(struct Queue));
	Queue_Init(ret->macro_tokens);

	ret->arguments=malloc(sizeof(struct Map));
	Map_Init(ret->arguments);

	ret->number_of_arguments=0;

	return ret;
}

/*returns an array of queues*/
struct Queue* make_define_argument_list(size_t number_of_arguments)
{
	size_t i;
	struct Queue *ret;

	if(number_of_arguments==0)
		return NULL;

	ret=malloc(sizeof(struct Queue)*number_of_arguments);

	for(i=0;i<number_of_arguments;++i)
	{
		Queue_Init(ret+i);
	}
	return ret;
}
void delete_define_argument_list(size_t number_of_arguments,struct Queue *args)
{
	if(number_of_arguments==0)
	{
		assert(args==NULL);
		return;
	}
	flush_macro_arguments(number_of_arguments,args);
	free(args);
}

void expand_macro_argument(struct Queue *replacement_tokens,struct Source_File *src,struct Translation_Data *translation_data)
{
	struct Queue_Node *it;
	struct token *hold_token;
	for(it=replacement_tokens->first;it!=NULL;it=it->prev)
	{
		hold_token=copy_token((struct token*)it->data);
		hold_token->line=src->which_row;
		hold_token->column=src->which_column;
		Queue_Push(translation_data->tokens,hold_token);
		//Queue_Push(translation_data->tokens,copy_token((struct token*)it->data));
	}
}
void load_macro_arguments(struct Queue  *args,size_t number_of_arguments,struct Source_File *src,struct Translation_Data *translation_data)
{
	struct token *hold;
	struct Queue *hack;
	size_t i;
	size_t j;

	if(number_of_arguments==0)
		return;

	hold=get_next_token(src,&chonky[0],1);
	if(hold->type!=KW_OPEN_NORMAL)
	{
		push_lexing_error("expected '(' in macro expansion",src,translation_data);
		free(hold);
		return;
	}
	free(hold);

	hack=translation_data->tokens;
	for(i=0;i<number_of_arguments-1;++i)
	{
		translation_data->tokens=args+i;
		for(
		hold=get_next_token(src,&chonky[0],1),j=0;
		hold->type!=KW_COMMA && hold->type!=KW_NOTYPE;
		hold=get_next_token(src,&chonky[0],1),++j
		   )
		{
			expand_macro(hold,src,translation_data);
		}
		if(hold->type==KW_NOTYPE)
		{
			push_lexing_error("expected ',' in macro argument list",src,translation_data);
			free(hold);
			goto cleanup;
		}
		if(j==0)
		{
			push_lexing_error("expected argument in macro argument list",src,translation_data);
			free(hold);
			goto cleanup;
		}
	
	}
	translation_data->tokens=args+i;
	for(
	hold=get_next_token(src,&chonky[0],1),j=0;
	hold->type!=KW_CLOSE_NORMAL;
	hold=get_next_token(src,&chonky[0],1),++j
	   )
	{
		if(hold->type==KW_NOTYPE)
		{
			push_lexing_error("expected ')' in macro argument list",src,translation_data);
			free(hold);
			goto cleanup;
		}
		expand_macro(hold,src,translation_data);
	}
		if(j==0)
		{
			push_lexing_error("expected argument in macro argument list",src,translation_data);
			free(hold);
		}
	
cleanup:
	translation_data->tokens=hack;
	

}
void flush_macro_arguments(size_t number_of_arguments,struct Queue *args)
{
	size_t i;
	for(i=0;i<number_of_arguments;++i)
	{
		while(args[i].size>0)
			free(Queue_Pop(args+i));
	}	
}
/*macro name token is freed on expansion , if it is not a macro name it is pushed into token queue*/
void expand_macro(struct token* macro_name,struct Source_File *src,struct Translation_Data *translation_data)
{
	struct define_directive *hold=NULL;
	struct token *hold_token;
	int *index;
	struct Queue_Node *it;
	struct Queue *argument_list;

	if(macro_name->type==KW_ID)
		hold=Map_Check(translation_data->macros,macro_name->data,macro_name->data_size);
	if(hold!=NULL)
	{
		free(macro_name);
		argument_list=make_define_argument_list(hold->number_of_arguments);
		load_macro_arguments(argument_list,hold->number_of_arguments,src,translation_data);
		if(translation_data->errors->size>0)
		{
			delete_define_argument_list(hold->number_of_arguments,argument_list);
			return;
		}

		
		for(it=hold->macro_tokens->first;it!=NULL;it=it->prev)
		{
			hold_token=(struct token*)it->data;
			index=Map_Check(hold->arguments,hold_token->data,hold_token->data_size);
			if(index!=NULL)
			{
				expand_macro_argument(argument_list+*index,src,translation_data);
			}else
			{
				hold_token=copy_token(hold_token);

				hold_token->line=src->which_row;
				hold_token->column=src->which_column;

				Queue_Push(translation_data->tokens,hold_token);
			}
		}
		delete_define_argument_list(hold->number_of_arguments,argument_list);
	}else
	{
		/*this isn't a macro, so we just push it to the token queue*/
		Queue_Push(translation_data->tokens,macro_name);
	}
}
void preproc_lex_first_part(struct Source_File *src,struct Translation_Data *translation_data)
{
	struct Source_File temp_src;
	struct token *hold_token;
	char just_in_case;

	temp_src=*src;
	hold_token=preproc_find_else(src,translation_data,1);


	temp_src.src_size=src->where_in_src;
	just_in_case=src->src[src->where_in_src];
	src->src[src->where_in_src]='\0';

	lex(&temp_src,translation_data);

	src->src[src->where_in_src]=just_in_case;

	if(hold_token!=NULL)
		free(hold_token);
	do
	{
		hold_token=preproc_find_else(src,translation_data,0);
		if(hold_token)
			free(hold_token);
		else
			break;
	}while(!has_new_errors(translation_data));
	
	if(hold_token!=NULL)
	{
		free(hold_token);
		push_lexing_error("could not find matching #else, #elif or #endif",src,translation_data);
	}
}
/*
   we have skipped the #if part so this could be used for elif 
 */
void parse_preproc_if_line(struct Source_File *src,struct Translation_Data *translation_data)
{
	size_t hold_line;
	struct Queue hold_tokens;
	struct Queue *swap_tokens;
	struct token *hold_token;
	struct Scope *empty_scope;
	struct AST *expression;
	int result;

	hold_line=src->which_row;
	Queue_Init(&hold_tokens);
	hold_token=get_next_token(src,&chonky[0],0);
	empty_scope=get_normal_scope(NULL,EXTERN_SCOPE);

	swap_tokens=translation_data->tokens;
	translation_data->tokens=&hold_tokens;
	while(hold_token->type!=KW_NOTYPE && hold_token->line==hold_line)
	{
		expand_macro(hold_token,src,translation_data);
		hold_token=get_next_token(src,&chonky[0],0);	
	}

	/*NOTYPE*/
	free(hold_token);
		
	expression=parse_expression(translation_data,empty_scope);
	result=evaluate_const_expression_integer(expression);

	delete_ast(expression);
	delete_scope(empty_scope);
	translation_data->tokens=swap_tokens;

	if(hold_tokens.size>0)
	{
		push_lexing_error("unexpected token",src,translation_data);
		while(hold_tokens.size>0)
			free(Queue_Pop(&hold_tokens));
	}
	
	if(has_new_errors(translation_data))
	{
		push_lexing_error("fatal error",src,translation_data);
		return ;
	}else
	{

		if(result)
		{
			preproc_lex_first_part(src,translation_data);	
		}else
		{
			hold_token=preproc_find_else(src,translation_data,0);
			if(hold_token!=NULL && hold_token->type==PKW_ELIF)
			{
				//preproc_find_else(src,translation_data,0);
				parse_preproc_if_line(src,translation_data);
			}
			else
			{
				preproc_lex_first_part(src,translation_data);
			}
		}	
		
	}
	
}
struct token* preproc_find_else(struct Source_File *src,struct Translation_Data *translation_data,char jump_before)
{
	struct token *hold_token;
	struct Source_File temp_src;
	int indentation=1;

	while(src->src[src->where_in_src]!='\0' && indentation)
	{
		/*BEWARE*/
		goto_new_line(src,translation_data);
		temp_src=*src;
		/*END BEWARE*/

		hold_token=get_next_token(src,&chonky[0],1);
		if(hold_token->type==KW_HASHTAG)
		{
			free(hold_token);
			hold_token=get_next_token(src,&chonky_jr[0],0);
			switch(hold_token->type)
			{
				case PKW_IF:
				case PKW_IFDEF:
				case PKW_IFNDEF:
					++indentation;
					break;

				case PKW_ENDIF:
					--indentation;
					break;

				case PKW_ELSE:
				case PKW_ELIF:
					if(indentation==1)
					{
						if(jump_before)
							*src=temp_src;
						return hold_token;
					}
					else
					{
						break;
					}
				case PKW_NOTYPE:
					push_lexing_error("unexpected character",src,translation_data);
					free(hold_token);
					return NULL;
			}
			free(hold_token);

		}else if(hold_token->type!=KW_NOTYPE)
		{
			free(hold_token);
		}else
		{
			if(src->where_in_src!=src->src_size)
				push_lexing_error("unexpected character",src,translation_data);
			free(hold_token);
			return NULL;
		}
	}
	/*BEWARE*/
	//goto_new_line(src,translation_data);	
	/*END BEWARE*/
	if(jump_before)
		*src=temp_src;
	return NULL;
}
void parse_preproc_ifdef_line(struct Source_File *src,struct Translation_Data *translation_data)
{
	struct token *hold_token;
	hold_token=get_next_token(src,&chonky[0],0);
	if(hold_token==NULL || hold_token->type!=KW_ID)
	{
		free(hold_token);
		push_lexing_error("expected an id here",src,translation_data);
		chase_new_line(src,translation_data);
		return;
	}else
	{
		if(Map_Check(translation_data->macros,hold_token->data,hold_token->data_size))
		{
			preproc_lex_first_part(src,translation_data);
		}else
		{
			free(hold_token);

			hold_token=preproc_find_else(src,translation_data,1);

			if(hold_token!=NULL && hold_token->type==PKW_ELIF)
			{
				parse_preproc_if_line(src,translation_data);
			}
			else
			{
				preproc_find_else(src,translation_data,0);
				preproc_lex_first_part(src,translation_data);
			}

			free(hold_token);
		}

	}
	chase_new_line(src,translation_data);
}
void parse_preproc_ifndef_line(struct Source_File *src,struct Translation_Data *translation_data)
{
	struct token *hold_token;
	hold_token=get_next_token(src,&chonky[0],0);
	if(hold_token==NULL || hold_token->type!=KW_ID)
	{
		push_lexing_error("expected an id here",src,translation_data);
		chase_new_line(src,translation_data);
		free(hold_token);
		return;
	}else
	{
		if(!Map_Check(translation_data->macros,hold_token->data,hold_token->data_size))
		{
			free(hold_token);
			preproc_lex_first_part(src,translation_data);
		}else
		{
			free(hold_token);

			hold_token=preproc_find_else(src,translation_data,1);
			if(hold_token!=NULL && hold_token->type==PKW_ELIF)
			{
				parse_preproc_if_line(src,translation_data);
			}
			else
			{
				preproc_find_else(src,translation_data,0);
				preproc_lex_first_part(src,translation_data);
			}

			free(hold_token);
		}

	}
	chase_new_line(src,translation_data);
}
void parse_preproc_undef_line(struct Source_File *src,struct Translation_Data *translation_data)
{
	struct define_directive *hold_macro;
	struct token *id;
	
	id=get_next_token(src,&chonky[0],0);
	if(id->type!=KW_ID)
	{
		push_lexing_error("expected an id here",src,translation_data);
	}else
	{
		hold_macro=Map_Check(translation_data->macros,id->data,id->data_size);
		if(hold_macro!=NULL)
		{
			delete_macro(hold_macro);
			Map_Remove(translation_data->macros,id->data,id->data_size);
		}
	}
	free(id);
	chase_new_line(src,translation_data);
}
void delete_macro(void *macro)
{
#define AS_MACRO(x) ((struct define_directive*)macro)
	free(AS_MACRO(macro)->macro_name);
	while(AS_MACRO(macro)->macro_tokens->size>0)
		free(Queue_Pop(AS_MACRO(macro)->macro_tokens));
	free(AS_MACRO(macro)->macro_tokens);
	Map_Map(AS_MACRO(macro)->arguments,free);
	free(AS_MACRO(macro)->arguments);
	free(macro);
#undef AS_MACRO
}
#endif
