#include "util.h"
#include <stdio.h>

typedef struct {
	void(*callback)(void*);
	void* dataptr;
} exit_handler;

exit_handler exit_handlers[40] = {0};
int count = 0;

void register_exit(void(*handler)(void*), void* dataptr) {
	if (count + 1 >= 40) return;
	exit_handlers[count].callback = handler;
	exit_handlers[count].dataptr = dataptr;
	count++;
	printf("Registered handler: %d at %p with %p\n", count, handler, dataptr);
}

void run_exit_handlers() {
	for (int i = 0; i < count; i++) {
		exit_handler h = exit_handlers[i];
		h.callback(h.dataptr);
	}
	printf("Exit handler\n");
}
