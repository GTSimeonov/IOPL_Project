#ifndef GCC_PREPROCESSING_H
#define GCC_PREPROCESSING_H GCC_PREPROCESSING_H
#include <preprocessing.hh>
#include <program.h>
#include <lexer.h>
#include <gcc_error.h>

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

void parse_preproc_line(struct Source_File *src,struct Translation_Data *translation_data);
void parse_include_line(struct Source_File *src,struct Translation_Data *translation_data);
void parse_define_line(struct Source_File *src,struct Translation_Data *translation_data);
struct define_directive* get_define_directive(struct token* macro_name);


#endif
