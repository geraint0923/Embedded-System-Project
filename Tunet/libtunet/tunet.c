#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>


void get_ip_from_login_result(char* result, unsigned char ip[4])
{
	char tmp[1024];
	unsigned char *present;
	int i, len; 
	unsigned long long ret;
	strcpy(tmp,result+to_period(result,2));
	len = strlen(tmp);
	for(i = 0; i < len; i++)
		if(tmp[i] == ',')
		{
			tmp[i] = 0;
			break;
		}
	sscanf(tmp,"%lld",&ret);
	printf("%s,%lld\n",tmp,ret);
	present = (char*) &ret;
	ip[0] = present[4];
	ip[1] = present[5];
	ip[2] = present[6];
	ip[3] = present[7];
}

void get_user_name_from_check_online_result(char* result, char* user, int slen)
{
	char tmp[1024];
	int i, len;
	strcpy(tmp,result+to_period(result,2));
	len = strlen(tmp);
	for(i = 0; i < len; i++)
		if(tmp[i] == ',')
		{
			tmp[i] = 0;
			break;
		}
	strncpy(user,tmp,slen);
}

int get_time_from_check_online_result(char* result)
{
	char tmp[1024];
	int i, len, ret;
	strcpy(tmp,result+to_period(result,5));
	len = strlen(tmp);
	for(i = 0; i < len; i++)
		if(tmp[i] == ',')
		{
			tmp[i] = 0;
			break;
		}
	sscanf(tmp,"%d",&ret);
	return ret;
}

long long get_cost_from_check_online_result(char* result)
{
	char tmp[1024];
	int i, len;
	long long ret;
	strcpy(tmp,result+to_period(result,3));
	len = strlen(tmp);
	for(i = 0; i < len; i++)
		if(tmp[i] ==',')
		{
			tmp[i] = 0;
			break;
		}
	sscanf(tmp,"%lld",&ret);
	return ret;
}

long long get_cost_from_login_result(char* result)
{
	return get_cost_from_check_online_result(result);
}

int to_period(char* input, int index)
{
	int i = 0;
	index--;
	while(index != 0)
		if(input[i++] == ',')
			index--;
	return i;
}
int change_display_mode()
{
	struct termios term;
	int err;
	if(tcgetattr(STDIN_FILENO,&term) == -1)
	{
		perror("can not get attribution of terminal");
		return 0;
	}
	term.c_lflag ^= ECHO;
	err = tcsetattr(STDIN_FILENO,TCSAFLUSH,&term);
	if(err == -1 || err == EINTR)
	{
		perror("can not set attribution of terminal");
		return 0;
	}
	return 1;
}

int is_display()
{
	struct termios term;
	if(tcgetattr(STDIN_FILENO,&term) == -1)
	{
		perror("can not get attrbution of terminal");
		return -1;
	}
	if(term.c_lflag & ECHO)
		return 1;
	else
		return 0;
}

int get_passwd(char* input, int len)
{
	char tmp[1024];
	/*
	if(is_display())
		printf("before change : display\n");
	else
		printf("before change : not display\n");
	*/
	if(change_display_mode())
	{
		scanf("%s",tmp);
		/*
		printf("after first change : ");
		if(is_display())
			printf("display\n");
		else
			printf("not display\n");
		*/
		if(!change_display_mode())
			return 0;
		strncpy(input,tmp,len);
		/*
		printf("after second change : ");
		if(is_display())
			printf("display\n");
		else
			printf("not display\n");
		*/
		return 1;
	}
	else
		return 0;

}
