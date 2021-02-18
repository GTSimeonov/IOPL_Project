#ifndef GCC_PREPROCESSING_C
#define GCC_PREPROCESSING_C GCC_PREPROCESSING_C
#include <preprocessing.h>


/*we have skipped the leading #*/
/*
 	#include string
	#include <qchar>
	#define [ id(list) replacement
	#line number [string]
	#if
	#ifdef
	#ifndef
	#pragma
	#error
	#

	these should be seperated from the ifs
	#elif
	#else
	#endif

	
*/
void parse_preproc_line(struct Source_File *src,struct Translation_Data *translation_data)
{
	struct token *hold;
	hold=get_next_token(src,&chonky_jr[0]);
	switch(hold->type)
	{
		case PKW_INCLUDE:
			free(hold);
			parse_include_line(src,translation_data);
			return;
		case PKW_DEFINE:
			free(hold);
			parse_define_line(src,translation_data);
		default:
			return;
			/*TODO error*/

	}
}
void parse_include_line(struct Source_File *src,struct Translation_Data *translation_data)
{
	struct token *hold;
	hold=get_next_token(src,&chonky[0]);
	if(hold->type==KW_STRING)
	{
		char *where_to_search[]={src->src_name->base,NULL};
		struct Source_File *hold_file;

		hold->data[hold->data_size-1]='\0';
		hold->data_size-=2;
		++hold->data;
		handle_splicing(hold);


		/*search in the directory of the file from which we include*/
		hold_file=get_source_file(hold->data,where_to_search);
		/*fallback to well known locations == <>*/
		if(hold_file==NULL)
		{
			hold_file=get_source_file(hold->data,well_known_locations_base);
			if(hold_file==NULL)
			{
				/*TODO error*/
				push_translation_error("include error",translation_data);
				return;
			}
		}
		lex_program(translation_data,hold_file);
		free(hold);
	}else if(hold->type==KW_LESS)/*hack*/
	{
		struct Source_File *hold_file;
		++hold->data;
		while(src->src[src->where_in_src]!='>' && src->where_in_src<src->src_size)
		{
			++src->where_in_src;
			++hold->data_size;
		}
		if(src->where_in_src==src->src_size)
		{
			/*TODO error*/
			return;
		}
		/*skip the >*/
		++src->where_in_src;
		hold->data[hold->data_size-1]='\0';
		handle_splicing(hold);
		
		hold_file=get_source_file(hold->data,well_known_locations_base);
		if(hold_file==NULL)
		{
			/*TODO error*/
			push_translation_error("include error",translation_data);
			return;
		}

		lex_program(translation_data,hold_file);
		free(hold);

	}else
	{
		/*TODO error*/
		push_translation_error("include error",translation_data);
		return;
	}


	chase_new_line(src,translation_data);
}
void parse_define_line(struct Source_File *src,struct Translation_Data *translation_data)
{
}
/*
	id[(list)] tokens \n 
 */
struct define_directive* get_define_directive(struct token* macro_name)
{
}

#endif
