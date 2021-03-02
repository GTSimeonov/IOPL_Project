#ifndef GCC_LINKAGE_H
#define GCC_LINKAGE_H GCC_LINKAGE_H
#include <linkage.hh>
#include <map.h>
#include <denoted.h>

enum Linkage_Type;

/*this can be external or internal depending on which structure it is located*/
struct Linkage
{
	struct Map ids;
};


struct Linkage* get_linkage();
void delete_linkage();

/*these don't push the object into the linkage map*/
void resolve_object_linkage(struct Scope *scope,struct Translation_Data *translation_data,struct Denoted_Object *denoted_object);
void resolve_function_linkage(struct Scope *scope,struct Translation_Data *translation_data,struct Denoted_Function *denoted_function);
#endif
