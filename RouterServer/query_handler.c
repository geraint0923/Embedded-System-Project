#include "mongoose.h"
#include "query_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tunet.h"
#include "tunet_automator.h"

#define BUFFER_LEN	128

#define HTTP_HEADER_JSON		"HTTP/1.0 200 OK\r\n" \
								"Content-Type: text/json\r\n\r\n" 

#define HTTP_HEADER_PLAIN		"HTTP/1.0 200 OK\r\n" \
								"Content-Type: text/plain\r\n\r\n" 


static void *query_tunet_add_user(struct mg_connection *conn) {
	const struct mg_request_info *ri = mg_get_request_info(conn);
	char user[BUFFER_LEN], passwd[BUFFER_LEN];
	int qlen = strlen(ri->query_string);

	memset(user, 0, BUFFER_LEN);
	memset(passwd, 0, BUFFER_LEN);

	mg_get_var(ri->query_string, qlen, "user", user, BUFFER_LEN);
	mg_get_var(ri->query_string, qlen, "passwd", passwd, BUFFER_LEN);

	printf("%s|||%s\n", user, passwd);
	mg_printf(conn, HTTP_HEADER_PLAIN
			"%d", ta_add_user(user, passwd));
	/*
	mg_printf(conn, HTTP_HEADER_JSON
					"{\"a\":\"aaa\",\"b\":\"bbbbbb\"}");
	*/

	return "";
}

static void *query_tunet_rm_user(struct mg_connection *conn) {
	const struct mg_request_info *ri = mg_get_request_info(conn);
	char user[BUFFER_LEN];
	int qlen = strlen(ri->query_string);

	memset(user, 0, BUFFER_LEN);

	mg_get_var(ri->query_string, qlen, "user", user, BUFFER_LEN);

	mg_printf(conn, HTTP_HEADER_PLAIN
			"%d", ta_del_user(user));


	return "";
}

static void *query_tunet_check_online(struct mg_connection *conn) {
	//const struct mg_request_info *ri = mg_get_request_info(conn);
	char user[128];
	long long flow;
	int ret = ta_check_online(user, 128, &flow);
	if(ret) {
		mg_printf(conn, HTTP_HEADER_JSON
				"{\"status\":\"online\",\"user\":\"%s\",\"flow\":%ld}", user, flow);
	} else {
		mg_printf(conn, HTTP_HEADER_JSON
				"{\"status\":\"offline\",\"user\":\"\",\"flow\":0}");
	}
	return "";
}

static void *query_tunet_login(struct mg_connection *conn) {
	//const struct mg_request_info *ri = mg_get_request_info(conn);
	mg_printf(conn, HTTP_HEADER_PLAIN
			"%d", ta_login());
	return "";
}

static void *query_tunet_logout(struct mg_connection *conn) {
	//const struct mg_request_info *ri = mg_get_request_info(conn);
	ta_logout();
	mg_printf(conn, HTTP_HEADER_PLAIN
			"0");
	return "";
}

static void *query_tunet_user_list(struct mg_connection *conn) {
	//const struct mg_request_info *ri = mg_get_request_info(conn);
	struct tunet_connection tc_list[128];
	int i, len = ta_get_user_list(tc_list, 128);
	mg_printf(conn, HTTP_HEADER_PLAIN
			"[");
	for(i = 0; i < len; ++i) {
		if(i)
			mg_printf(conn, ",");
		mg_printf(conn, "{\"user\":\"%s\",\"flow\":%ld}", tc_list[i].user, tc_list[i].input_bytes);
	}
	mg_printf(conn, "]");
	return "";
}

static void *query_download_add_task(struct mg_connection *conn) {
	//const struct mg_request_info *ri = mg_get_request_info(conn);
	return NULL;
}

static void *query_download_task_list(struct mg_connection *conn) {
	//const struct mg_request_info *ri = mg_get_request_info(conn);
	return NULL;
}

static void *query_download_remove_task(struct mg_connection *conn) {
	//const struct mg_request_info *ri = mg_get_request_info(conn);
	return NULL;
}

static void *query_download_task_set_state(struct mg_connection *conn) {
	//const struct mg_request_info *ri = mg_get_request_info(conn);
	return NULL;
}


void *query_handle(struct mg_connection *conn) {
	int i;
	const struct mg_request_info *ri = mg_get_request_info(conn); 
	for(i = 0; i < nr_query_handler; ++i) {
		if(!strcmp(ri->request_method, query_handler_list[i].method) 
				&& !strcmp(ri->uri, query_handler_list[i].uri)) {
			return query_handler_list[i].handler(conn);
		}
	}
	return NULL;
}



struct query_handler query_handler_list[] = {
	{
		.method = "GET",
		.uri = "/tunet/add_user",
		.handler = query_tunet_add_user,
	},
	{
		.method = "GET",
		.uri = "/tunet/remove_user",
		.handler = query_tunet_rm_user,
	},
	{
		.method = "GET",
		.uri = "/tunet/login",
		.handler = query_tunet_login,
	},
	{
		.method = "GET",
		.uri = "/tunet/logout",
		.handler = query_tunet_logout,
	},
	{
		.method = "GET",
		.uri = "/tunet/check_online",
		.handler = query_tunet_check_online,
	},
	{
		.method = "GET",
		.uri = "/tunet/user_list",
		.handler = query_tunet_user_list,
	},
	{
		.method = "GET",
		.uri = "/download/add_task",
		.handler = query_download_add_task,
	},
	{
		.method = "GET",
		.uri = "/download/task_list",
		.handler = query_download_task_list,
	},
	{
		.method = "GET",
		.uri = "/download/remove_task",
		.handler = query_download_remove_task,
	},
	{
		.method = "GET",
		.uri = "/download/task_set_state",
		.handler = query_download_task_set_state,
	},
};

int nr_query_handler = sizeof(query_handler_list) / sizeof(struct query_handler);
