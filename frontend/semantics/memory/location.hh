#ifndef GCC_LOCATION_HH
#define GCC_LOCATION_HH GCC_LOCATION_HH
enum Location_Type
{
	LT_ON_STACK,
	LT_RAW,
	LT_RELATIVE,
	LT_LABELED,
	LT_GLOBAL
};
struct Location;
struct Location_Stack;
struct Location_Raw;
struct Location_Relative;
struct Location_Labeled;

#endif
