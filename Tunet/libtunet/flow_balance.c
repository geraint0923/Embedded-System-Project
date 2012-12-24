/*************************************************************************
    > File Name: flow_balance.c
    > Author: tomxice
    > Mail: tomxice@gmail.com 
    > Created Time: Mon 24 Dec 2012 03:47:01 PM CST
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include"flow_balance.h"

#define POOL_SIZE 20
struct tunet_connection* tc_pool[POOL_SIZE];
int tc_cnt = 0;
int fb_stop = 0;
pthread_mutex_t pool_lock;

// parameter strategy is currently unavailable...
// pass NULL instead..
void init_flow_balance(void* s) {
	pthread_mutex_init(&pool_lock, NULL);
	struct tunet_connection* ptc = NULL;
	int idx = 0;
	while (! fb_stop) {
		//lock
		pthread_mutext_lock(&pool_lock);
		idx %= tc_cnt;
		ptc = tc_pool[idx];
		idx += 1;
		tunet_login(ptc, 1);	
		//unlock
		pthread_mutext_unlock(&pool_lock);
		sleep(3600);
		//lock
		pthread_mutext_lock(&pool_lock);
		tunet_logout(ptc);
		pthread_mutext_unlock(&pool_lock);
		//unlock
	}
	pthread_mutex_destory(&pool_lock);
}

// return value is err code, 0 for OK
int stop_flow_balance() {
	fb_stop = 1;
}

// add a new tc to pool
void fb_add_tc(struct tunet_connection *tc) {
	//lock
	pthread_mutext_lock(&pool_lock);
	if (tc_cnt >= POOL_SIZE) {
		printf("exceed pool\n");
		return;
	}
	tc_pool[tc_cnt] = tc;
	tc_cnt ++;
	//unlock
	pthread_mutext_unlock(&pool_lock);
}

// remove a tc from pool
void fb_remove_tc(struct tunet_connection *tc) {
	int i,j;
	//lock
	pthread_mutext_lock(&pool_lock);
	for (i = 0; i < tc_cnt; ++ i) {
		if (tc_pool[i] == tc) {
			for (j = i; j < tc_cnt-1; ++ j) {
				tc_pool[j] = tc_pool[j+1];
			}
			break;
		}
	}
	//unlock
	pthread_mutext_unlock(&pool_lock);
}

#endif 



