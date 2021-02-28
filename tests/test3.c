#define VERSION 2
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

int external_int;
int static_int;
int fib(int n)
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
int main()
{
	int *a;
	int iff;
	++external_int;
	a[10*100];
	alert(fib(10));
	+1;
	a->b&c;
	do 1; while(1);
	return 0;
}
