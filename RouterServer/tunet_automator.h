#ifndef _TUNET_AUTOMATOR_H_
#define _TUNET_AUTOMATOR_H_

#include "tunet.h"

int ta_add_user(char *user, char *md5_passwd);

int ta_del_user(char *user);

int ta_login();

int ta_logout();

int ta_init(char *filename);

/*
   return value :
	0 - offline
	1 - online
*/
int ta_check_online(char *user, int len, long long *flow);

int ta_get_user_list(struct tunet_connection tc_list[], int nr_user);

#endif
