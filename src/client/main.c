#include "../common.h"
#include "../util.h"
#include "../message.h"
#include "../payloads.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>

#include "client.h"

void login_res_handler(struct client* c, int server_fd, char* data, int size) {
	payload_login_response* res = (payload_login_response*)data;
	printf("Login response handler\n");
	printf("  status: %d\n", res->status);
}

int main() {
	client c = client_new("127.0.0.1", PORT);
	client_register_message_handler(&c, pt_login_response, login_res_handler);

	pthread_create(&c.server_msg_listener_thread, NULL, client_message_listener_thread, &c);

	atexit(run_exit_handlers);
	register_exit(client_exit_callback, &c);

	message_send(c.client_fd, pt_login_request, sizeof(payload_login_request), &(payload_login_request){.username="rf", .password="pass"});
	sleep(5);

	client_close(&c);
}
