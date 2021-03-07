#ifndef GCC_DENOTED_HH
#define GCC_DENOTED_HH GCC_DENOTED_HH

#define AS_DENOTED_OBJECT_PTR(x) ((struct Denoted_Object*)x)
#define AS_DENOTED_FUNCTION(x) ((struct Denoted_Function*)x)
#define AS_DENOTED_TYPEDEF(x) ((struct Denoted_Typedef*)x)
#define AS_DENOTED_ENUM(x) ((struct Denoted_Enum*)x)
#define AS_DENOTED_ENUM_CONST(x) ((struct Denoted_Enum_Const*)x)
#define AS_DENOTED_STRUCT_UNION(x) ((struct Denoted_Struct_Union*)x)

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


#endif
