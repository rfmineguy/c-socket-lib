#include "server.h"

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

	s.message_decoder_ht = ht_msg_type_msg_decoder_create();
	s.clients = vec_server_client_handler_create();
	printf("Initialized client vector\n");

	return s;
}
void server_close(server* server) {
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
void server_exit_callback(void* arg) {
	server* s = (struct server*)arg;
	close(s->server_fd);
}

void server_register_message_handler(server* server, int type, void(*msgDecoder)(struct server*, char*, int)) {
	if (msgDecoder == (void*)0) {
		fprintf(stderr, "Can't register a null message decoder function\n");
		return;
	}
	ht_msg_type_msg_decoder_put(&server->message_decoder_ht, type, msgDecoder);
	printf("Registered message decoder: %d\n", type);
}

void* server_client_thread(void* arg) {
	pthread_setname_np("client_thread");
	printf("Started client thread\n");
	server_client_handler* handler = (server_client_handler*)arg;
	char buf[1024] = {0};
	while (handler->server_ref->running) {
		message_ret ret = message_read(handler->client_fd);
		printf("read msg-type: %d\n", ret.type);
		if (ht_msg_type_msg_decoder_contains_key(&handler->server_ref->message_decoder_ht, ret.type)) {
			msg_decoder* d = ht_msg_type_msg_decoder_get(&handler->server_ref->message_decoder_ht, ret.type);
			(*d)(handler->server_ref, ret.payload, ret.size);
		}
	}
	printf("Client thread terminating\n");
	return NULL;
}
int set_socket_blocking(int sockfd) {
	int flags = fcntl(sockfd, F_GETFL, 0);
  if (flags == -1) {
    return -1; // Error getting flags
  }

  flags &= ~O_NONBLOCK; // Clear the non-blocking flag
  if (fcntl(sockfd, F_SETFL, flags) == -1) {
    return -1; // Error setting flags
  }
  return 0;
}
void* server_connection_thread(void* arg) {
	server* s = (server*)arg;
	int socket_fd;
	int blocking = 1;

	while (s->running) {
		check((socket_fd = accept(s->server_fd, (struct sockaddr*)&s->address, &s->addrlen)) < 0, "Failed to accept");
		check(set_socket_blocking(socket_fd) == -1, "Failed to set socket to blocking mode");

		vec_server_client_handler_pushback(&s->clients, (server_client_handler){.client_fd = socket_fd, .server_ref = s});
		server_client_handler* handler = vec_server_client_handler_get(&s->clients, (server_client_handler){.client_fd = socket_fd, .server_ref = s});
		assert(handler && "Handler is null");
		pthread_create(&handler->handler_thread, NULL, server_client_thread, handler);
		printf("New connection: %d\n", socket_fd);
 
		vec_server_client_handler_print(&s->clients);
	}

	printf("Connection thread terminating\n");
	return NULL;
}
