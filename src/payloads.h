#ifndef PAYLOADS_H
#define PAYLOADS_H

typedef struct {
	char buf[12];
} payload0;

typedef struct {
	int x;
	char words[100];
} payload1;

#endif
