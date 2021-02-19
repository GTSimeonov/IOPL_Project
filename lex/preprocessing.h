#ifndef GCC_PREPROCESSING_H
#define GCC_PREPROCESSING_H GCC_PREPROCESSING_H
#include <preprocessing.hh>
#include <program.h>
#include <lexer.h>
#include <gcc_error.h>

struct define_directive
{
	struct token *macro_name;

	/*the tokens of the macro*/
	struct Queue *macro_tokens;

	struct Map *arguments;

	/*put arguments here*/
	/*an array to pointers to token queues*/
	struct Queue **argument_list;
};

void parse_preproc_line(struct Source_File *src,struct Translation_Data *translation_data);
void parse_include_line(struct Source_File *src,struct Translation_Data *translation_data);
void parse_define_line(struct Source_File *src,struct Translation_Data *translation_data);



/*define stuff*/
struct define_directive* get_define_directive(struct token* macro_name);
struct token* copy_token(struct token *token);
void expand_macro_argument(struct Queue *replacement_tokens);
void expand_macro(struct token* macro_name,struct Translation_Data *translation_data);
void parse_define_line(struct Source_File *src,struct Translation_Data *translation_data);


#endif
