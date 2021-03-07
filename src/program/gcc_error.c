#ifndef WONKY_ERROR_C
#define WONKY_ERROR_C WONKY_ERROR_C
#include <gcc_error.h>



struct Translation_Error* get_translation_error(const char *error_message,size_t line,size_t column,const char *filename)
{
	struct Translation_Error *ret;
	ret=malloc(sizeof(struct Translation_Error));
	ret->error_message=error_message;
	ret->column=column;
	ret->line=line;
	ret->filename=filename;
}

struct Translation_Error* get_translation_error_by_token(const char *error_message,struct token *error_token)
{
	if(error_token==NULL)
	{
		return get_translation_error(error_message,0,0,"");
	}else
	{
		return get_translation_error(error_message,error_token->line+1,error_token->column+1,error_token->filename);
	}
}

void push_translation_error(const char *error_message,struct Translation_Data *translation_data)
{
	if(translation_data->tokens->size==0)
	{
		get_translation_error(error_message,0,0,"");
	}else
	{
		Queue_Push(translation_data->errors,get_translation_error_by_token(error_message,(struct token*)translation_data->tokens->first->data));
	}
}

void push_lexing_error(const char *error_message,struct Source_File *src,struct Translation_Data *translation_data)
{
	Queue_Push(translation_data->errors,get_translation_error(error_message,src->which_row+1,src->which_column+1,src->src_name->filename));
}
void print_translation_error(FILE *out,struct Translation_Error *error)
{
	fprintf(out,"Error ");
	if(error->filename!=NULL)
	{
		fprintf(out,"(line %i column %i) ",error->line,error->column);
		fprintf(out,"in %s ",error->filename);
	}
	fprintf(out,": %s\n",error->error_message);
}


void delete_translation_error(struct Translation_Error *translation_error)
{
	free(translation_error);
}
#endif
