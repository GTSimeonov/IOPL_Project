#ifndef GCC_TYPE_C 
#define GCC_TYPE_C GCC_TYPE_C
#include "type.h"


void merge_type_nodes(struct Type_Node *consumer,struct Type_Node *source)
{
	if(source->is_signed)
		consumer->is_signed=1;	
	if(source->is_const)
		consumer->is_const=1;	
	if(source->is_volatile)
		consumer->is_volatile=1;	
	if(source->is_extern)
		consumer->is_extern=1;	
	if(source->is_static)
		consumer->is_static=1;	
	if(source->is_bit_field)
		consumer->is_bit_field=1;	
	/*
	if(source->is_typedef)
		consumer->is_typedef=1;	
		*/
	if(source->error)
		consumer->error=1;	
	if(consumer->type_specifier==TS_NONE)
	{
		consumer->type_specifier=source->type_specifier;
	}else
	{
		consumer->error=1;
	}
	if(consumer->type_constraints==TC_NONE)
	{
		consumer->type_constraints=source->type_constraints;
	}
	if(consumer->type_constraints!=source->type_constraints)
	{
		consumer->error=1;
	}
	consumer->specifics=source->specifics;
}
struct Type_Node* check_first_type_component(struct Type *type)
{
	return ((struct Type_Node*)type->components.first->data);
}
struct Type_Node* check_base_type_component(struct Type *type)
{
	return ((struct Type_Node*)type->components.last->data);
}
size_t size_of(struct Type *type)
{
	struct Type_Node *hold;
	size_t size;

	hold=type->components.first->data;
		
	switch(hold->type_specifier)
	{
		case TS_INT:
			size=INT_SIZE;
			break;
		case TS_CHAR:
			size=CHAR_SIZE;
			break;
		case TS_FLOAT:
			size=FLOAT_SIZE;
			break;
		case TS_DOUBLE:
			size=DOUBLE_SIZE;
			break;
		case TS_ARRAY:
			return size_of_array(type);
		case TS_STRUCT:
			return size_of_struct(type);
		case TS_UNION:
			return size_of_union(type);
		case TS_POINTER:
			return PTR_SIZE;
		default:
			size=0;
	}
	switch(hold->type_constraints)
	{
		case TC_LONG:
			return size<<1;
			break;
		case TC_LONG_LONG:
			return size<<2;
			break;
		case TC_SHORT:
			return size>>1;
			break;
		default:
			return size;
	}

}

size_t size_of_array(struct Type *type)
{
	struct Type temp;
	struct Type_Node *hold;
	size_t size;

	hold=type->components.first->data;
	assert(hold->type_specifier==TS_ARRAY);
	temp.components=Queue_Fit(&type->components,1);
	size=hold->specifics.arr.number_of_elements*size_of(&temp);
	return size;


}
size_t size_of_struct(struct Type *type)
{
	struct Queue_Node *it;
	struct Queue_Node *it2;
	struct Type_Node *hold;
	size_t size=0;
	size_t i;

	hold=type->components.first->data;
	assert(hold->type_specifier==TS_STRUCT);

	if(hold->specifics.struct_union->declarations.size==0)
		return 0;


	for(it=hold->specifics.struct_union->declarations.first;it!=hold->specifics.struct_union->declarations.last->prev; it=it->prev)
	{
		if(DECLR_PTR(it->data)->declarators.size!=0)
		{
			for(it2=DECLR_PTR(it->data)->declarators.first;DECLR_PTR(it->data)->declarators.last->prev;it2=it2->prev)
			{
				size+=size_of(((struct Declarator*)(it2->data))->type);
			}
		}
	}

	return size;
}
size_t size_of_union(struct Type *type)
{
	struct Queue_Node *it;
	struct Queue_Node *it2;
	struct Type_Node *hold;
	size_t size=0;
	size_t hold_size;
	size_t i;

	hold=type->components.first->data;
	assert(hold->type_specifier==TS_STRUCT);

	if(hold->specifics.struct_union->declarations.size==0)
		return 0;


	for(it=hold->specifics.struct_union->declarations.first;it!=hold->specifics.struct_union->declarations.last->prev; it=it->prev)
	{
		if(DECLR_PTR(it->data)->declarators.size!=0)
		{
			for(it2=DECLR_PTR(it->data)->declarators.first;DECLR_PTR(it->data)->declarators.last->prev;it2=it2->prev)
			{
				if(hold_size>size)
				{
					size=hold_size;
				}
			}
		}
	}
	return size;
}

struct Type* get_type()
{
	struct Type *hold;
	hold=malloc(sizeof(struct Type));
	Queue_Init(&hold->components);
	return hold;

}

struct Type_Node* get_node()
{
	struct Type_Node *temp;
	temp=malloc(sizeof(struct Type_Node));
	temp->type_specifier=TS_NONE;
	temp->type_constraints=TC_NONE;
	temp->type_def=NULL;
	temp->is_typedef=temp->error=temp->is_const=temp->is_signed=temp->is_volatile=temp->is_extern=temp->is_static=temp->is_bit_field=0;
	return temp;
}
#endif
