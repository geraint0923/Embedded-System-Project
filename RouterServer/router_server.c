#include "mongoose.h"
#include "query_handler.h"

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
static char server_name[] = "Router Server";

static char *options[] = {
	"document_root", "./public",
	"listening_ports", "8080",
	NULL
};

static void signal_handler(int sig_num) {
	exit_flag = sig_num;
}

static void *router_server_callback(enum mg_event ev, struct mg_connection *conn) {
	const struct mg_request_info *ri = mg_get_request_info(conn); 
	void *ret = NULL;
	if(ev == MG_NEW_REQUEST) {
		printf("new connection method:%s  uri:%s\n", ri->request_method, ri->uri);
		ret = query_handle(conn);
	//	printf("0x%08x\n", ret);
		return ret;
	}

	return NULL;
}

static void init_signal() {
	signal(SIGTERM, signal_handler);
	signal(SIGINT, signal_handler);
}

int main(int argc, char **argv) {
	init_signal();
	if(argc == 2) {
		options[3] = (char*)malloc(strlen(argv[1])+1);
		sprintf(options[3], "%s", argv[1]);
	}
	ctx = mg_start(&router_server_callback, NULL, (const char**)options);
	if(!ctx) {
		printf("Null context, failed to start the mongoose server.\n");
		return -1;
	}
	printf("%s started on port(s) %s with web root [%s]\n",
			server_name, mg_get_option(ctx, "listening_ports"),
			mg_get_option(ctx, "document_root"));

	while(exit_flag == 0) {
		sleep(1);
	}
	printf("Exiting on signal %d, waiting for all threads to finish...", exit_flag);
	fflush(stdout);
	mg_stop(ctx);
	printf("%s", " done.\n");

	return 0;
}
