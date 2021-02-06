#ifndef LEXER_C
#define LEXER_C LEXER_C
#include "lexer.h"

struct Queue* lex(struct Source_File *src,struct Program *prog)
{


	struct token *current_token;
	struct Queue *tokens;

	tokens=malloc(sizeof(struct Queue));
	Queue_Init(tokens);
	while(src->src[src->where_in_src]!='\0')
	{

		if(src->which_column==0 && src->src[src->where_in_src]=='#')
		{
			/*todo preprocesing*/
			++src->where_in_src;
			++src->which_column;
			do_preproc_stuff(src,prog);
		}else
		{
			current_token=get_next_token(src,prog,&chonky[0]);
			if(current_token->type!=KW_NOTYPE)
				Queue_Push(tokens,current_token);
			else
				free(current_token);
		}
	}


	return tokens;

}

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
void do_preproc_stuff(struct Source_File *src,struct Program *prog)
{
	struct token *hold;
	hold=get_next_token(src,prog,&chonky_jr[0]);
	switch(hold->type)
	{
		case PKW_INCLUDE:
			free(hold);
			do_include_stuff(src,prog);
			return;
		case PKW_DEFINE:
			free(hold);
			do_define_stuff(src,prog);


		default:
			return;
			/*TODO error*/

	}
}

void do_include_stuff(struct Source_File *src,struct Program *prog)
{
	struct token *hold;
	hold=get_next_token(src,prog,&chonky[0]);
	if(hold->type==KW_STRING)
	{
		hold->data[hold->data_size-1]='\0';
		hold->data_size-=2;
		++hold->data;
		handle_splicing(hold);
		lex_program(hold->data,prog);
		free(hold);
	}else if(hold->type==KW_LESS)/*hack*/
	{
		++hold->data;
		while(src->src[src->where_in_src]!='>')
		{
			++src->where_in_src;
			++hold->data_size;
		}
		/*skip the >*/
		++src->where_in_src;
		hold->data[hold->data_size-1]='\0';
		handle_splicing(hold);
		
		lex_program(hold->data,prog);
		free(hold);

	}else
	{
		/*TODO error*/
		return;
	}
}
struct define_directive* get_define_directive(struct token* macro_name)
{
	struct define_directive* ret;
	ret=malloc(sizeof(struct define_directive));
	ret->macro_name=macro_name;

	Queue_Init(&ret->replacement_list);
	Queue_Init(&ret->id_list);
	ret->number_of_arguments=0;
	Map_Init(&ret->arguments);
	return ret;
}
/*
	id[(list)] tokens \n 
 */
void do_define_stuff(struct Source_File *src,struct Program *prog)
{
	struct token *hold;
	struct define_directive *macro;
	hold=get_next_token(src,prog,&chonky[0]);
	if(hold->type==KW_ID)
	{
		macro=get_define_directive(hold);
		Map_Push(&prog->defines,hold->data,hold->data_size,macro);
		hold=get_next_token(src,prog,&chonky[0]);
	}else
	{
		/*TODO error*/
		return;
	}
}



/*hack*/
void handle_splicing(struct token *word)
{
	size_t back;
	size_t front;
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

char check(struct Queue *tokens,enum KEYWORDS kw,size_t ahead)
{
	size_t i;
	struct Queue_Node *current;
	if(tokens->size<=ahead)
	{
		return 0;
	}else
	{
		for(i=0,current=tokens->first;i<ahead;++i,current=current->prev);

		if( ((struct token*)(current->data))->type == kw )
		{
			return 1;
		}else
		{
			return 0;
		}
	}
}
char get_and_check(struct Queue  *tokens,enum KEYWORDS kw)
{
	struct token *hold_token;
	if(tokens->size==0)
	{
		return 0;
	}else
	{
		hold_token=tokens->first->data;
		if(hold_token->type!=kw)
		{
			return 0;
		}else
		{
			hold_token=Queue_Pop(tokens);
			free(hold_token);
			return 1;
		}
	}
}
char get_and_check_unsafe(struct Queue  *tokens,enum KEYWORDS kw)
{
	struct token *hold_token;
	hold_token=tokens->first->data;
	if(hold_token->type!=kw)
	{
		return 0;
	}else
	{
		hold_token=Queue_Pop(tokens);
		free(hold_token);
		return 1;
	}
}
void chomp(struct Queue *tokens)
{
	free(Queue_Pop(tokens));
}

enum KEYWORDS kw_get(struct Queue *tokens)
{
	if(tokens->size==0)
		return KW_NOTYPE;
	return ((struct token*)(tokens->first->data))->type;

}

struct token* get_next_token(struct Source_File *src,struct Program *prog,struct automata_entry *start_state)
{
	int temp;
	size_t current_size;

	struct token *ret;
	struct automata_entry *current_state;
	struct automata_entry *best_state;

	current_size=0;
	best_state=current_state=start_state;

	/*check for double slash comment*/
	if(src->where_in_src<src->src_size-1 && src->src[src->where_in_src]=='/' && src->src[src->where_in_src+1]=='/')
	{
		ret=malloc(sizeof(struct token));
		ret->type=KW_COMMENT;
		ret->data=src->src + src->where_in_src;
		src->where_in_src+=2;
		while(src->where_in_src!=src->src_size && src->src[src->where_in_src]!='\n')
		{
			if(src->where_in_src<src->src_size-1 && src->src[src->where_in_src]=='\\' && src->src[src->where_in_src+1]=='\n')
			{
				src->where_in_src+=2;
			}else
			{
				++src->where_in_src;
			}
		}

	}
	/*ignore leading spaces and tabs and check for double slash comment*/
	while(src->src[src->where_in_src]==' ' || src->src[src->where_in_src]=='\n' || src->src[src->where_in_src]=='\t')
	{
		if(src->src[src->where_in_src]=='\n')
		{
			src->which_column=0;
			++src->which_row;
		}else if(src->src[src->where_in_src]=='\t')
		{
			src->which_row+=5;
		}
		++src->where_in_src;
	}

	while(src->src[src->where_in_src]!='\0')
	{

		if(src->where_in_src<src->src_size-1 && src->src[src->where_in_src]=='\\' && src->src[src->where_in_src+1]=='\n')
		{
			src->where_in_src+=2;
			current_size+=2;
		}

		current_state=current_state->delta[cmpr[src->src[src->where_in_src]]];
		if(current_state==NULL)
		{
			if(best_state->type==KW_COMMENT || best_state->type==PKW_COMMENT)
			{
				current_size=0;
				best_state=current_state=start_state;
			}else
			{
				ret=malloc(sizeof(struct token));
				ret->type=best_state->type;
				ret->data_size=current_size;
				ret->column=src->which_column;
				ret->line=src->which_row;
				ret->data=src->src+(src->where_in_src-current_size);
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
	ret=malloc(sizeof(struct token));
	ret->type=KW_NOTYPE;
	ret->data_size=0;

	return ret;
}
#endif
