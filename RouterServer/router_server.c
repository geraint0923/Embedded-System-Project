#include "mongoose.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>


static int exit_flag = 0;
static struct mg_context *ctx;
static char server_name[128];

static const char *options[] = {
	"document_root", "./public",
	"listening_ports", "8080",
	NULL
};

static void signal_handler(int sig_num) {
	exit_flag = sig_num;
}

static void *router_server_callback(enum mg_event ev, struct mg_connection *conn) {
	const struct mg_request_info *ri = mg_get_request_info(conn);
	if(ev == MG_NEW_REQUEST) {
		printf("new connection\n");
		mg_printf(conn, 
				"HTTP/1.0 200 OK\r\n"
				"Content-Type: text/json\r\n\r\n"
				"{ \"a\": \"aaa\", \"b\": \"bbb\"}"
				);
		return "";
	}

	return NULL;
}

static void init_signal() {
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
}

int main(int argc, char **argv) {
	init_signal();
	ctx = mg_start(&router_server_callback, NULL, options);
	while(exit_flag == 0) {
		sleep(1);
	}
	printf("Exiting on signal %d, waiting for all threads to finish...", exit_flag);
	fflush(stdout);
	mg_stop(ctx);
	printf("%s", " done.\n");

	return 0;
}
