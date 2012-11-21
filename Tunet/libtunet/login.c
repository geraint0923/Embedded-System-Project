#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <curl/curl.h>

size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp)
{
	sprintf((char*)userp,"%s",(char*)buffer);
	return size * nmemb;
}

void md5_cal(char* input, char* output)
{
	unsigned char value[16];
	int i;
	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx,(void*)input,strlen(input));
	MD5_Final(value,&ctx);
	for(i = 0; i < 16; i++)
		sprintf(output+2*i,"%02x",value[i]);
}

long long get_used_byte(char* in_result)
{
	int count = 2, i, len = strlen(in_result), stop;
	char result[1024];
	long long ret;
	strcpy(result,in_result);
	for(i = 0; i < len && count != 0; i++)
		if(result[i] == ',')
			count--;
	stop = i;
	while(result[i] <= '9' && result[i] >= '0' && i < len)
		i++;
	result[i] = 0;
	sscanf(result+stop,"%lld",&ret);
	return ret;
}

int main()
{
	char user_name[1024], passwd[1024], result[1024];
	scanf("%s %s",user_name,passwd);
	switch(tunet_login(user_name,passwd,result))
	{
		case -1:
			printf("curl error!\n");
			break;
		case 0:
			printf("used data count : %lld byte\n",get_used_byte(result));
			break;
		case 1:
			printf("something wrong : %s\n",result);
			break;
	}
	return 0;
}

int tunet_login(char* user_name, char* passwd, char* result)
{
	CURL* curl;
	int i, len;

	char passwd_md5[1024];
	char post_data[2048];
	char all_value[1024];

	md5_cal(passwd,passwd_md5);

	sprintf(post_data, "username=%s&password=%s&drop=0&type=1&n=100", user_name, passwd_md5);

	/*
	printf("POST DATA : %s\n", post_data);
	*/

	curl = curl_easy_init();
	if(!curl)
	{
		printf("null curl!\n");
		return -1;
	}

	curl_easy_setopt(curl,CURLOPT_URL,"http://net.tsinghua.edu.cn/cgi-bin/do_login");
	curl_easy_setopt(curl,CURLOPT_POSTFIELDS, post_data);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA, &all_value);
	curl_easy_setopt(curl,CURLOPT_POST, 1);
	curl_easy_setopt(curl,CURLOPT_VERBOSE, 0);
	curl_easy_setopt(curl,CURLOPT_HEADER, 0);
	curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION, 1);

	curl_easy_perform(curl);

	curl_easy_cleanup(curl);
	
	len = strlen(all_value);
	for(i = 0; i < len; i++)
		if(all_value[i] == '\r' || all_value[i] == '\n')
		{
			all_value[i] = 0;
			break;
		}
	strcpy(result,all_value);
	if(result[0] <= '9' && result[0] >= '0')
		return 0;
	else
		return 1;
}
