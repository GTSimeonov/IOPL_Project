#include<stdio.h>
#include<stdlib.h>

#include <wonky.h>




int id(int a)
{
	return a;
}
int (*test(int (*a)(int)))(int)
{
	return a;
}

int main(int argc,char **argv)
{
	/*
	int (*k)(int a,char** b)=main;
	FILE *in;
	char *src;
	struct Queue tokens;
	struct AST* test_expression;
	struct AST* test_declaration;
	struct AST* test_translation_unit;
	struct Type* test_typename;
	struct Scope *global_scope;
	const const const volatile typedef const int const volatile * const index;
	int typedef kek;

	//printf("%i",test(id)(1));

	in=fopen(argv[1],"r");
	if(in==NULL)
	{
		return -1;
	}
	src=get_src(in);

	tokens=lex(src);
//	print_tokens(stdout,&tokens);
//	printf("\n---------------------------\n");

	global_scope=get_scope(NULL);
	test_translation_unit=parse_translation_unit(&tokens,global_scope);
	print_ast(stdout,(struct AST*)test_translation_unit);
//	test_typename=parse_type_name(&tokens);
//	test_expression=parse_statement(&tokens);
//	print_expression(test_expression,stdout,0);

	*/
	struct Command_Arguments *command_arguments;
	struct Program *program;

	command_arguments=parse_command_arguments(argv);
	if(command_arguments->error_message!=NULL)
	{
		fprintf(stderr,command_arguments->error_message);
		return 1;
	}
	if(command_arguments->print_tokens && !command_arguments->is_quiet)
	{
		return print_tokens_of_program(stdout,command_arguments->source_names);
	}else
	{
		program=parse_program(command_arguments->source_names);
		if(program==NULL)
			return 0;
		if(program->errors->size>0)
		{
			if(!command_arguments->is_quiet)
			{
				print_errors(stdout,program->errors);
			}
			delete_command_arguments(command_arguments);
			delete_program(program);
			return 1;
		}else if(command_arguments->print_ast && !command_arguments->is_quiet)
		{
			print_program_ast(stdout,program);
		}else if(command_arguments->transpile_to_js)
		{
			transpile_to_javascript(command_arguments->output_file,program,command_arguments);
		}
	}
	

	/*
	program=get_program();
	lex_program(argv[1],program);
	print_program_tokens(stdout,program);
	parse_program(program);
	print_program_ast(stdout,program);
	*/

	

	delete_command_arguments(command_arguments);
	delete_program(program);
	return 0;
}



/*

	int const index=111+222;
	kek test;
	if(1)
	{
		while(2)
		{
			(kek)-1+2;
		}
		1?a,b:1?2:3*1;
	}else
	{
		do
		{
			{
				--a;
			}
		}while(a>b?1:0);
		for(;;);
		switch(a)
		{
			case b:
				return 0;
				break;
			default:
				return 1;
				return a=b=c=a->b->c->d;
		}
		goto error;
	}
   */
