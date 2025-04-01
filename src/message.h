#ifndef MESSAGE_H
#define MESSAGE_H
#include <unistd.h>

struct server;
struct client;
typedef void(*client_msg_decoder)(struct client*, int, char*,int);
typedef void(*server_msg_decoder)(struct server*, int, char*,int);

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

#endif
