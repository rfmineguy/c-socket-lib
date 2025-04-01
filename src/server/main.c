#include "../common.h"
#include "../generated/vector_server_client_handler.h"
#include "../message.h"
#include "../util.h"
#include "server.h"
#include "../payloads.h"
#include <pthread.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>

void help() {
	printf("Help:\n");
	printf("  stop              - stops the server\n");
	printf("  list_connections  - lists clients connected to server\n");
	printf("  help              - displays this message\n");
}

void type0decoder(struct server* server, char* data, int size) {
	payload0* payload = (payload0*)data;
	printf("Decoding message type 0\n");
	printf("decoder0 :: buf=%s\n", payload->buf);
}

void type1decoder(struct server* server, char* data, int size) {
	payload1* payload = (payload1*)data;
	printf("decoder1 :: x=%d, words=%s\n", payload->x, payload->words);
}

int main() {
	server server = server_new(PORT);
	printf("Listening\n");
	pthread_create(&server.connection_thread, NULL, server_connection_thread, &server);

	// register exit handler
	atexit(run_exit_handlers);
	register_exit(server_exit_callback, &server);

	// register message decoders
	server_register_message_handler(&server, 0, type0decoder);
	server_register_message_handler(&server, 1, type1decoder);

	// server logic
	char buf[1024];
	while (true) {
		fgets(buf, 1024, stdin);
		buf[strlen(buf) - 1] = 0;
		
		// check input
		if (strncmp(buf, "stop", 1024) == 0) {
			printf("Stop server...\n");
			server.running = false;
			server_close(&server);
			break;
		}
		else if (strncmp(buf, "list_connections", 1024) == 0) {
			vec_server_client_handler_print(&server.clients);
			printf("\n");
		}
		else if (strncmp(buf, "help", 1024) == 0) {
			help();
		}
		else {
			help();
		}
	}
	server_close(&server);
}
