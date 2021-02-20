#ifndef LEXER_H
#define LEXER_H LEXER_H
#include <lexer.hh>
#include <stdio.h>
#include <chonky.h>
#include <chonky_jr.h>
#include <queue.h>
#include <program.h>
#include <preprocessing.h>

extern char *well_known_locations_base[];
struct token
{
	enum KEYWORDS type;
	size_t data_size;
	char *data;
	size_t line,column;
	/*:X*/
	const char *filename;
};

struct token_vector
{
	struct token *tokens;
	size_t size;
};



struct Source_Name
{
	char *filename;
	char *base;
};

struct Source_File
{
	struct Source_Name *src_name;

	char *src;
	size_t src_size;
	size_t where_in_src;
	size_t which_column;
	size_t which_row;
};




void lex(struct Source_File *src,struct Translation_Data *translation_data);
struct token* get_next_token(struct Source_File *src,struct automata_entry *start_state,char skip_new_line);
struct token* copy_token(struct token *src);
struct token_vector Lex_Queue_Condense(struct Queue *tokens);
void handle_splicing(struct token *word);
void chase_new_line(struct Source_File *src,struct Translation_Data *translation_data);
void goto_new_line(struct Source_File *src,struct Translation_Data *translation_data);
void skip_white_space(struct Source_File *src,char skip_new_line);
size_t skip_line_splice(struct Source_File *src);


char check(struct Translation_Data *translation_data,enum KEYWORDS kw,size_t ahead);
char get_and_check(struct Translation_Data *translation_data,enum KEYWORDS kw);
char get_and_check_unsafe(struct Translation_Data *translation_data,enum KEYWORDS kw);
void chomp(struct Translation_Data *translation_data);
enum KEYWORDS kw_get(struct Translation_Data *translation_data);

#endif
