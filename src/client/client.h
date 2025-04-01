#ifndef CLIENT_H
#define CLIENT_H
#include <arpa/inet.h>
#include <stdbool.h>
#include "../generated/hash_table_client_msg_type_msg_decoder.h"

typedef struct client {
	bool running;
	int client_fd;
	struct sockaddr_in server_address;
	pthread_t server_msg_listener_thread;

	ht_client_msg_type_msg_decoder message_decoder_ht;
} client;

void* client_message_listener_thread(void*);
void client_exit_callback(void*);

void   client_register_message_handler(client*, int, client_msg_decoder msgDecoder);

client client_new(const char*, int);
void client_close(client*);

#endif
