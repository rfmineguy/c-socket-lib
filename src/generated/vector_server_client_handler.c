#include "vector_server_client_handler.h"
#include "vector_server_client_handler_spec.c"
#include <assert.h>
#include <stdlib.h>

void vec_server_client_handler_reserve_if_needed(vec_server_client_handler* arr, int capacity) {
	if (arr->size + 1 >= arr->capacity) {
		if (!arr->data) {
			arr->data = calloc(capacity, sizeof(server_client_handler));
		}
		else {
			arr->data = realloc(arr->data, capacity);
		}
		arr->capacity = capacity;
		printf("Realloced: newcap: %d\n", capacity);
	}
}

vec_server_client_handler vec_server_client_handler_create() {
	vec_server_client_handler dyn_array = {.data = 0, .capacity = 10, .size = 0};
	dyn_array.data = calloc(dyn_array.capacity, sizeof(server_client_handler));
	return dyn_array;
}

void vec_server_client_handler_pushback (vec_server_client_handler* arr, server_client_handler val) {
	vec_server_client_handler_reserve_if_needed(arr, arr->capacity * 2);
	assert(arr->data != (void*)0 && "Can't pushback, data is null");
	arr->data[arr->size++] = val;
}

void vec_server_client_handler_pushfront(vec_server_client_handler* arr, server_client_handler val) {
	vec_server_client_handler_reserve_if_needed(arr, arr->capacity * 2);
	assert(arr->data != (void*)0 && "Can't pushfront, data is null");
	assert(arr->size + 1 <= arr->capacity && "Not enough space to pushfront");
	for (int i = 0; i < arr->size; i++) {
		arr->data[i + 1] = arr->data[i];
	}
	arr->data[0] = val;
	arr->size++;
}

void vec_server_client_handler_popback(vec_server_client_handler* arr) {
	for (int i = 0; i < arr->size; i++) {
		arr->data[i] = arr->data[i + 1];
	}
	arr->size--;
}

void vec_server_client_handler_popfront (vec_server_client_handler* arr) {
	for (int i = 1; i < arr->size; i++) {
		arr->data[i - 1] = arr->data[i];
	}
	arr->size--;
}

server_client_handler vec_server_client_handler_peekfront(vec_server_client_handler* arr) {
	assert(arr->size == 0 && "Dynamic array size 0. Can't peekfront.");
	return arr->data[0];
}

server_client_handler vec_server_client_handler_peekback(vec_server_client_handler* arr) {
	assert(arr->size == 0 && "Dynamic array size 0. Can't peekfront.");
	return arr->data[arr->size - 1];
}

void vec_server_client_handler_print(vec_server_client_handler* arr) {
	printf("vec_server_client_handler : [");
	for (int i = 0; i < arr->size; i++) {
		vec_server_client_handler_print_server_client_handler(arr->data[i]);
		if (i != arr->size - 1)	printf(", ");
	}
	printf("]");
}

server_client_handler* vec_server_client_handler_get(vec_server_client_handler* arr, server_client_handler v) {
	for (int i = 0; i < arr->size; i++) {
		if (vec_server_client_handler_cmp(arr->data[i], v) == 0) {
			return &arr->data[i];
		}
	}
	return (void*)0;
}

void vec_server_client_handler_delete(vec_server_client_handler* arr, server_client_handler v) {
	for (int i = 0; i < arr->size; i++) {
		if (vec_server_client_handler_cmp(arr->data[i], v) == 0) {
		}
	}
}
