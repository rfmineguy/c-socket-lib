#ifndef PAYLOADS_H
#define PAYLOADS_H

typedef enum {
	payload_type0, payload_type1
} payload_type;

typedef struct {
	char buf[12];
} payload0;

typedef struct {
	int x;
	char words[100];
} payload1;

#endif
