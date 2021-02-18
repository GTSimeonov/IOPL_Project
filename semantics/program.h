#ifndef GCC_PROGRAM_H
#define GCC_PROGRAM_H GCC_PROGRAM_H


#include <program.hh>
#include <queue.h>
#include <scope.h>
#include <lexer.h>
#include <gcc_string.h>
#include <gcc_arguments.h>
#include <parse_translation_unit.h>


struct Program
{
	struct Queue *translation_units;
	struct Queue *source_files;
	struct Queue *errors;
	struct Scope *externs;
};
struct Translation_Data
{
	struct Queue *tokens;
	struct Queue *errors;
	struct Queue *source_files;
};

struct Program* get_program();
struct Source_Name* get_source_name(char *filename,char *base);

struct Source_File* extract_source_file(FILE *in,struct Source_Name *name);
struct Source_File* get_source_file(char *filename,char **where_to_search);
void normalise_source_name(struct Source_Name *name);

struct Translation_Data* get_translation_data();


struct Program* parse_program(char **base_source_names);
void lex_program(struct Translation_Data *hold,struct Source_File *file);



#endif
