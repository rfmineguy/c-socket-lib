#include "serverdata.h"
#include <stdio.h>

int vec_server_client_handler_cmp(server_client_handler v1, server_client_handler v2) {
	if (v1.client_fd == v2.client_fd) return 0;
	if (v1.client_fd < v2.client_fd) return -1;
	return 1;
}

void vec_server_client_handler_print_server_client_handler(server_client_handler v1) {
	printf("%d", v1.client_fd);
}
