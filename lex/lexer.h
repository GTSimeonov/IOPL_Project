#ifndef LEXER_H
#define LEXER_H LEXER_H
#include <lexer.hh>
#include <stdio.h>
#include <chonky.h>
#include <chonky_jr.h>
#include <queue.h>
#include <program.h>
#include <preprocessing.h>
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





struct Queue* lex(struct Source_File *src,struct Program *prog);
struct token* get_next_token(struct Source_File *src,struct Program *prog,struct automata_entry *start_state);
struct token_vector Lex_Queue_Condense(struct Queue *tokens);
char check(struct Queue *tokens,enum KEYWORDS kw,size_t ahead);
char get_and_check(struct Queue  *tokens,enum KEYWORDS kw);
char get_and_check_unsafe(struct Queue  *tokens,enum KEYWORDS kw);
void chomp(struct Queue *tokens);
enum KEYWORDS kw_get(struct Queue *tokens);


#endif
