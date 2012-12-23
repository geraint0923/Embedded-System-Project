TARGET := router_server
$(TARGET) : mongoose.c mongoose.h router_server.c query_handler.h query_handler.c
	gcc mongoose.c query_handler.c router_server.c -pthread -o $@ -std=c99 -O2 -W -Wall -pedantic

origin : main.c
	gcc mongoose.c main.c -pthread -o $@ -std=c99 -O2 -W -Wall -pedantic

clean :
	rm -rf $(TARGET)
