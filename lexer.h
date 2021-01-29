#ifndef LEXER_H
#define LEXER_H LEXER_H
#include<stdio.h>
#include"chonky.c"
#include"chonky_jr.c"
#include"queue.c"
#include "program.h"

struct token
{
	enum KEYWORDS type;
	size_t data_size;
	char *data;
	size_t line,column;
};

struct token_vector
{
	struct token *tokens;
	size_t size;
};

struct define_directive
{
	struct token *macro_name;
	struct Queue replacement_list;
	/*the tokens of the macro (contains a special token)*/
	struct Queue id_list;

	struct Map arguments;
	size_t number_of_arguments;
	/*put arguments here*/
	struct token **argument_list;
};

struct Queue* lex(struct Source_File *src,struct Program *prog);
struct token* get_next_token(struct Source_File *src,struct Program *prog,struct automata_entry *start_state);
struct token_vector Lex_Queue_Condense(struct Queue *tokens);
char check(struct Queue *tokens,enum KEYWORDS kw,size_t ahead);
char get_and_check(struct Queue  *tokens,enum KEYWORDS kw);
char get_and_check_unsafe(struct Queue  *tokens,enum KEYWORDS kw);
void chomp(struct Queue *tokens);
enum KEYWORDS kw_get(struct Queue *tokens);


/*I know, i know*/
void do_preproc_stuff(struct Source_File *src,struct Program *prog);
void do_include_stuff(struct Source_File *src,struct Program *prog);
void do_define_stuff(struct Source_File *src,struct Program *prog);
void handle_splicing(struct token *word);
struct define_directive* get_define_directive(struct token* macro_name);
#endif
