#ifndef GCC_PREPROCESSING_C
#define GCC_PREPROCESSING_C GCC_PREPROCESSING_C
#include "preprocessing.h"

struct define_directive
{
	struct token *macro_name;
	struct Queue replacement_list;
	/*the tokens of the macro (contains a special token)*/
	struct Queue id_list;

	struct Map arguments;
	size_t number_of_arguments;
	/*put arguments here*/
	struct token **argument_list;
};

#endif
