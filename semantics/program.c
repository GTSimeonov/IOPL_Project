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
	ret->types=malloc(sizeof(struct Map));

	Queue_Init(ret->translation_units);
	Queue_Init(ret->source_files);
	Queue_Init(ret->errors);
	/*this isn't really a scope,
	 TODO rework*/
	ret->externs=get_normal_scope(NULL,EXTERN_SCOPE);

	Map_Init(ret->types);

	return ret;
}
struct Source_File* extract_source_file(FILE *in,struct Source_Name *name)
{
	long file_size;
	struct Source_File *src;


	if(fseek(in,0,SEEK_END)==-1)
		return NULL;
	if((file_size=ftell(in))==-1)
		return NULL;
	if(fseek(in,0,SEEK_SET)==-1)
		return NULL;

	src=malloc(sizeof(struct Source_File));

	src->src_name=name;

	src->src=malloc(file_size+1);
	src->src_size=file_size;

	src->where_in_src=0;

	src->which_column=0;
	src->which_row=0;
	src->is_in_the_begining_of_line=1;

	src->src[file_size]='\0';

	fread(src->src,1,file_size,in);
	fclose(in);
	return src;	
}
struct Translation_Data* get_translation_data(struct Map *types)
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

	ret->types=types;

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
	char is_directory=0;
	struct Source_Name *name;
	struct Source_File *file;

	assert(where_to_search!=NULL);
	assert(*where_to_search!=NULL);
	do
	{
		temp_name=gstr_append(*where_to_search,filename);
		in=fopen(temp_name,"r");
		free(temp_name);
		if(in==NULL)
			continue;

		name=get_source_name(filename,*where_to_search);
		file=extract_source_file(in,name);
		if(file!=NULL)
		{
			return file;
		}else
		{
			delete_source_name(name);
		}
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
	hold_translation_data=get_translation_data(program->types);	
	do
	{
		base_file=get_source_file(*base_source_names,this_directory);
		if(base_file==NULL)
		{
			/*TODO error*/
			free(base_file);
			continue;
		}else
		{
			Queue_Push(hold_translation_data->source_files,base_file);
			lex(base_file,hold_translation_data);
			if(!has_new_errors(hold_translation_data))
			{
				Queue_Push(program->translation_units,parse_translation_unit(hold_translation_data,program->externs));
			}else
			{
				flush_tokens(hold_translation_data->tokens);
			}
			assimilate_translation_data(program,hold_translation_data);
		}
	}while(*(++base_source_names));
	
	delete_translation_data(hold_translation_data);	
	return program;
}

void lex_program(struct Translation_Data *hold,struct Source_File *file)
{
	Queue_Push(hold->source_files,file);
	lex(file,hold);
}


char has_new_errors(struct Translation_Data *translation_data)
{
	if(translation_data->errors->size != translation_data->number_of_errors_when_last_checked)
	{
		translation_data->number_of_errors_when_last_checked=translation_data->errors->size;
		return 1;
	}else
	{
		return 0;
	}
}

void delete_program(struct Program *program)
{
	while(program->translation_units->size>0)
		delete_ast(Queue_Pop(program->translation_units));
	free(program->translation_units);



	while(program->source_files->size>0)
		delete_source_file(Queue_Pop(program->source_files));
	free(program->source_files);


	while(program->errors->size>0)
		delete_translation_error(Queue_Pop(program->errors));
	free(program->errors);


	delete_scope(program->externs);

	/*BEWARE*/
	Map_Map(program->types,delete_type);
	Map_Destroy(program->types);
	free(program->types);

	free(program);

}
void delete_translation_data(struct Translation_Data *translation_data)
{
	assert(translation_data->tokens->size==0 &&
		translation_data->errors->size==0 &&
		translation_data->source_files->size==0);
	free(translation_data->tokens);
	free(translation_data->errors);
	free(translation_data->source_files);

	Map_Map(translation_data->macros,delete_macro);
	Map_Destroy(translation_data->macros);
	free(translation_data->macros);

	free(translation_data);
	
}
void assimilate_translation_data(struct Program *program,struct Translation_Data *translation_data)
{
	Queue_Append(program->errors,translation_data->errors);
	Queue_Append(program->source_files,translation_data->source_files);
	
	flush_tokens(translation_data->tokens);

	Queue_Init(translation_data->errors);
	Queue_Init(translation_data->source_files);

	translation_data->number_of_errors_when_last_checked=0;


}
#endif
