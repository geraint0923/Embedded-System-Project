#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

/*
 * parameters:
 * user_name - user name
 * passwd 	 - password of user
 * result	 - a buffer to save the result
 * len		 - the length of the buffer
 *
 * return value:
 * -1 - null curl, maybe a network problem
 *  0 - a successful login
 *  1 - there is something wrong
 */
int tunet_login_with_passwd(char* user_name, char* passwd, char* result, int len);

/*
 * parameters:
 * user_name - user name
 * md5		 - md5 sum of password
 * result	 - a buffer to save the result
 * len		 - the length of the buffer
 *
 * return value:
 * -1 - null curl, maybe a network problem
 *  0 - a successful login
 *  1 - there is something wrong
 */
int tunet_login_with_md5(char* user_name, char* md5, char* result, int len);


/*
 * parameter:
 * result	- a buffer to save the result
 * len		- the length of the buffer
 *
 * return value:
 * -1 - null curl, maybe a network problem
 *  0 - a successful logout
 *  1 - there is something wrong
 */
int tunet_logout(char* result, int len);

/*
 * reeturn value:
 * -1 - null curl, maybe a network problem
 *  0 - offline
 *  1 - online
 */
int tunet_check_online(char* result, int len);

