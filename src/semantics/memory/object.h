#ifndef WONKY_OBJECT_H
#define WONKY_OBJECT_H WONKY_OBJECT_H

#include <object.hh>
#include <type.h>
#include <location.h>


enum Storage_Class_Specifier;

struct Object
{
	struct Type *type;
	struct Location *location;
	enum Storage_Class_Specifier storage_class;
};

void delete_object(struct Object *object);

#endif
