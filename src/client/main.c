#include "../common.h"
#include "../util.h"
#include "../message.h"
#include "../payloads.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct client {
	bool running;
	int client_fd;
	struct sockaddr_in server_address;
} client;

void client_exit_callback(void* dataptr) {
	client* client = (struct client*)dataptr;
	printf("exit: client_fd=%d\n", client->client_fd);

	close(client->client_fd);
	// send disconnect message here
}

void* message_listener_thread(void* arg) {
	client* clientp = (client*)arg;
	// we need to somehow ensure that the whole message is ready
	
	char buffer[1024] = {0};
	while (clientp->running) {
		// ssize_t b = message_read(clientp->client_fd);
		// message_decode_buffer(buffer, b);	
	}
	return NULL;
}

int main() {
	int status, val;
	char* message = "Pello world";
	char buffer[1024];
	pthread_t listener_thread;

	client client = {0};

	atexit(run_exit_handlers);
	register_exit(client_exit_callback, &client);

	check((client.client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0, "Failed to create socket");
	client.server_address.sin_family = AF_INET;
	client.server_address.sin_port = htons(PORT);

	check((inet_pton(AF_INET, "127.0.0.1", &client.server_address.sin_addr)) <= 0, "Address not supported");
	check((status = connect(client.client_fd, (struct sockaddr*)&client.server_address, sizeof(client.server_address))), "Failed to connect");

	pthread_create(&listener_thread, NULL, message_listener_thread, &client);
	pthread_join(listener_thread, NULL);

	message_send(client.client_fd, 0, sizeof("Pello World"), "Pello World");
	message_send(client.client_fd, 0, sizeof("New message"), "New message");
	message_send(client.client_fd, 1, sizeof(payload1), &(payload1){.x = 4, .words = "booger"});
}
