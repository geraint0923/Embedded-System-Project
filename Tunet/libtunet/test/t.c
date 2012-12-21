#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tunet_protocol.h"

int main() {
	char buffer[1024];
	if(!tunet_login_with_passwd("yangy09", "yy600703", buffer, 1024)) {
		printf("successfully : %s\n", buffer);
		tunet_check_online(buffer, 1024);
		printf("%s\n", buffer);
	} else {
		printf("login failed\n");
	}
	return 0;
}
