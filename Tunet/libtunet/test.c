#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tunet_util.h"

int main()
{
	/*
	char result[] = "414477228996833,2631582378438426624,2730965969,0,0";
	unsigned char ip[4];
	*/
	char result[] = "414477228996833,yangy09,2730965969,0,2134";
	/*
	printf("%lld",get_cost_from_check_online_result(result));
	get_ip_from_login_result(result,ip);
	printf("%d.%d.%d.%d\n",ip[0],ip[1],ip[2],ip[3]);
	*/
	printf("time:%d\n",get_time_from_check_online_result(result));
	return 0;
}
