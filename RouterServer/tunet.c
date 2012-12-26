#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <curl/curl.h>
#include "md5.h"
#include "tunet.h"

static int to_period(char *input, int idx);

static void get_ip_from_login_result(char* result, unsigned char ip[4]) {
	char tmp[1024];
	unsigned char *present;
	int i, len; 
	unsigned long long ret;
	strcpy(tmp, result + to_period(result,2));
	len = strlen(tmp);
	for(i = 0; i < len; i++) {
		if(tmp[i] == ',') {
			tmp[i] = 0;
			break;
		}
	}
	sscanf(tmp, "%llu", &ret);
	//printf("%s,%lld\n",tmp,ret);
	present = (unsigned char*) &ret;
	ip[0] = present[4];
	ip[1] = present[5];
	ip[2] = present[6];
	ip[3] = present[7];
}

static void get_user_name_from_check_online_result(char* result, char* user, int slen) {
	char tmp[1024];
	int i, len;
	strcpy(tmp,result+to_period(result,2));
	len = strlen(tmp);
	for(i = 0; i < len; i++) {
		if(tmp[i] == ',') {
			tmp[i] = 0;
			break;
		}
	}
	strncpy(user,tmp,slen);
}

static long long get_time_from_check_online_result(char* result) {
	char tmp[1024];
	int i, len;
	long long ret;
	strcpy(tmp,result+to_period(result,5));
	len = strlen(tmp);
	for(i = 0; i < len; i++) {
		if(tmp[i] == ',') {
			tmp[i] = 0;
			break;
		}
	}
	sscanf(tmp,"%lld",&ret);
	return ret;
}

static long long get_cost_from_check_online_result(char* result) {
	char tmp[1024];
	int i, len;
	long long ret;
	strcpy(tmp,result+to_period(result,3));
	len = strlen(tmp);
	for(i = 0; i < len; i++) {
		if(tmp[i] ==',') {
			tmp[i] = 0;
			break;
		}
	}
	sscanf(tmp,"%lld",&ret);
	return ret;
}

static long long get_cost_from_login_result(char* result) {
	return get_cost_from_check_online_result(result);
}

static int to_period(char* input, int index) {
	int i = 0;
	index--;
	while(index != 0) {
		if(input[i++] == ',') {
			index--;
		}
	}
	return i;
}

/*
static int change_display_mode() {
	struct termios term;
	int err;
	if(tcgetattr(STDIN_FILENO,&term) == -1) {
		perror("can not get attribution of terminal");
		return 0;
	}
	term.c_lflag ^= ECHO;
	err = tcsetattr(STDIN_FILENO,TCSAFLUSH,&term);
	if(err == -1 || err == EINTR) {
		perror("can not set attribution of terminal");
		return 0;
	}
	return 1;
}

static int is_display() {
	struct termios term;
	if(tcgetattr(STDIN_FILENO,&term) == -1) {
		perror("can not get attrbution of terminal");
		return -1;
	}
	if(term.c_lflag & ECHO)
		return 1;
	else
		return 0;
}
*/

/*
static int get_passwd(char* input, int len) {
	char tmp[1024];
	if(change_display_mode()) {
		scanf("%s",tmp);
		if(!change_display_mode())
			return 0;
		strncpy(input,tmp,len);
		return 1;
	}
	else
		return 0;

}
*/

static size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp) {
	sprintf((char*)userp,"%s",(char*)buffer);
	//printf("user space : %s\n", userp);
	return size * nmemb;
}

static void md5_cal(char* input, char* output) {
	unsigned char value[16];
	int i;
	MD5Context ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx,(void*)input,strlen(input));
	MD5_Final(&ctx, value);
	for(i = 0; i < 16; i++)
		sprintf(output+2*i,"%02x",value[i]);
}

static int post_data_to(char* url, char* post_data, char* value) {	
	CURL* curl;
	char all_value[1024];

	all_value[0] = 0;

	curl = curl_easy_init();

	if(!curl) {
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

static void trim_string(char* str) {
	int len = strlen(str), i;
	for(i = 0; i < len; i++) {
		if(str[i] == '\r' || str[i] == '\n') {
			str[i] = 0;
			break;
		}
	}
}

/*
static long long get_used_byte(char* in_result) {
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
*/

static int login_with_passwd(char* user_name, char* passwd, char* result, int len) {

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
	//printf("result:%s\n", result);
	
	if(result[0] <= '9' && result[0] >= '0')
		return 0;
	else
		return 1;
}


static int login_with_md5(char* user_name, char* md5, char* result, int len) {

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


static int logout(char* result, int len) {
	char url[1024];
	char all_value[1024];

	sprintf(url,"http://net.tsinghua.edu.cn/cgi-bin/do_logout");

	if(post_data_to(url,"",all_value) != 0)
		return -1;

	trim_string(all_value);

	strncpy(result,all_value,len);

	if(result[7] == 'o')
		return 0;

	return 1;
}

static int check_online(char* result, int len) {
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

/*
int tunet_get_passwd(char *input, int len) {
	return 0;
}
*/

#define BUFFER_LEN		1024

void tunet_login(struct tunet_connection *tc, int is_md5) {
	if(!tc)
		return;

	char buffer[BUFFER_LEN];

	memset(buffer, 0, BUFFER_LEN);

	if(is_md5) {
		tc->status = login_with_md5(tc->user, tc->passwd, buffer, BUFFER_LEN);
	} else {
		tc->status = login_with_passwd(tc->user, tc->passwd, buffer, BUFFER_LEN);
	}

	if(!tc->status) {
		get_ip_from_login_result(buffer, tc->ip_addr);
		tc->input_bytes = get_cost_from_login_result(buffer);
		tc->elapsed_time = 0;
	} else {
		strncpy(tc->err_msg, buffer, TC_BUFFER_LEN);
	}

}

void tunet_logout(struct tunet_connection *tc) {
	char buffer[BUFFER_LEN];
	memset(buffer, 0, BUFFER_LEN);

	if(tc) {
		memset(buffer, 0, BUFFER_LEN);
		tc->status = logout(buffer, BUFFER_LEN);
		if(tc->status != 0) {
			strncpy(tc->err_msg, buffer, TC_BUFFER_LEN);
		}
	}
}

void tunet_check_online(struct tunet_connection *tc) {
	char buffer[BUFFER_LEN];
	memset(buffer, 0, BUFFER_LEN);

	if(tc) {
		memset(buffer, 0, BUFFER_LEN);
		tc->status = check_online(buffer, BUFFER_LEN);
		if(tc->status == 0) {
			strncpy(tc->err_msg, buffer, TC_BUFFER_LEN);
		} else {
			get_user_name_from_check_online_result(buffer, tc->user, TC_BUFFER_LEN);
			tc->elapsed_time = get_time_from_check_online_result(buffer);
			tc->input_bytes = get_cost_from_check_online_result(buffer);
		}
	} 

}
