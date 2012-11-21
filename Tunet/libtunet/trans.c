#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main()
{
	unsigned char ip[4];
	int a, b, c, d;
	scanf("%d.%d.%d.%d",&a,&b,&c,&d);
	ip[0] = d;
	ip[1] = c;
	ip[2] = b;
	ip[3] = a;
	printf("after transformat : %d\n",*(int*)ip);

	return 0;
}
