#ifndef GCC_LINKAGE_C
#define GCC_LINKAGE_C GCC_LINKAGE_C
#include<linkage.h>





struct Linkage* get_linkage()
{
	struct Linkage *ret;
	ret=malloc(sizeof(struct Linkage));
	Map_Init(&ret->ids);

	return ret;
}

/*denoted things are deleted here and left alone when deleting scopes*/
void delete_linkage(struct Linkage *linkage)
{
	Map_Map(&linkage->ids,delete_denoted_wrapper);
	Map_Destroy(&linkage->ids);
	free(linkage);
}


/*TODO optimise when you know what should happen here*/

void account_for_upper_linkage_on_object(struct Scope *scope,struct Translation_Data *translation_data,struct Denoted_Object *denoted_object)
{
	struct Denoted_Object *hold_object;
	hold_object=check_ordinary(scope,denoted_object->id);
	if(hold_object!=NULL && hold_object->denotation==DT_Object && types_are_identical(denoted_object->object->type,hold_object->object->type))
	{
		if(hold_object->linkage!=LINKAGE_NONE)
		{
			denoted_object->linkage=hold_object->linkage;
			denoted_object->object->storage_class=SCS_STATIC;
		}else
		{
			denoted_object->linkage=LINKAGE_EXTERNAL;
			denoted_object->object->storage_class=SCS_STATIC;
		}
	}else
	{
		denoted_object->linkage=LINKAGE_EXTERNAL;
		denoted_object->object->storage_class=SCS_STATIC;
	}
}
void account_for_upper_linkage_on_function(struct Scope *scope,struct Translation_Data *translation_data,struct Denoted_Function *denoted_function)
{
	struct Denoted_Function *hold_function;
	hold_function=check_ordinary(scope,denoted_function->id);
	if(hold_function!=NULL)
	{
		if(hold_function->denotation==DT_Function && types_are_identical(denoted_function->type,hold_function->type))
		{
			denoted_function->linkage=( (hold_function->linkage==LINKAGE_NONE) ? LINKAGE_EXTERNAL : hold_function->linkage);
		}else
		{
			push_translation_error("linking mismatching things",translation_data);
			return;
		}
	}else
	{
		denoted_function->linkage=LINKAGE_EXTERNAL;
	}
}
void resolve_object_linkage(struct Scope *scope,struct Translation_Data *translation_data,struct Denoted_Object *denoted_object)
{
	if(scope->type==FILE_SCOPE)
	{
		if(denoted_object->object->storage_class==SCS_NONE || denoted_object->object->storage_class==SCS_EXTERN)
		{
			denoted_object->linkage=LINKAGE_EXTERNAL;
			denoted_object->object->storage_class=SCS_STATIC;
			account_for_upper_linkage_on_object(scope,translation_data,denoted_object);

		}else if(denoted_object->object->storage_class==SCS_STATIC) 
		{
			denoted_object->linkage=LINKAGE_INTERNAL;
			denoted_object->object->storage_class=SCS_STATIC;
		}else
		{
			assert(0);
		}
	}else if(scope->type==FUNCTION_PROTOTYPE_SCOPE)
	{
		denoted_object->linkage=LINKAGE_NONE;
		denoted_object->object->storage_class=SCS_NONE;
		if(denoted_object->object->storage_class!=SCS_NONE && denoted_object->object->storage_class!=SCS_REGISTER)
		{
			push_translation_error("storage class specifier other than register in function prototype scope",translation_data);
			return ;
		}
	}else if(scope->type==BLOCK_SCOPE)
	{
		denoted_object->linkage=LINKAGE_NONE;
		/*here comes the spooky part*/
		if(denoted_object->object->storage_class==SCS_EXTERN)
			account_for_upper_linkage_on_object(scope,translation_data,denoted_object);
	}else
	{
		assert(0);
	}

	/*check if id is linked oppositely*/
	if(denoted_object->linkage==LINKAGE_EXTERNAL &&
		Map_Check(&translation_data->internal_linkage->ids,denoted_object->id->data,denoted_object->id->data_size)!=NULL)
	{
		push_translation_error("linking id both internally and externally",translation_data);
	}else if(denoted_object->linkage==LINKAGE_INTERNAL && 
		Map_Check(&translation_data->external_linkage->ids,denoted_object->id->data,denoted_object->id->data_size)!=NULL)
	{
		push_translation_error("linking id both internally and externally",translation_data);
	}
}
void resolve_function_linkage(struct Scope *scope,struct Translation_Data *translation_data,struct Denoted_Function *denoted_function)
{
	if(scope->type==BLOCK_SCOPE)
	{
		if(denoted_function->linkage==LINKAGE_INTERNAL)
		{
			push_translation_error("static storage class specifier on function declaration in block scope",translation_data);
			return;
		}else
		{
			denoted_function->linkage=LINKAGE_EXTERNAL;
			account_for_upper_linkage_on_function(scope,translation_data,denoted_function);

		}
	}else if(scope->type==FILE_SCOPE)
	{
		if(denoted_function->linkage==LINKAGE_NONE)
			denoted_function->linkage=LINKAGE_EXTERNAL;
		/*falltrough*/

		if(denoted_function->linkage==LINKAGE_EXTERNAL)
			account_for_upper_linkage_on_function(scope,translation_data,denoted_function);

	}else
	{
		assert(0);
	}

	/*check if id is linked oppositely*/
	if(denoted_function->linkage==LINKAGE_EXTERNAL &&
		Map_Check(&translation_data->internal_linkage->ids,denoted_function->id->data,denoted_function->id->data_size)!=NULL)
	{
		push_translation_error("linking id both internally and externally",translation_data);
	}else if(denoted_function->linkage==LINKAGE_INTERNAL &&
		Map_Check(&translation_data->external_linkage->ids,denoted_function->id->data,denoted_function->id->data_size)!=NULL)
	{
		push_translation_error("linking id both internally and externally",translation_data);
	}

}

#endif
