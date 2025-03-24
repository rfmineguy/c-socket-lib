#include "../common.h"
#include "../generated/vector_server_client_handler.h"
#include <pthread.h>
#include <stdbool.h>
#include <assert.h>

typedef struct server {
	int port;

	int server_fd;
	struct sockaddr_in address;
	socklen_t addrlen;
	bool running;

	vec_server_client_handler clients;

	pthread_t connection_thread;
} server;

server server_new(int port) {
	server s = (server){.port = port, .running = true};

	int opt;
	check((s.server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0, "Failed to create server socket");
	check(setsockopt(s.server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0, "Failed to setsockopt");
	s.address.sin_family = AF_INET;
	s.address.sin_addr.s_addr = INADDR_ANY;
	s.address.sin_port = htons(PORT);
	check(bind(s.server_fd, (struct sockaddr*)&s.address, sizeof(s.address)) < 0, "Failed to bind");
	check(listen(s.server_fd, 3) < 0, "Failed to listen");

	s.clients = vec_server_client_handler_create();
	printf("Initialized client vector\n");

	return s;
}

void server_close(const server* server) {
	struct timeval tv;
	tv.tv_usec = 1;
	tv.tv_sec = 0;
	close(server->server_fd);
	pthread_join(server->connection_thread, NULL);
	for (int i = 0; i < server->clients.size; i++) {
		check(setsockopt(server->clients.data[i].client_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0, "Failed to set timeout");
		pthread_join(server->clients.data[i].handler_thread, NULL);
		close(server->clients.data[i].client_fd);
	}
}

void* client_thread(void* arg) {
	pthread_setname_np("client_thread");
	printf("Started client thread\n");
	server_client_handler* handler = (server_client_handler*)arg;
	while (handler->server_ref->running) {
		usleep(10000);
	}
	printf("Client thread terminating\n");
	return NULL;
}

void* connection_thread(void* arg) {
	server* s = (server*)arg;
	int socket_fd;

	while (s->running) {
		check((socket_fd = accept(s->server_fd, (struct sockaddr*)&s->address, &s->addrlen)) < 0, "Failed to accept");

		vec_server_client_handler_pushback(&s->clients, (server_client_handler){.client_fd = socket_fd, .server_ref = s});
 		server_client_handler* handler = vec_server_client_handler_get(&s->clients, (server_client_handler){.client_fd = socket_fd, .server_ref = s});
 		assert(handler && "Handler is null");
 		pthread_create(&handler->handler_thread, NULL, client_thread, handler);
 		printf("New connection: %d\n", socket_fd);
 
		vec_server_client_handler_print(&s->clients);
	}

	printf("Connection thread terminating\n");
	return NULL;
}

void help() {
	printf("Help:\n");
	printf("  stop  - stops the server\n");
	printf("  help  - displays this message\n");
}

int main() {
	server server = server_new(PORT);
	printf("Listening\n");
	pthread_create(&server.connection_thread, NULL, connection_thread, &server);
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
		else if (strncmp(buf, "help", 1024) == 0) {
			help();
		}
		else {
			help();
		}
	}
	server_close(&server);
}
