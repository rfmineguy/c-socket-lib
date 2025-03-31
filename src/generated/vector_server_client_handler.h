#ifndef VECserver_client_handlerOR_server_client_handler_H
#define VECserver_client_handlerOR_server_client_handler_H
#include "serverdata.h"

typedef struct vec_server_client_handler {
	server_client_handler *data;
	int capacity, size;
} vec_server_client_handler;

vec_server_client_handler vec_server_client_handler_create();
void vec_server_client_handler_reserve  (vec_server_client_handler*, int);
void vec_server_client_handler_pushback (vec_server_client_handler*, server_client_handler);
void vec_server_client_handler_pushfront(vec_server_client_handler*, server_client_handler);
void vec_server_client_handler_popback  (vec_server_client_handler*);
void vec_server_client_handler_popfront (vec_server_client_handler*);
server_client_handler 	 vec_server_client_handler_peekfront(vec_server_client_handler*);
server_client_handler 	 vec_server_client_handler_peekback (vec_server_client_handler*);
void vec_server_client_handler_print    (vec_server_client_handler*);
server_client_handler*   vec_server_client_handler_get 			(vec_server_client_handler*, server_client_handler);
void vec_server_client_handler_delete 	(vec_server_client_handler*, server_client_handler);

#endif
