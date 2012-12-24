#include "mongoose.h"
#include "query_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HTTP_HEADER_JSON		"HTTP/1.0 200 OK\r\n" \
								"Content-Type: text/json\r\n\r\n" 

static void *query_tunet_add_user(struct mg_connection *conn) {
	const struct mg_request_info *ri = mg_get_request_info(conn);
	mg_printf(conn, HTTP_HEADER_JSON
					"{\"a\":\"aaa\",\"b\":\"bbbbbb\"}");
	printf("printf!\n");
	return "";
}

static void *query_tunet_rm_user(struct mg_connection *conn) {
	const struct mg_request_info *ri = mg_get_request_info(conn);
	return NULL;
}

static void *query_tunet_login(struct mg_connection *conn) {
	const struct mg_request_info *ri = mg_get_request_info(conn);
	return NULL;
}

static void *query_tunet_logout(struct mg_connection *conn) {
	const struct mg_request_info *ri = mg_get_request_info(conn);
	return NULL;
}

static void *query_tunet_check_online(struct mg_connection *conn) {
	const struct mg_request_info *ri = mg_get_request_info(conn);
	return NULL;
}

static void *query_download_add_task(struct mg_connection *conn) {
	const struct mg_request_info *ri = mg_get_request_info(conn);
	return NULL;
}

static void *query_download_task_list(struct mg_connection *conn) {
	const struct mg_request_info *ri = mg_get_request_info(conn);
	return NULL;
}

static void *query_download_remove_task(struct mg_connection *conn) {
	const struct mg_request_info *ri = mg_get_request_info(conn);
	return NULL;
}

static void *query_download_task_set_state(struct mg_connection *conn) {
	const struct mg_request_info *ri = mg_get_request_info(conn);
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
		.uri = "/tunet/rm_user",
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
