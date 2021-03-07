#ifndef WONKY_PROGRAM_H
#define WONKY_PROGRAM_H WONKY_PROGRAM_H


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

	/*
	   we the type node structures from 
	   all the translation units are stored here
	 */
	struct Map *types;

	struct Linkage *external_linkage;
};
struct Translation_Data
{
	struct Queue *tokens;


	struct Queue *errors;
	size_t number_of_errors_when_last_checked;


	struct Queue *source_files;

	struct Map *macros;

	/*passed from program struct*/
	struct Map *types;
	struct Linkage *external_linkage;
	struct Linkage *internal_linkage;
	/*end of passed from program struct*/
};

struct Program* get_program();
struct Source_Name* get_source_name(char *filename,char *base);

struct Source_File* extract_source_file(FILE *in,struct Source_Name *name);
struct Source_File* get_source_file(char *filename,char **where_to_search);
void normalise_source_name(struct Source_Name *name);

struct Translation_Data* get_translation_data(struct Map *types,struct Linkage *internal_linkage,struct Linkage *external_linkage);


struct Program* parse_program(char **base_source_names);
void lex_program(struct Translation_Data *hold,struct Source_File *file);
void entype_program(struct Program *program);


char has_new_errors(struct Translation_Data *translation_data);
char has_no_tokens(struct Translation_Data *translation_data);

void delete_program(struct Program *program);
void delete_translation_data(struct Translation_Data *translation_data);
void destroy_translation_data(struct Translation_Data *translation_data);
void assimilate_translation_data(struct Program *program,struct Translation_Data *translation_data);

#endif
