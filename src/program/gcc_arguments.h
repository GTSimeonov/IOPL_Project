#ifndef WONKY_ARGUMENTS_H
#define WONKY_ARGUMENTS_H WONKY_ARGUMENTS_H
#include <gcc_arguments.hh>
#include <queue.h>
#include <gcc_string.h>
#include <stdio.h>


struct Command_Arguments
{
	char **source_names;
	char print_tokens:1;
	char print_ast:1;
	char is_quiet:1;
	char transpile_to_js:1;
	char insert_html:1;


	FILE* output_file;
	FILE* javascript_extern_file;


	char *error_message;
};

struct Command_Arguments* parse_command_arguments(char **argv);
void delete_command_arguments(struct Command_Arguments *args);

#endif
