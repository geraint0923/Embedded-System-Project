#ifndef _QUERY_HANDLER_H_
#define _QUERY_HANDLER_H_

#include "mongoose.h"

struct query_handler {
	const char *method;
	const char *uri;
	void *(*handler)(struct mg_connection *conn);
};

extern struct query_handler query_handler_list[];

extern int nr_query_handler;


#endif
