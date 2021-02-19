#ifndef GSTRING_C
#define GSTRING_C GSTRING_C
#include <gcc_string.h>


size_t gstrlen(char *str)
{
	size_t i;
	for(i=0;str[i]!='\0';++i);
	return i;
}

char* gstr_append(char *lead,char *follower)
{
	char *ret,*hold;
	hold=ret=malloc(gstrlen(lead) + gstrlen(follower)+1);
	while(*(hold++)=*(lead++));
	hold--;
	while(*(hold++)=*(follower++));
	return ret;
}

void strmv(char *target,char *source)
{
	while(*(target++)=*(source++));
}
char* gstrcpy(char *str)
{
	char *temp=malloc(gstrlen(str)+1);
	for(size_t i=0;(temp[i]=str[i])!='\0';++i);
	return temp;
}
char gstr_cmp(const char *a,const char *b)
{
	while(*a==*b && *a)
		++a,++b;
	if(*a==*b)
		return 1;
	else
		return 0;
}
#endif
