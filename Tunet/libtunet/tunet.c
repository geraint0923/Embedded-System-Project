#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "tunet_protocol.h"
#include "tunet_util.h"

int print_state()
{
	char result[1024];
	char tmp[1024];
	printf("state		:	");
	if(tunet_check_online(result,1024))
	{
		printf("online\n");
		get_user_name_from_check_online_result(result,tmp,1024);
		printf("user		:	%s\n",tmp);
		printf("cost		:	%lld bytes\n",get_cost_from_check_online_result(result));
		printf("online time	:	%d seconds\n",get_time_from_check_online_result(result));
		return 1;
	}
	else
	{
		printf("offline\n");
		printf("user		:	unknown\n");
		printf("cost		:	unknown\n");
		printf("online time	:	0\n");
		return 0;
	}
}

void online_choose()
{
}


void offline_choose()
{
}


int main()
{
	printf("*********************************************\n");
	printf("		Welcome to GTunet!\n");
	printf("*********************************************\n");
	if(print_state())
		online_choose();
	else
		offline_choose();

	return 0;
}
