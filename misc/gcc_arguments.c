#ifndef GCC_ARGUMENTS_C
#define GCC_ARGUMENTS_C GCC_ARGUMENTS_C
#include<gcc_arguments.h>



struct Command_Arguments* parse_command_arguments(char **argv)
{
	struct Command_Arguments *ret;
	struct Queue *source_names;

	ret=malloc(sizeof(struct Command_Arguments));
	ret->print_ast=ret->print_tokens=0;

	source_names=malloc(sizeof(struct Queue));
	Queue_Init(source_names);


	/*we skip the first element(the program name)*/
	for(++argv;*argv;++argv)
	{
		if(gstr_cmp(*argv,"--print-tokens"))
		{
			ret->print_tokens=1;
		}else if(gstr_cmp(*argv,"--print-ast"))
		{
			ret->print_ast=1;
		}else if(gstr_cmp(*argv,"--quiet") || gstr_cmp(*argv,"-q"))
		{
			ret->is_quiet=1;
		}else
		{
			Queue_Push(source_names,*argv);
		}
	}

	ret->source_names=malloc((source_names->size+1)*sizeof(char*) );
	ret->source_names[source_names->size]=NULL;
	while(source_names->size)
	{
		ret->source_names[source_names->size-1]=(char*)source_names->first->data;
		Queue_Pop(source_names);
	}
	free(source_names);
	return ret;
}


#endif
