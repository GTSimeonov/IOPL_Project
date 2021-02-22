#ifndef GCC_SCOPE_HH
#define GCC_SCOPE_HH GCC_SCOPE_HH

enum Scope_Type{
	EXTERN_SCOPE,
	FILE_SCOPE,
	BLOCK_SCOPE,
	FUNCTION_PROTOTYPE_SCOPE,
	FUNCTION_SCOPE
};
struct Scope;
struct Normal_Scope;
struct Function_Scope;
#endif
