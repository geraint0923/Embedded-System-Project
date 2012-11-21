#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <curl/curl.h>
#include "tunet_protocol.h"

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

int post_data_to(char* url, char* post_data, char* value)
{	
	int i;
	CURL* curl;
	char all_value[1024];

	all_value[0] = 0;

	curl = curl_easy_init();

	if(!curl)
	{
		printf("null curl!\n");
		return -1;
	}

	curl_easy_setopt(curl,CURLOPT_URL,url);
	curl_easy_setopt(curl,CURLOPT_POSTFIELDS, post_data);
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA, &all_value);
	curl_easy_setopt(curl,CURLOPT_POST, 1);
	curl_easy_setopt(curl,CURLOPT_VERBOSE, 0);
	curl_easy_setopt(curl,CURLOPT_HEADER, 0);
	curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION, 1);

	curl_easy_perform(curl);

	curl_easy_cleanup(curl);

	strcpy(value,all_value);
	
	return 0;
}	

void trim_string(char* str)
{
	int len = strlen(str), i;
	for(i = 0; i < len; i++)
		if(str[i] == '\r' || str[i] == '\n')
		{
			str[i] = 0;
			break;
		}
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

int tunet_login_with_passwd(char* user_name, char* passwd, char* result, int len)
{
	CURL* curl;
	int i, slen;

	char url[1024];
	char passwd_md5[1024];
	char post_data[2048];
	char all_value[1024];

	md5_cal(passwd,passwd_md5);

	sprintf(url,"http://net.tsinghua.edu.cn/cgi-bin/do_login");
	sprintf(post_data, "username=%s&password=%s&drop=0&type=1&n=100", user_name, passwd_md5);

	/*
	printf("POST DATA : %s\n", post_data);
	*/
	if(post_data_to(url,post_data,all_value) != 0)
		return -1;

	trim_string(all_value);

	strncpy(result,all_value,len);
	
	if(result[0] <= '9' && result[0] >= '0')
		return 0;
	else
		return 1;
}


int tunet_login_md5(char* user_name, char* md5, char* result, int len)
{
	CURL* curl;
	int i, slen;

	char url[1024];
	char post_data[2048];
	char all_value[1024];

	sprintf(url,"http://net.tsinghua.edu.cn/cgi-bin/do_login");
	sprintf(post_data, "username=%s&password=%s&drop=0&type=1&n=100", user_name, md5);

	/*
	printf("POST DATA : %s\n", post_data);
	*/
	if(post_data_to(url,post_data,all_value) != 0)
		return -1;

	trim_string(all_value);

	strncpy(result,all_value,len);
	
	if(result[0] <= '9' && result[0] >= '0')
		return 0;
	else
		return 1;
}


int tunet_logout(char* result, int len)
{
	CURL* curl;
	char url[1024];
	char all_value[1024];
	int i, slen;

	sprintf(url,"http://net.tsinghua.edu.cn/cgi-bin/do_logout");

	if(post_data_to(url,"",all_value) != 0)
		return -1;

	trim_string(all_value);

	strncpy(result,all_value,len);

	return 0;
}

int tunet_check_online(char* result, int len)
{
	CURL* curl;
	char url[1024];
	char post_data[1024];
	char all_value[1024];
	
	sprintf(url,"http://net.tsinghua.edu.cn/cgi-bin/do_login");
	sprintf(post_data,"action=check_online");

	if(post_data_to(url,post_data,all_value) != 0)
		return -1;

	trim_string(all_value);

	if(result)
		strncpy(result,all_value,len);

	if(all_value[0])
		return 1;
	else
		return 0;
}
