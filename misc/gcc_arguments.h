#ifndef GCC_ARGUMENTS_H
#define GCC_ARGUMENTS_H GCC_ARGUMENTS_H
#include <gcc_arguments.hh>
#include <queue.h>
#include <gcc_string.h>


struct Command_Arguments
{
	char **source_names;
	char print_tokens:1;
	char print_ast:1;
	char is_quiet:1;
};

struct Command_Arguments* parse_command_arguments(char **argv);

#endif
