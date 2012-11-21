#include "tunet_protocol.h"
#include "tunet_util.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
	char user[1024], passwd[1024], result[1024];
	get_passwd(passwd, 1024);
	tunet_login_with_passwd("yy09", passwd, result, 1024);
	return 0;
}
