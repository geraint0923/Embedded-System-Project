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

int main()
{
	char result[1024];
	tunet_logout(result);
	printf("%s\n",result);
	return 0;
}

int tunet_logout(char* result)
{
	CURL* curl;
	char all_value[1024];
	int i, len;

	curl = curl_easy_init();
	if(!curl)
	{
		printf("null curl!\n");
		return -1;
	}

	curl_easy_setopt(curl,CURLOPT_URL,"http://net.tsinghua.edu.cn/cgi-bin/do_logout");
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA, &all_value);
	curl_easy_setopt(curl,CURLOPT_POSTFIELDS,"");
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

	return 0;
}
