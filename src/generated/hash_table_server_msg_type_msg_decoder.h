#ifndef HASH_TABLE_server_msg_type_msg_decoder_H
#define HASH_TABLE_server_msg_type_msg_decoder_H
#include <stdbool.h>
#include "message.h"

typedef struct ht_node_server_msg_type_msg_decoder {
	struct ht_node_server_msg_type_msg_decoder* next;
	int key;
	server_msg_decoder val;
} ht_node_server_msg_type_msg_decoder;

typedef struct ht_server_msg_type_msg_decoder {
	struct ht_node_server_msg_type_msg_decoder* buckets[10];
	int bucket_count;
} ht_server_msg_type_msg_decoder;

ht_server_msg_type_msg_decoder ht_server_msg_type_msg_decoder_create();
void 			 	 ht_server_msg_type_msg_decoder_put(ht_server_msg_type_msg_decoder*, int, server_msg_decoder);
void 			 	 ht_server_msg_type_msg_decoder_remove(ht_server_msg_type_msg_decoder*, int);
bool 				 ht_server_msg_type_msg_decoder_contains_key(ht_server_msg_type_msg_decoder*, int);
bool 				 ht_server_msg_type_msg_decoder_contains_val(ht_server_msg_type_msg_decoder*, server_msg_decoder);
server_msg_decoder* 		   ht_server_msg_type_msg_decoder_get(ht_server_msg_type_msg_decoder*, int);
void 				 ht_server_msg_type_msg_decoder_print(ht_server_msg_type_msg_decoder*);

#endif
