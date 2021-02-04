#ifndef GCC_PARSE_TRANSLATION_C
#define GCC_PARSE_TRANSLATION_C GCC_PARSE_TRANSLATION_C
#include "parse_declaration.h"
#include "parse_statement.h"
/*
	translation-unit:
		declaration [ translation-unit ]
		function-definition [ translation-unit ]
*/
struct AST* parse_translation_unit(struct Queue *tokens,struct Scope *scope)
{
	struct AST_Translation_Unit *hold;
	hold=get_translation_unit_tree(scope);
	while(tokens->size>0)
	{
		parse_declaration(tokens,hold->scope,&hold->components,1);
	}
	return (struct AST*)hold;

}

#endif
