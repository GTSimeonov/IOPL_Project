#ifndef GCC_PROGRAM_C
#define GCC_PROGRAM_C GCC_PROGRAM_C
#/*Don't mind me*/include/*When I grow up I want to be a new line*/"program.h"
#include"parse_translation_unit.h"


struct Program* get_program()
{
	struct Program *ret;
	ret=malloc(sizeof(struct Program));
	Queue_Init(&ret->translation_units_tokens);
	Queue_Init(&ret->source_files);
	Map_Init(&ret->defines);
	ret->global_scope=get_scope(NULL);

	ret->number_of_translation_units=0;
	ret->translation_units=NULL;

	return ret;
}
struct Source_File* get_source_file(char *name_of_file)
{
	FILE *in;
	size_t file_size;
	struct Source_File *src;

	in=fopen(name_of_file,"r");
	if(in==NULL)
	{
		/*error*/
		return NULL;
	}

	fseek(in,0,SEEK_END);
	file_size=ftell(in);
	rewind(in);

	src=malloc(sizeof(struct Source_File));
	src->src_name=name_of_file;
	src->src=malloc(file_size+1);
	src->src_size=file_size;
	src->where_in_src=0;
	src->which_column=0;
	src->which_row=0;
	src->src[file_size]='\0';

	fread(src->src,1,file_size,in);
	fclose(in);
	return src;	
}
struct Queue* lex_source_file(char *name_of_file,struct Program* program)
{
	struct Source_File *src;
	src=get_source_file(name_of_file);

	Queue_Push(&program->translation_units_tokens,lex(src,program));
	Queue_Push(&program->source_files,src);
}
void lex_program(char *name_of_file,struct Program *program)
{
	lex_source_file(name_of_file,program);
}

void parse_program(struct Program *program)
{
	size_t i;
	program->translation_units=malloc(sizeof(struct	AST*) * program->translation_units_tokens.size);
	program->number_of_translation_units=program->translation_units_tokens.size;
	for(i=0;i<program->number_of_translation_units;++i)
	{
		program->translation_units[i]=parse_translation_unit(
					Queue_Pop(&program->translation_units_tokens),
					program->global_scope
				);
	}
}
#endif
