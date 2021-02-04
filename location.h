#ifndef GCC_LOCATION_H
#define GCC_LOCATION_H GCC_LOCATION_H

enum Location_Type
{
	LT_ON_STACK,
	LT_RAW,
	LT_RELATIVE,
	LT_LABELED,
	LT_GLOBAL
};
struct Location 
{
	enum Location_Type type;
};
struct Location_Stack
{
	enum Location_Type type;
	size_t offset;
};
struct Location_Raw
{
	enum Location_Type type;
	size_t address;
};

struct Location_Relative
{
	enum Location_Type type;
	struct Location *base;
	size_t offset;
};

struct Location_Labeled
{
	enum Location_Type type;
	struct token *id;
};

struct Location_Stack* get_location_on_stack(size_t offset);
struct Location* get_global_location();
struct Location_Raw* get_location_raw(size_t address);
struct Location_Relative* get_relative_location(struct Location *base,size_t offset);
struct Location* get_location_for_denoted_object(struct Location *base,struct Type *type,struct token *id);

#endif
