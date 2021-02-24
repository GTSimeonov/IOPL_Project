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
int main(int argc,char *argv[])
{
	int k[10*10];
	a=b=c=d;
	external_int++;
	fib(10);
	return 0;
}
