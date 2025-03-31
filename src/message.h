#ifndef MESSAGE_H
#define MESSAGE_H
#include <unistd.h>

typedef void(*msg_decoder)(char*,int);

/*
 * message
 * | type  | payloadsize | payloaddata |
 * | 00 00 | 00 00 00 00 | 00 00 00 .. |
 */
typedef struct __attribute__((packed)) message {
	short type;
	int payloadsize;
	char* payloaddata;
} message;

typedef struct __attribute__((packed)) message_hdr {
	char header_sig[3];
	short type;
	int payloadsize;
	char eot;
} message_hdr;

typedef struct {
	short type;
	int size;
	char* payload;
} message_ret;

void    		message_send(int sockfd, short type, int size, char* payloadptr);
message_ret message_read(int sockfd);

void 				message_decoder_register(int type, msg_decoder decoder);
void    		message_decode_buffer(char* buffer, int size, int type);

#endif
