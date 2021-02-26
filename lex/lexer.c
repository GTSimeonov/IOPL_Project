#ifndef LEXER_C
#define LEXER_C LEXER_C
/*asdf*/#include <lexer.h>

char *well_known_locations_base[]={"./","/usr/include/","/usr/include/x86_64-linux-gnu/",NULL};
void lex(struct Source_File *src,struct Translation_Data *translation_data)
{


	struct token *current_token;
	/*this is a hack*/
	ssize_t last_line=-1;

	while(src->src[src->where_in_src]!='\0')
	{
		if(has_new_errors(translation_data))
		{
			push_lexing_error("could not process",src,translation_data);
			return;
		}

		current_token=get_next_token(src,&chonky[0],1);
		if(current_token->type==KW_HASHTAG)
		{
			if(last_line!=current_token->line)
			{
				parse_preproc_line(src,translation_data);
				last_line=current_token->line;
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
				last_line+=2;
			}

		}else if(current_token->type!=KW_NOTYPE)
		{
			last_line=current_token->line;

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
	while(src->src[src->where_in_src]!='\n' && src->src[src->where_in_src]!='\0')
	{
		skip_line_splice(src);
		++src->which_column;
		++src->where_in_src;
	}
	src->which_column=0;
	++src->which_row;
}
void chase_new_line(struct Source_File *src,struct Translation_Data *translation_data)
{
	while(src->src[src->where_in_src]!='\n' && src->src[src->where_in_src]!='\0')
	{
		skip_line_splice(src);
		if(src->src[src->where_in_src]!=' ' && src->src[src->where_in_src]!='\t')
		{
			push_lexing_error("expected a new line",src,translation_data);
			break;
		}else
		{
			++src->which_column;
		}
		++src->where_in_src;
	}
	src->which_column=0;
	++src->which_row;
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
	while(src->src[src->where_in_src]==' ' || (src->src[src->where_in_src]=='\n' && skip_new_line) || src->src[src->where_in_src]=='\t')
	{
		if(src->src[src->where_in_src]=='\n')
		{
			src->which_column=0;
			++src->which_row;
		}else
		{
			++src->which_column;
		}
		++src->where_in_src;
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
	size_t current_size;
	char hold_char;

	struct token *ret;
	struct automata_entry *current_state;
	struct automata_entry *best_state;

	current_size=0;
	best_state=current_state=start_state;

	/*ignore leading spaces,tabs and newlines*/
	skip_white_space(src,skip_new_line);

	while(src->src[src->where_in_src]!='\0')
	{
		current_size+=skip_line_splice(src);

		current_state=current_state->delta[cmpr[src->src[src->where_in_src]]];
		if(current_state==NULL)
		{
			if(best_state->type==KW_COMMENT || best_state->type==PKW_COMMENT)
			{
				/*TODO account for new lines not counted in comment*/
				current_size=0;
				best_state=current_state=start_state;
				skip_white_space(src,1);
			}else
			{
				ret=malloc(sizeof(struct token));
				ret->type=best_state->type;
				ret->data_size=current_size;
				ret->column=src->which_column;
				ret->line=src->which_row;
				ret->data=src->src+(src->where_in_src-current_size);
				ret->filename=src->src_name->filename;
				handle_splicing(ret);
				return ret;
			}

		}else
		{
			if(current_state->is_final)
			{
				best_state=current_state;
			}
			++current_size;
			++src->where_in_src;
			++src->which_column;
		}
	}

	if(best_state->type==KW_COMMENT || best_state->type==PKW_COMMENT)
	{
		ret=malloc(sizeof(struct token));
		ret->type=KW_NOTYPE;
		ret->data_size=0;
		ret->filename=src->src_name->filename;
	}else
	{
		ret=malloc(sizeof(struct token));
		ret->type=best_state->type;
		ret->data_size=current_size;
		ret->column=src->which_column;
		ret->line=src->which_row;
		ret->data=src->src+(src->where_in_src-current_size);
		ret->filename=src->src_name->filename;
		handle_splicing(ret);
		return ret;
	}
	return ret;
}
char src_getc(struct Source_File *src,char skip_line_splice)
{
superhack:
	if(src->src[src->where_in_src]=='\\' && skip_line_splice)
	{
		if(src->where_in_src < src->src_size-1 && src->src[src->where_in_src+1]=='\n')
		{
			++src->where_in_src;
			++src->which_row;
			src->which_column=0;
			goto superhack;
		}else
		{
			return '\\';
		}
	}else
	{
		if(src->src[src->where_in_src]=='\n')
		{
			++src->which_row;
			src->which_column=0;
			goto superhack;
		}else
		{
			++src->which_column;
		}
		return src->src[src->where_in_src++];
	}
}
struct token* copy_token(struct token *src)
{
	struct token *cpy;
	cpy=malloc(sizeof(struct token));
	*cpy=*src;
	return cpy;
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
#endif
