/*************************************************************************
    > File Name: flow_balance.h
    > Author: tomxice
    > Mail: tomxice@gmail.com 
    > Created Time: Mon 24 Dec 2012 03:32:07 PM CST
 ************************************************************************/
#ifndef _FLOW_BALANCE_H_
#define _FLOW_BALANCE_H_
#include "tunet.h"
// parameter strategy is currently unavailable...
// pass NULL instead..
void init_flow_balance(void* s);

// return value is err code, 0 for OK
int stop_flow_balance();

// add a new tc to pool
void fb_add_tc(struct tunet_connection *tc);

// remove a tc from pool
void fb_remove_tc(struct tunet_connection *tc);

#endif 



