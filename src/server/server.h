#ifndef SERVER_H
#define SERVER_H
#include "../common.h"
#include "../generated/vector_server_client_handler.h"
#include "../generated/hash_table_msg_type_msg_decoder.h"
#include "../message.h"
#include "../util.h"
#include <pthread.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>

typedef struct server {
	int port;

	int server_fd;
	struct sockaddr_in address;
	socklen_t addrlen;
	bool running;

	vec_server_client_handler clients;
	ht_msg_type_msg_decoder message_decoder_ht;

	pthread_t connection_thread;
} server;

server server_new(int port);
void   server_close(server*);
void   server_exit_callback(void*);

void   server_register_message_handler(server*, int, void(*msgDecoder)(char*, int));

void*  server_client_thread(void*);
void*  server_connection_thread(void*);

#endif
