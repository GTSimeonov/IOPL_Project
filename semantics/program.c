#ifndef GCC_PROGRAM_C
#define GCC_PROGRAM_C GCC_PROGRAM_C
#/*Don't mind me*/include/*When I grow up I want to be a new line*/ <program.h>


struct Program* get_program()
{
	struct Program *ret;
	ret=malloc(sizeof(struct Program));
	ret->translation_units=malloc(sizeof(struct Queue));
	ret->source_files=malloc(sizeof(struct Queue));
	ret->errors=malloc(sizeof(struct Queue));

	Queue_Init(ret->translation_units);
	Queue_Init(ret->source_files);
	Queue_Init(ret->errors);
	ret->externs=get_scope(NULL);

	return ret;
}
struct Source_File* extract_source_file(FILE *in,struct Source_Name *name)
{
	size_t file_size;
	struct Source_File *src;


	fseek(in,0,SEEK_END);
	file_size=ftell(in);
	rewind(in);

	src=malloc(sizeof(struct Source_File));

	src->src_name=name;

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
struct Translation_Data* get_translation_data()
{
	struct Translation_Data *ret;
	ret=malloc(sizeof(struct Translation_Data));
	ret->tokens=malloc(sizeof(struct Queue));
	ret->errors=malloc(sizeof(struct Queue));
	ret->source_files=malloc(sizeof(struct Queue));
	Queue_Init(ret->tokens);
	Queue_Init(ret->errors);
	Queue_Init(ret->source_files);

	ret->macros=malloc(sizeof(struct Map));
	Map_Init(ret->macros);

	ret->number_of_errors_when_last_checked=0;

	return ret;
}
struct Source_Name* get_source_name(char *filename,char *base)
{
	struct Source_Name *ret;
	ret=malloc(sizeof(struct Source_Name));
	ret->base=gstrcpy(base);
	ret->filename=gstrcpy(filename);
	normalise_source_name(ret);
	return ret;
}
/*where_to_search ends in a NULL pointer*/
struct Source_File* get_source_file(char *filename,char **where_to_search)
{
	FILE *in;
	char *temp_name;
	struct Source_Name *name;
	assert(where_to_search!=NULL);
	assert(*where_to_search!=NULL);
	do
	{
		temp_name=gstr_append(*where_to_search,filename);
		in=fopen(temp_name,"r+");
		if(in!=NULL)
		{
			free(temp_name);
			name=get_source_name(filename,*where_to_search);
			return extract_source_file(in,name);
		}
		free(temp_name);
	}while(*(++where_to_search));
	return NULL;
}

/*this might cause compatability issues TODO*/
void normalise_source_name(struct Source_Name *name)
{
	size_t offset;
	size_t i;
	size_t last_slash;
	char *hold_base;

	for(last_slash=offset=0;name->filename[offset];++offset)
	{
		if(name->filename[offset]=='/')
		{
			last_slash=offset;
		}
	}

	if(last_slash==0)
		return;

	if(name->base==NULL)
	{
		offset=0;
		name->base=malloc(last_slash+1);
		name->base[last_slash]='\0';
		name->base[last_slash-1]='/';
		
	}else
	{
		offset=gstrlen((char*)name->base);
		hold_base=malloc(offset+last_slash+2);
		strmv(hold_base,(char*)name->base);

		hold_base[last_slash+offset]='/';
		hold_base[last_slash+offset+1]='\0';
		free((void*)name->base);

		name->base=hold_base;
	}

	for(i=0;i<last_slash;++i)
		name->base[offset+i]=name->filename[i];


	++i;
	/*prune the filename*/
	offset=gstrlen(name->filename+i);
	hold_base=malloc(offset+1);
	strmv(hold_base,name->filename+i);
	free(name->filename);
	name->filename=hold_base;


}

struct Program* parse_program(char **base_source_names)
{
	struct Source_File *base_file;
	struct Program *program;
	struct Translation_Data *hold_translation_data;
	char *this_directory[]={"./",NULL};

	assert(base_source_names!=NULL);

	if(*base_source_names==NULL)
	{
		return NULL;
	}

	program=get_program();
	hold_translation_data=get_translation_data();	
	do
	{
		base_file=get_source_file(*base_source_names,this_directory);
		if(base_file==NULL)
		{
			/*TODO error*/
			continue;
		}else
		{
			Queue_Push(hold_translation_data->source_files,base_file);
			lex(base_file,hold_translation_data);
			Queue_Push(program->translation_units,parse_translation_unit(hold_translation_data,program->externs));

			while(hold_translation_data->tokens->size!=0)
			{
				free(Queue_Pop(hold_translation_data->tokens));
			}

			Queue_Append(program->errors,hold_translation_data->errors);
			Queue_Init(hold_translation_data->errors);
		}
	}while(*(++base_source_names));
	
	Queue_Append(program->source_files,hold_translation_data->source_files);
	
	free(hold_translation_data->errors);
	free(hold_translation_data->tokens);
	free(hold_translation_data->source_files);
	free(hold_translation_data);

	return program;
}

void lex_program(struct Translation_Data *hold,struct Source_File *file)
{
	Queue_Push(hold->source_files,file);
	lex(file,hold);
}


char has_new_errors(struct Translation_Data *translation_data)
{
	if(translation_data->errors->size<translation_data->number_of_errors_when_last_checked)
	{
		translation_data->number_of_errors_when_last_checked=translation_data->errors->size;
		return 1;
	}else
	{
		return 0;
	}
}
#endif
