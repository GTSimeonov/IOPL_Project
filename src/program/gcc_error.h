#ifndef WONKY_ERROR_H
#define WONKY_ERROR_H WONKY_ERROR_H
#include <gcc_error.hh>
#include <lexer.h>
#include <stdio.h>



struct Translation_Error
{
	const char *error_message;
	size_t line,column;
	const char *filename;

};



struct Translation_Error* get_translation_error(const char *error_message,size_t line,size_t column,const char *filename);
struct Translation_Error* get_translation_error_by_token(const char *error_message,struct token *error_token);
void push_translation_error(const char *error_message,struct Translation_Data *translation_data);
void push_lexing_error(const char *error_message,struct Source_File *src,struct Translation_Data *translation_data);
void print_translation_error(FILE *out,struct Translation_Error *error);

void delete_translation_error(struct Translation_Error *translation_error);
#endif
