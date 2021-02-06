#ifndef GCC_DENOTED_HH
#define GCC_DENOTED_HH GCC_DENOTED_HH


enum Denotation_Type
{
	DT_Macro,
	DT_Macro_Parameter,
	DT_Label,
	DT_Object,
	DT_Typedef,
	DT_Function,
	DT_Enum,
	DT_Enum_Constant,
	DT_Struct_Union_Member,
	DT_Struct_Union_Tag,
	DT_Error,
	DT_Prototype
};

enum Function_Specifier
{
	FS_Inline,
	FS_None
};
enum Storage_Class
{
	SC_EXTERN,
	SC_STATIC,
	SC_TYPEDEF,
	SC_NONE
};

struct Denoted;
struct Denoted_Error;
struct Denoted_Base;
struct Denoted_Function;
struct Denoted_Object;
struct Denoted_Typedef;
struct Denoted_Enum;
struct Denoted_Enum_Const;
struct Denoted_Struct_Union;
struct Denotation_Prototype;
struct Object;


#endif
