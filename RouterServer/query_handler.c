#include "mongoose.h"
#include "query_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

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
				"{\"status\":\"online\",\"user\":\"%s\",\"flow\":%lld}", user, flow);
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
		mg_printf(conn, "{\"user\":\"%s\",\"flow\":%lld}", tc_list[i].user, tc_list[i].input_bytes);
	}
	mg_printf(conn, "]");
	return "";
}

static void *query_vpn_connect(struct mg_connection *conn) {
	const struct mg_request_info *ri = mg_get_request_info(conn);
	char type[BUFFER_LEN];
	int qlen = strlen(ri->query_string);

	memset(type, 0, BUFFER_LEN);

	mg_get_var(ri->query_string, qlen, "type", type, BUFFER_LEN);

	if(type[0] == '4') {
		system("/mnt/usb/img/vpn4.sh");
		mg_printf(conn, HTTP_HEADER_PLAIN
				"0");
	} else if(type[0] == '6') {
		system("/mnt/usb/img/vpn6.sh");
		mg_printf(conn, HTTP_HEADER_PLAIN
				"0");
	} else {
		mg_printf(conn, HTTP_HEADER_PLAIN
				"-1");
	}
	return "";
}

static void *query_vpn_disconnect(struct mg_connection *conn) {
	system("/mnt/usb/img/vpn_clean.sh");
	mg_printf(conn, HTTP_HEADER_PLAIN
			"0");
	return "";
}

static void *query_vpn_check(struct mg_connection *conn) {
	FILE *file = fopen("/mnt/usb/img/vpn_exist", "r");
	int status = 0;
	if(file) {
		status = 1;
	} else {
		status = 0;
	}
	mg_printf(conn,HTTP_HEADER_PLAIN
			"%d", status);
	return "";
}


static void udp_broadcast(char *str) {
	int sock;
	socklen_t addr_len;
	struct sockaddr_in server_addr;

	if((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return;
	}
	/*
	int i = 1;
	socklen_t len= sizeof(i);
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &i, len);
	*/

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("192.168.23.168");
	server_addr.sin_port = htons(33333);  
	addr_len=sizeof(server_addr);
	sendto(sock, str, strlen(str), 0, (struct sockaddr*)&server_addr, addr_len);

	close(sock);
}

static void *query_phone_find(struct mg_connection *conn) {
	const struct mg_request_info *ri = mg_get_request_info(conn);
	char name[BUFFER_LEN];
	char buffer[BUFFER_LEN];
	int qlen = strlen(ri->query_string);

	memset(name, 0, BUFFER_LEN);

	mg_get_var(ri->query_string, qlen, "name", name, BUFFER_LEN);
	
	sprintf(buffer, "find:%s", name);
	printf("send:%s\n", buffer);
	udp_broadcast(buffer);

	mg_printf(conn, HTTP_HEADER_PLAIN
			"0");
	return "";

}

static void *query_phone_stop(struct mg_connection *conn) {
	const struct mg_request_info *ri = mg_get_request_info(conn);
	char name[BUFFER_LEN];
	char buffer[BUFFER_LEN];
	int qlen = strlen(ri->query_string);

	memset(name, 0, BUFFER_LEN);

	mg_get_var(ri->query_string, qlen, "name", name, BUFFER_LEN);

	sprintf(buffer, "stop:%s", name);
	printf("send:%s\n", buffer);
	udp_broadcast(buffer);

	mg_printf(conn, HTTP_HEADER_PLAIN
			"0");
	return "";
}



/*
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
*/

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
		.uri = "/vpn/connect",
		.handler = query_vpn_connect,
	},
	{
		.method = "GET",
		.uri = "/vpn/disconnect",
		.handler = query_vpn_disconnect,
	},
	{
		.method = "GET",
		.uri = "/vpn/check",
		.handler = query_vpn_check,
	},
	{
		.method = "GET",
		.uri = "/phone/find",
		.handler = query_phone_find,
	},
	{
		.method = "GET",
		.uri = "/phone/stop",
		.handler = query_phone_stop,
	},
	/*
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
	*/
};

int nr_query_handler = sizeof(query_handler_list) / sizeof(struct query_handler);
