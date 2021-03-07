#ifndef WONKY_LEXER_C
#define WONKY_LEXER_C WONKY_LEXER_C
/*asdf*/#include <lexer.h>

char *well_known_locations_base[]={"","/usr/include/","/usr/include/x86_64-linux-gnu/",NULL};
void lex(struct Source_File *src,struct Translation_Data *translation_data)
{


	struct token *current_token;

	while(src->src[src->where_in_src]!='\0')
	{
		if(has_new_errors(translation_data))
		{
			push_lexing_error("Fatal error",src,translation_data);
			return;
		}

		current_token=get_next_token(src,&chonky[0],1);
		if(current_token->type==KW_HASHTAG)
		{
			if(src->is_in_the_begining_of_line)
			{
				parse_preproc_line(src,translation_data);
				free(current_token);
			}else
			{
				push_lexing_error("preprocessing directive must be at the beggining of the line",src,translation_data);
				free(current_token);
				while((current_token=get_next_token(src,&chonky[0],0))->type!=KW_NOTYPE)
				{
					free(current_token);
				}
				free(current_token);
			}

		}else if(current_token->type!=KW_NOTYPE)
		{

			expand_macro(current_token,src,translation_data);
		}else
		{
			if(src->where_in_src!=src->src_size)
				push_lexing_error("unexpected character",src,translation_data);
			free(current_token);
			return;
		}
	}

}



/*hack*/
void handle_splicing(struct token *word)
{
	size_t back;
	size_t front;
	if(word->data_size==0)
		return;
	front=0;
	for(front;front<word->data_size-1;++front)
	{
		if(word->data[front]=='\\' && word->data[front+1]=='\n')
		{
			front+=2;
			break;
		}
	}
	if(front==word->data_size-1)
		return;

	for(back=front-2;front<word->data_size-1;)
	{
		if(word->data[front]=='\\' && word->data[front+1]=='\n')
		{
			front+=2;
		}else
		{
			word->data[back]=word->data[front];
			++front;
			++back;
		}
	}
	word->data[back]=word->data[front];
}
void goto_new_line(struct Source_File *src,struct Translation_Data *translation_data)
{
	char hold_char;
	while( (hold_char=src_getc(src,1,1,0)) != '\0' && hold_char != '\n');
	src->is_in_the_begining_of_line=1;
	++src->where_in_src;
	++src->which_row;
	src->which_column=0;
}
void chase_new_line(struct Source_File *src,struct Translation_Data *translation_data)
{
	char hold_char;
	for(hold_char=src_getc(src,1,1,0);hold_char!='\n' && hold_char!='\0';
			hold_char=src_getc(src,1,1,0));

	++src->where_in_src;
	++src->which_row;
	src->is_in_the_begining_of_line=1;
	src->which_column=0;


}
/*returns the number of bytes skipped*/
size_t skip_line_splice(struct Source_File *src)
{
	size_t current_size=0;
	while(src->where_in_src<src->src_size-1 && src->src[src->where_in_src]=='\\' && src->src[src->where_in_src+1]=='\n')
	{
		src->where_in_src+=2;
		current_size+=2;
	}
	return current_size;
}

void skip_white_space(struct Source_File *src,char skip_new_line)
{
	char hold_char;
	while(hold_char=src_getc(src,1,1,skip_new_line))
	{
		if(hold_char=='\n' && !skip_new_line)
		{
			return ;
		}
		if(hold_char!=' ' && hold_char!='\t')
		{
			src_ungetc(src);
			return ;
		}
	}
}
struct token_vector Lex_Queue_Condense(struct Queue *tokens)
{
	size_t i;
	struct token_vector ret;
	struct token *hold;
	
	ret.tokens=malloc(sizeof(struct token)*tokens->size);
	ret.size=tokens->size;

	for(i=0;tokens->size>0;++i)
	{
		hold=Queue_Pop(tokens);
		ret.tokens[i]=*hold;
		free(hold);
	}

	Queue_Destroy(tokens);

	return ret;
}

char check(struct Translation_Data *translation_data,enum KEYWORDS kw,size_t ahead)
{
	size_t i;
	struct Queue_Node *current;
	if(translation_data->tokens->size<=ahead)
	{
		return 0;
	}else
	{
		for(i=0,current=translation_data->tokens->first;i<ahead;++i,current=current->prev);

		if( ((struct token*)(current->data))->type == kw )
		{
			return 1;
		}else
		{
			return 0;
		}
	}
}
char get_and_check(struct Translation_Data *translation_data,enum KEYWORDS kw)
{
	struct token *hold_token;
	if(translation_data->tokens->size==0)
	{
		return 0;
	}else
	{
		hold_token=translation_data->tokens->first->data;
		if(hold_token->type!=kw)
		{
			return 0;
		}else
		{
			hold_token=Queue_Pop(translation_data->tokens);
			free(hold_token);
			return 1;
		}
	}
}
char get_and_check_unsafe(struct Translation_Data *translation_data,enum KEYWORDS kw)
{
	struct token *hold_token;
	hold_token=translation_data->tokens->first->data;
	if(hold_token->type!=kw)
	{
		return 0;
	}else
	{
		hold_token=Queue_Pop(translation_data->tokens);
		free(hold_token);
		return 1;
	}
}
void chomp(struct Translation_Data *translation_data)
{
	free(Queue_Pop(translation_data->tokens));
}

enum KEYWORDS kw_get(struct Translation_Data *translation_data)
{
	if(translation_data->tokens->size==0)
		return KW_NOTYPE;
	return ((struct token*)(translation_data->tokens->first->data))->type;

}

