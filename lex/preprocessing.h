#ifndef GCC_PREPROCESSING_H
#define GCC_PREPROCESSING_H GCC_PREPROCESSING_H
#include <preprocessing.hh>
#include <program.h>
#include <lexer.h>
#include <chonky.h>
#include <gcc_error.h>
#include <map.h>

struct define_directive
{
	struct token *macro_name;

	/*the tokens of the macro*/
	struct Queue *macro_tokens;

	/*ints are stored here*/
	struct Map *arguments;

	/*put arguments here*/
	/*an array of token queues*/
	size_t number_of_arguments;
	struct Queue *argument_list;
};

void parse_preproc_line(struct Source_File *src,struct Translation_Data *translation_data);
void parse_include_line(struct Source_File *src,struct Translation_Data *translation_data);
void parse_define_line(struct Source_File *src,struct Translation_Data *translation_data);



/*define stuff*/
struct define_directive* get_define_directive(struct token* macro_name);
void make_define_argument_list(struct define_directive* directive,size_t number_of_arguments);

void expand_macro_argument(struct Queue *replacement_tokens,struct Translation_Data *translation_data);
void load_macro_arguments(struct define_directive *macro,struct Source_File *src,struct Translation_Data *translation_data);
void flush_macro_arguments(struct define_directive *macro);
void expand_macro(struct token* macro_name,struct Source_File *src,struct Translation_Data *translation_data);
void parse_define_line(struct Source_File *src,struct Translation_Data *translation_data);


#endif
