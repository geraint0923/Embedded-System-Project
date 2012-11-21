tunet : tunet.c tunet_protocol.c tunet_protocol.h  tunet_util.h tunet_util.c
#	gcc -g tunet_protocol.c main.c -lcurl -lrt -lz -lgnutls -lssl
	gcc -o tunet tunet.c tunet_protocol.c tunet_util.c -lcurl -lrt -lz -lgnutls -lssl

tmp :
	gcc tmp.c tunet_protocol.c tunet_util.c -lcurl -lrt -lz -lgnutls -lssl