struct token* get_next_token(struct Source_File *src,struct automata_entry *start_state,char skip_new_line)
{
	int temp;
	char hold_char;

	struct token *ret;
	struct automata_entry *current_state;
	struct automata_entry *best_state;

	/*ignore leading spaces,tabs and newlines*/
	skip_white_space(src,skip_new_line);

	src_reset_token_data(src,1);

	best_state=current_state=start_state;


	while( (hold_char=src_getc(src,1,0,0)) !='\0')
	{
		if(hold_char=='\n' && !skip_new_line)
		{
			break;
		}
		current_state=current_state->delta[cmpr[hold_char]];
		if(current_state==NULL)
		{
			if(best_state->type==KW_COMMENT || best_state->type==PKW_COMMENT)
			{
				/*TODO account for new lines not counted in comment*/
				src_reset_token_data(src,0);
				best_state=current_state=start_state;
				skip_white_space(src,1);
			}else
			{
				return src_extract_token(src,best_state->type);
			}

		}else
		{
			if(current_state->is_final)
			{
				best_state=current_state;
				src_assimilate_into_best_token(src);
			}
		}
	}

	if(best_state->type==KW_COMMENT || best_state->type==PKW_COMMENT)
	{
		return src_extract_token(src,KW_NOTYPE);
	}else
	{
		return src_extract_token(src,best_state->type);
	}
	return ret;
}
/*here be dragons*/
char src_getc(struct Source_File *src,char skip_line_splice,char skip_comments,char skip_new_line)
{
superhack:
	if(src->src[src->where_in_src]=='\\' && skip_line_splice)
	{
		if(src->where_in_src < src->src_size-1 && src->src[src->where_in_src+1]=='\n')
		{
			src->where_in_src+=2;
			++src->which_row;
			src->token_size+=2;
			src->which_column=0;
			goto superhack;
		}else
		{
			++src->token_size;
			++src->which_column;
			++src->where_in_src;
			src->is_in_the_begining_of_line=0;
			return '\\';
		}
	}else
	{
		if(src->src[src->where_in_src]=='\n' && skip_new_line)
		{
			++src->which_row;
			src->which_column=0;
			src->is_in_the_begining_of_line=1;

			++src->where_in_src;
			goto superhack;
		}else if(src->src[src->where_in_src]=='/' && skip_comments)
		{
			if(src->src[src->where_in_src+1]=='*')
			{
				char hold_char;


				src->where_in_src+=2;
				hold_char=src_getc(src,1,0,1);
				while(hold_char)
				{
					if(hold_char=='*')
					{
						hold_char=src_getc(src,1,0,1);
						if(hold_char=='\0')
						{
							src->where_in_src=src->src_size;
							return '\0';
						}
						else if(hold_char=='/')
						{
							goto superhack;
						}
					}else
					{
						hold_char=src_getc(src,1,0,1);
					}
				}
				src->where_in_src=src->src_size;
				return '\0';

			}
		}else
		{
			++src->which_column;
		}
		++src->token_size;
		if(src->src[src->where_in_src]!='#' || src->is_in_the_begining_of_line!=1)
			src->is_in_the_begining_of_line=0;
		if(src->src[src->where_in_src]=='\n')
		{
			return '\n';
		}

		if(src->src[src->where_in_src]=='\0')
			return src->src[src->where_in_src];
		else
			return src->src[src->where_in_src++];
	}
}
void src_ungetc(struct Source_File *src)
{
	--src->where_in_src;
	if(src->src[src->where_in_src]=='\n')
	{
		--src->which_row;
		src->which_column=0;
	}
}
struct token* copy_token(struct token *src)
{
	struct token *cpy;
	cpy=malloc(sizeof(struct token));
	*cpy=*src;
	return cpy;
}
struct token* src_extract_token(struct Source_File *src,enum KEYWORDS kw)
{
	struct token *ret;
	ret=malloc(sizeof(struct token));
	ret->type=kw;

	ret->data_size=src->best_token_size;
	ret->column=src->best_token_column;
	ret->line=src->best_token_line;
	ret->data=src->src+src->best_token_where_in_src_start;
	ret->filename=src->src_name->filename;
	handle_splicing(ret);
	src->where_in_src=src->best_token_where_in_src_end;
	src->is_in_the_begining_of_line=src->best_token_beg_line;
	return ret;
}

void src_reset_token_data(struct Source_File *src,char use_src_as_base)
{
	src->token_size=0;
	src->best_token_size=0;
	src->best_token_line=src->which_row;
	src->best_token_column=src->which_column;
	if(use_src_as_base)
	{
		src->best_token_where_in_src_end=src->where_in_src;
	}else
	{
		src->where_in_src=src->best_token_where_in_src_end;
	}
	src->best_token_where_in_src_start=src->where_in_src;
}
void src_assimilate_into_best_token(struct Source_File *src)
{
	src->best_token_size=src->token_size;
	src->best_token_line=src->which_row;
	src->best_token_column=src->which_column;
	src->best_token_where_in_src_end=src->where_in_src;
	src->best_token_beg_line=src->is_in_the_begining_of_line;
}
void delete_source_file(struct Source_File *src)
{
	delete_source_name(src->src_name);
	free(src->src);
	free(src);
}
void delete_source_name(struct Source_Name *name)
{
	free(name->filename);
	free(name->base);
	free(name);
}
void flush_tokens(struct Queue *tokens)
{
	while(tokens->size>0)
		free(Queue_Pop(tokens));
}
#endif
