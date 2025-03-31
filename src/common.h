#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#define PORT 8080
#define check(expr, errmessage) \
	do {\
		int v = expr;\
		if (v) {\
			fprintf(stderr, "Error(line=%d)[%d][%s]: %s\n", __LINE__, errno, strerror(errno), errmessage);\
			exit(1);\
		}\
	} while (0);

