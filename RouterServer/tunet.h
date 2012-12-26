#ifndef _LIBTUNET_H_
#define _LIBTUNET_H_

#define TC_BUFFER_LEN	128

struct tunet_connection {
	char user[TC_BUFFER_LEN];
	char passwd[TC_BUFFER_LEN];
	char err_msg[TC_BUFFER_LEN];
	int status;
	unsigned char ip_addr[4];
	long long input_bytes;
	long long elapsed_time;
};

//int tunet_get_passwd(char *input, int len);

void tunet_login(struct tunet_connection *tc, int is_md5);

void tunet_logout(struct tunet_connection *tc);

void tunet_check_online(struct tunet_connection *tc);


#endif
