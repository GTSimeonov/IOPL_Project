#ifndef GCC_PREPROCESSING_C
#define GCC_PREPROCESSING_C GCC_PREPROCESSING_C
#include <preprocessing.h>


/*we have skipped the leading #*/
/*
 	#include string
	#include <qchar>
	#define [ id(list) replacement
	#line number [string]
	#if
	#ifdef
	#ifndef
	#pragma
	#error
	#

	these should be seperated from the ifs
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
		default:
			return;
			/*TODO error*/

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
				push_translation_error("include error",translation_data);
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
			push_translation_error("include error",translation_data);
			return;
		}

		lex_program(translation_data,hold_file);
		free(hold);

	}else
	{
		/*TODO error*/
		push_translation_error("include error",translation_data);
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
			++number_of_arguments;
		}	
		hold_token=get_next_token(src,&chonky[0],0);
		make_define_argument_list(new_macro,number_of_arguments+1);

	}else if(hold_token->type==KW_NOTYPE)
	{
		push_lexing_error("empty define directive",src,translation_data);
		free(hold_token);
		/*TODO destroy new define directive*/
		/*TODO there is a memory leak here*/
		return ;	
	}

/*push things*/

	hold_tokens=translation_data->tokens;
	translation_data->tokens=new_macro->macro_tokens;
/*there is something in hold_token*/
	do{
		expand_macro(hold_token,src,translation_data);
	}while( (hold_token=get_next_token(src,&chonky[0],0))->type != KW_NOTYPE);

/*removing the notype token*/
	free(hold_token);

	translation_data->tokens=hold_tokens;
/*push the directive into the macro map*/
	Map_Push(translation_data->macros,macro_name->data,macro_name->data_size,new_macro);
	free(macro_name);

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
	ret->argument_list=NULL;	

	return ret;
}

void make_define_argument_list(struct define_directive* directive,size_t number_of_arguments)
{
	size_t i;
	assert(directive->number_of_arguments==0 && directive->argument_list==NULL);

	directive->number_of_arguments=number_of_arguments;
	directive->argument_list=malloc(sizeof(struct Queue)*number_of_arguments);

	for(i=0;i<number_of_arguments;++i)
	{
		Queue_Init(directive->argument_list+i);
	}
}

void expand_macro_argument(struct Queue *replacement_tokens,struct Translation_Data *translation_data)
{
	struct Queue_Node *it;
	for(it=replacement_tokens->first;it!=NULL;it=it->prev)
	{
		Queue_Push(translation_data->tokens,copy_token((struct token*)it->data));
	}
}
void load_macro_arguments(struct define_directive *macro,struct Source_File *src,struct Translation_Data *translation_data)
{
	struct token *hold;
	struct Queue *hack;
	size_t i;

	if(macro->number_of_arguments==0)
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
	for(i=0;i<macro->number_of_arguments-1;++i)
	{
		translation_data->tokens=macro->argument_list+i;
		for(
		hold=get_next_token(src,&chonky[0],1);
		hold->type!=KW_COMMA && hold->type!=KW_NOTYPE;
		hold=get_next_token(src,&chonky[0],1)
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
	
	}
	translation_data->tokens=macro->argument_list+i;
	for(
	hold=get_next_token(src,&chonky[0],1);
	hold->type!=KW_CLOSE_NORMAL;
	hold=get_next_token(src,&chonky[0],1)
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
	
cleanup:
	translation_data->tokens=hack;
	

}
void flush_macro_arguments(struct define_directive *macro)
{
	size_t i;
	for(i=0;i<macro->number_of_arguments;++i)
	{
		while(macro->argument_list[i].size>0)
			free(Queue_Pop(macro->argument_list+i));
	}	
}
/*macro name token is freed on expansion , if it is not a macro name it is pushed into token queue*/
void expand_macro(struct token* macro_name,struct Source_File *src,struct Translation_Data *translation_data)
{
	struct define_directive *hold=NULL;
	struct token *hold_token;
	int *index;
	struct Queue_Node *it;

	if(macro_name->type==KW_ID)
		hold=Map_Check(translation_data->macros,macro_name->data,macro_name->data_size);
	if(hold!=NULL)
	{
		free(macro_name);
		load_macro_arguments(hold,src,translation_data);
		if(translation_data->errors->size>0)
			return;

		
		for(it=hold->macro_tokens->first;it!=NULL;it=it->prev)
		{
			hold_token=(struct token*)it->data;
			index=Map_Check(hold->arguments,hold_token->data,hold_token->data_size);
			if(index!=NULL)
			{
				expand_macro_argument(hold->argument_list+*index,translation_data);
			}else
			{
				Queue_Push(translation_data->tokens,copy_token(hold_token));
			}
		}
		flush_macro_arguments(hold);
	}else
	{
		/*this isn't a macro, so we just push it to the token queue*/
		Queue_Push(translation_data->tokens,macro_name);
	}
}
#endif
