#ifndef _LIBTUNET_H_
#define _LIBTUNET_H_

struct tunet_connection {
	char user[128];
	char passwd[128];
	int status;
	unsigned char ip_addr[4];
	long long input_bytes;
};

int tunet_get_passwd(char *input, int len);

void tunet_login(struct tunet_connection *tc int is_md5);

void tunet_logout(struct tunet_connection *tc);

void tunet_check_online(struct tunet_connection *tc);


#endif
