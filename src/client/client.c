#include "client.h"
#include "../message.h"
#include "../common.h"
#include <pthread.h>

void* client_message_listener_thread(void* arg) {
	client* clientp = (client*)arg;
	while (clientp->running) {
		message_ret ret = message_read(clientp->client_fd);
		printf("read msg-type: %d\n", ret.type);
		if (ht_client_msg_type_msg_decoder_contains_key(&clientp->message_decoder_ht, ret.type)) {
			client_msg_decoder* d = ht_client_msg_type_msg_decoder_get(&clientp->message_decoder_ht, ret.type);
			(*d)(clientp, clientp->client_fd, ret.payload, ret.size);
		}
	}
	return NULL;
}

void client_exit_callback(void* dataptr) {
	client* client = (struct client*)dataptr;
	printf("exit: client_fd=%d\n", client->client_fd);
	close(client->client_fd);
}

void   client_register_message_handler(client* c, int type, client_msg_decoder msgDecoder) {
	if (msgDecoder == (void*)0) {
		fprintf(stderr, "Can't register a null message decoder function\n");
		return;
	}
	ht_client_msg_type_msg_decoder_put(&c->message_decoder_ht, type, msgDecoder);
	printf("Registered message decoder: %d\n", type);
}

client client_new(const char* ip, int port) {
	int status;
	client c = {0};
	c.running = true;
	check((c.client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0, "Failed to create socket");
	c.server_address.sin_family = AF_INET;
	c.server_address.sin_port = htons(PORT);

	check((inet_pton(AF_INET, "127.0.0.1", &c.server_address.sin_addr)) <= 0, "Address not supported");
	check((status = connect(c.client_fd, (struct sockaddr*)&c.server_address, sizeof(c.server_address))), "Failed to connect");
	return c;
}

void client_close(client* c) {
	pthread_join(c->server_msg_listener_thread, NULL);
}
