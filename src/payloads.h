#ifndef PAYLOADS_H
#define PAYLOADS_H

typedef enum {
	payload_type0, payload_type1,

	pt_login_request, pt_login_response,
} payload_type;

typedef struct {
	char buf[12];
} payload0;

typedef struct {
	int x;
	char words[100];
} payload1;

typedef struct {
	char username[100], password[100];
} payload_login_request;

typedef struct {
	int status;
} payload_login_response;

#endif
