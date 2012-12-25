#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include "tunet_automator.h"

#define USER_COUNT	128
#define SWITCH_SEC	36

static struct tunet_connection ta_user_list[USER_COUNT]; 

static int ta_try_login = 0;
static int ta_current_user = -1;
static int ta_user_count = 0;

static int find_available_user() {
	int min_idx = -1, i;
	long long min_bytes = 999999999999;
	for(i = 0; i < USER_COUNT; ++i) {
		if(ta_user_list[i].user[0]) {
			if(min_bytes > ta_user_list[i].input_bytes) {
				min_bytes = ta_user_list[i].input_bytes;
				min_idx = i;
			}
		}
	}
	return min_idx;
}

static void *worker_thread(void *pvoid) {
	pthread_detach(pthread_self());
	while(ta_try_login) {
		int idx = find_available_user();
		struct tunet_connection tc;

		if(ta_current_user != idx && idx != -1) {

			tunet_logout(&tc);
			ta_current_user = -1;

			int try_count = 20;
			while(ta_try_login && try_count) {
				//TODO do login until success
				printf("try %s\n", ta_user_list[idx].user);
				tunet_login(&ta_user_list[idx], 1);
				if(!ta_user_list[idx].status) {
					ta_current_user = idx;
					break;
				}
				printf("try %s, but failed\n", ta_user_list[idx].user);
				sleep(1);
				--try_count;
			}

		}
		sleep(SWITCH_SEC);
	}
}

int ta_add_user(char *user, char *md5_passwd) {
	int i;
	for(i = 0; i < USER_COUNT; ++i) {
		if(!strcmp(user, ta_user_list[i].user)) {
			return -1;
		}
	}
	for(i = 0; i < USER_COUNT; ++i) {
		if(ta_user_list[i].user[0] == 0) {
			strcpy(ta_user_list[i].user, user);
			strcpy(ta_user_list[i].passwd, md5_passwd);
			++ta_user_count;
			return 0;
		}
	}
	return -1;
}

int ta_del_user(char *user) {
	int i;
	if(user == NULL || user[0] == 0)
		return -1;
	for(i = 0; i < USER_COUNT; ++i) {
		if(!strcmp(user, ta_user_list[i].user)) {
			ta_user_list[i].user[0] = 0;
			--ta_user_count;
			return 0;
		}
	}
	return -1;
}

int ta_login() {
	pthread_t tid;
	if(ta_user_count <= 0)
		return -1;
	ta_try_login = 1;
	pthread_create(&tid, NULL, worker_thread, NULL);
	return 0;
}

int ta_logout() {
	struct tunet_connection tc;
	tunet_logout(&tc);
	ta_current_user = -1;
	ta_try_login = 0;
	return 0;
}

int ta_check_online(char *user, int len, long long *flow) {
	/*
	if(ta_current_user != -1) {
		strncpy(user, ta_user_list[ta_current_user].user, len);
		*flow = ta_user_list[ta_current_user].input_bytes;
		return 1;
	} 
	*/
	struct tunet_connection tc;
	tunet_check_online(&tc);

	if(tc.status) {
		strncpy(user, tc.user, len);
		*flow = tc.input_bytes;
		return 1;
	} 

	return 0;
}

int ta_get_user_list(struct tunet_connection tc_list[], int nr_user) {
	int i, idx = 0;
	for(i = 0; i < USER_COUNT; ++i) {
		if(ta_user_list[i].user[0]) {
			tc_list[idx] = ta_user_list[i];
			++idx;
			if(idx == nr_user)
				break;
		}
	}
	return idx;
}

int ta_init(char *filename) {
	int i; 
	for(i = 0; i < USER_COUNT; ++i) {
		memset(&ta_user_list[i], 0, sizeof(struct tunet_connection));
	}
	if(filename) {
		// TODO : init the configuration
	}
	return 0;
}
