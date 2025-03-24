#include "../common.h"
#include <arpa/inet.h>

int main() {
	int status, val, client_fd;
	struct sockaddr_in server_address;
	char* message = "Hello world";
	char buffer[1024];

	check((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0, "Failed to create socket");
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);

	check((inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr)) <= 0, "Address not supported");
	check((status = connect(client_fd, (struct sockaddr*)&server_address, sizeof(server_address))), "Failed to connect");

	send(client_fd, message, strlen(message), 0);

	close(client_fd);
}
