TARGET := router_server
CC := mips-openwrt-linux-gcc
$(TARGET) : mongoose.c mongoose.h router_server.c query_handler.h query_handler.c tunet_automator.c tunet_automator.h md5.h md5.c tunet.h tunet.c
	$(CC) mongoose.c md5.c tunet.c tunet_automator.c query_handler.c router_server.c -pthread -o $@ -std=c99 -O2 -W -Wall -pedantic -lcurl -ldl

origin : main.c
	gcc mongoose.c main.c -pthread -o $@ -std=c99 -O2 -W -Wall -pedantic

clean :
	rm -rf $(TARGET)

img : $(TARGET)
	rm -rf img
	mkdir img
	cp $(TARGET) img
	cp -r public img/public
