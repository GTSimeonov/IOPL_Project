#ifndef GCC_PARSE_TRANSLATION_C
#define GCC_PARSE_TRANSLATION_C GCC_PARSE_TRANSLATION_C
#include <parse_translation_unit.h>
#include <gcc_error.h>
/*
	translation-unit:
		declaration [ translation-unit ]
		function-definition [ translation-unit ]
*/
struct AST* parse_translation_unit(struct Translation_Data *translation_data)
{
	size_t loop_preventer;
	struct AST_Translation_Unit *hold;
	hold=get_translation_unit_tree();

	loop_preventer=0;
	while(translation_data->tokens->size>0 && loop_preventer!=translation_data->tokens->size)
	{
		loop_preventer=translation_data->tokens->size;

		if(is_type(translation_data,hold->file_scope) || kw_get(translation_data)==KW_ID)
		{
			parse_declaration(translation_data,hold->file_scope,&hold->components,1);
			if(has_new_errors(translation_data))
			{
				chase_next_semicolumn(translation_data);
			}
		}else
		{
			push_translation_error("declaration expected",translation_data);
			chase_next_semicolumn(translation_data);
		}
	}
	return (struct AST*)hold;

}

#endif
