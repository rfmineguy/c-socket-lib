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
	printf("sent header: %lu\n", sent_header_bytes);
	
	// 2. send message body
	ssize_t sent_message_bytes = send(sockfd, payloadptr, size, 0);
	printf("sent message: %lu. '%*s'\n", sent_header_bytes, size, payloadptr);
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

void message_decoder_register(int type, void(*msgDecoder)(char*, int)) {
	if (msgDecoder == (void*)0) {
		fprintf(stderr, "Can't register null message decoder\n");
		return;
	}
	ht_msg_type_msg_decoder_put(&ht, type, msgDecoder);
}

void message_decode_buffer(char* buffer, int size, int type) {
	if (!ht_msg_type_msg_decoder_contains_key(&ht, type)) {
		fprintf(stderr, "No message decoder for type [%d]\n", type);
		return;
	}
	msg_decoder* decoder = ht_msg_type_msg_decoder_get(&ht, type);
	if (decoder) {
		(*decoder)(buffer, size);
	}
	else {
		printf("No decoder for type: %d\n", type);
	}
	// message_decoder(buffer + sizeof(short) + sizeof(int), size);
}
