#ifndef GCC_PROGRAM_H
#define GCC_PROGRAM_H GCC_PROGRAM_H
#include "queue.c"
#include "scope.h"
#include "lexer.h"

struct Source_File
{
	char *src;
	char *src_name;
	size_t src_size;

	size_t where_in_src;
	size_t which_column;
	size_t which_row;
};


struct Program
{

	struct Queue translation_units_tokens;
	struct Queue source_files;
	struct Map defines;

	size_t number_of_translation_units;
	struct AST **translation_units;

	struct Scope *global_scope;
};

struct Program* get_program();
struct Source_File* get_source_file(char *name_of_file);
struct Queue* lex_source_file(char *name_of_file,struct Program *program);
void lex_program(char *name_of_file,struct Program *program);
void parse_program(struct Program *program);


#endif
