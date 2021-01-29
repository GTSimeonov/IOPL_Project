#include<stdio.h>
#include<stdlib.h>
#include "all.h"




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
	if(argv[1]==NULL)
	{
		printf("Give me a file!\n");
		return -1;
	}
	struct Program *program;
	program=get_program();
	lex_program(argv[1],program);
	print_program_tokens(stdout,program);
	parse_program(program);
	print_program_ast(stdout,program);

	

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
