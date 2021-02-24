#ifndef GCC_ARGUMENTS_C
#define GCC_ARGUMENTS_C GCC_ARGUMENTS_C
#include<gcc_arguments.h>



struct Command_Arguments* parse_command_arguments(char **argv)
{
	struct Command_Arguments *ret;
	struct Queue *source_names;

	ret=malloc(sizeof(struct Command_Arguments));
	ret->print_ast=ret->print_tokens=ret->transpile_to_js=0;
	ret->output_file=ret->javascript_extern_file=NULL;

	source_names=malloc(sizeof(struct Queue));
	Queue_Init(source_names);

	ret->error_message=NULL;

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
			if(ret->print_ast || ret->print_tokens)
			{
				ret->error_message="print and quiet conflict. Aborting";
				break;
			}
		}else if(gstr_cmp(*argv,"--transpile_to_js") || gstr_cmp(*argv,"-js"))
		{
			ret->transpile_to_js=1;	
		}else if(gstr_cmp(*argv,"-o"))
		{
			++argv;
			if(*argv==NULL)
			{
				ret->error_message="expected a filename after -o. Aborting";
				break;
			}else
			{
				ret->output_file=fopen(*argv,"w");
				if(ret->output_file==NULL)
				{
					ret->error_message="Couldn't open file for output. Aborting";
					break;
				}
			}
		}else if(gstr_cmp(*argv,"--make-extern-file"))
		{
				ret->javascript_extern_file=fopen(*argv,"w");
				if(ret->javascript_extern_file==NULL)
				{
					ret->error_message="Couldn't open file for extern dump. Aborting";
					break;
				}
		}else if(gstr_cmp(*argv,"--add-html"))
		{
			ret->insert_html=1;	
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
	if(ret->transpile_to_js && ret->output_file==NULL)
	{
		ret->output_file=fopen("a.js","w");
		if(ret==NULL)
			ret->error_message="Couldn't open file for output. Aborting";
	}
	return ret;
}


#endif
