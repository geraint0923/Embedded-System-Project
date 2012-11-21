#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tunet_protocol.h"


int main()
{
	char choice[12];
	char result[1024], user_name[1024], passwd[1024];
	if(tunet_check_online(result,1024))
		printf("You are online : %s\n",result);
	else
		printf("You are offline\n");

	printf("please select one:\n1.login\n2.logout\n");
	scanf("%s",choice);
	switch(choice[0])
	{
		case '1':
			printf("please input your user_name and password:\n");
			scanf("%s %s",user_name,passwd);
			if(!tunet_login(user_name,passwd,result,1024))
				printf("login successfully: %s\n",result);
			else
				printf("login failed: %s\n",result);
			break;
		case '2':
			if(!tunet_logout(result,1024))
				printf("logout:%s\n",result);
			break;
		default:
			if(tunet_check_online(result,1024))
				printf("You are online : %s\n",result);
			else
				printf("You are offline\n");
			break;
	}
	return 0;
}
