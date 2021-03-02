#ifndef GCC_SCOPE_HH
#define GCC_SCOPE_HH GCC_SCOPE_HH

#define AS_NORMAL_SCOPE(x) ((struct Normal_Scope*)x)
#define AS_FUNCTION_SCOPE(x) ((struct Function_Scope*)x)
enum Scope_Type{
	FILE_SCOPE,
	BLOCK_SCOPE,
	FUNCTION_PROTOTYPE_SCOPE,
	FUNCTION_SCOPE
};
struct Scope;
struct Normal_Scope;
struct Function_Scope;
#endif
