#ifndef SERVER_DATA_H
#define SERVER_DATA_H
#include <pthread.h>

struct server;

typedef struct {
	pthread_t handler_thread;
	int client_fd;
	const struct server* server_ref;
} server_client_handler;

#endif
