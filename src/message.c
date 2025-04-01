#include "message.h"
#include <arpa/inet.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include "generated/hash_table_msg_type_msg_decoder.h"

ht_msg_type_msg_decoder ht = {0};

void message_send(int sockfd, short type, int size, char* payloadptr) {
	message_hdr header = (message_hdr){.header_sig = {0x1, 0x2, 0x3}, .type = type, .payloadsize = size, .eot = 0x4};
	// 1. send message header
	ssize_t sent_header_bytes  = send(sockfd, &header, sizeof(header), 0);
	
	// 2. send message body
	ssize_t sent_message_bytes = send(sockfd, payloadptr, size, 0);
}

ssize_t recv_all(int sock, void *buffer, size_t size) {
    size_t total_received = 0;
    char *buf = buffer;

    while (total_received < size) {
        ssize_t received = recv(sock, buf + total_received, size - total_received, 0);
        if (received <= 0) {
            // Handle errors or disconnection
            perror("recv");
            return received; // Return -1 or 0 depending on the error
        }
        total_received += received;
				// printf("size: %zu, total_received: %zu\n", size, total_received);
    }
    return total_received;
}

message_ret message_read(int sockfd) {
	// read header
	message_hdr header = {0};
	ssize_t header_read, payload_read;
	while ((header_read = recv(sockfd, &header, sizeof(header), MSG_WAITALL)) == 0) {}
	char* buf = calloc(1, header.payloadsize);
	recv_all(sockfd, buf, header.payloadsize);

	return (message_ret){.type = header.type, .size = header.payloadsize, .payload = buf};
}
