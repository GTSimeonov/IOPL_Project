#ifndef WONKY_OBJECT_C
#define WONKY_OBJECT_C WONKY_OBJECT_C
#include<object.h>



void delete_object(struct Object *object)
{
	if(object->location!=NULL)
		delete_location(object->location);
	free(object);
}

#endif
