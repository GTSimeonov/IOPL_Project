#ifndef VERSION
#define VERSION 1
int kak;
#else
int err;
#endif

#undef VERSION
#ifdef VERSION
char asdf;
#endif

extern int external_int;
static int static_int;
static int fib(int n)
{
	int a=1,b=1,c;
	for(n;n>0;--n)
	{
		c=a;
		a+=b;
		b=c;
	}
	return a;

}
extern void alert(int a);
extern int main()
{
	int k[10*10][3][5];
	external_int++;
	alert(fib(10));
	return 0;
}
