#ifndef GSTRING_H
#define GSTRING_H GSTRING_H
#include <stdlib.h>


size_t gstrlen(char *str);
char* gstr_append(char *lead,char *follower);
char* gstrcpy(char *str);
char gstr_cmp(const char *a,const char *b);
void strmv(char *target,char *source);



#endif
