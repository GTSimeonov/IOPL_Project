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
};

void parse_preproc_line(struct Source_File *src,struct Translation_Data *translation_data);
void parse_include_line(struct Source_File *src,struct Translation_Data *translation_data);
void parse_define_line(struct Source_File *src,struct Translation_Data *translation_data);
void parse_preproc_if_line(struct Source_File *src,struct Translation_Data *translation_data);



/*preproc if stuff*/
/*
   TODO 
   TODO 
   TODO 
   TODO 
char proproc_is_if(struct Source_File *src,struct Translation_Data *translation_data); 
char proproc_is_else(struct Source_File *src,struct Translation_Data *translation_data); 
void preproc_find_matching_else(struct Source_File *src,struct Translation_Data *translation_data);
void preproc_find_endif(struct Source_File *src,struct Translation_Data *translation_data);
   TODO 
   TODO 
   TODO 
   TODO 
*/

/*define stuff*/
void expand_macro(struct token* macro_name,struct Source_File *src,struct Translation_Data *translation_data);



struct define_directive* get_define_directive(struct token* macro_name);
struct Queue* make_define_argument_list(size_t number_of_arguments);

void expand_macro_argument(struct Queue *replacement_tokens,struct Source_File *src,struct Translation_Data *translation_data);
void load_macro_arguments(struct Queue  *args,size_t number_of_arguments,struct Source_File *src,struct Translation_Data *translation_data);
void flush_macro_arguments(size_t number_of_arguments,struct Queue *args);
void parse_define_line(struct Source_File *src,struct Translation_Data *translation_data);

void delete_define_argument_list(size_t number_of_arguments,struct Queue *args);

#endif
