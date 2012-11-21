#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tunet_util.h"

int main()
{
	char input[1024];
	if(!get_passwd(input,1024))
	{
		printf("failed to get password\n");
		return -1;
	}
	else
		printf("your password is : %s\n",input);
	return 0;
}
