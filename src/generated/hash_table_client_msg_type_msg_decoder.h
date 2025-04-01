#ifndef HASH_TABLE_client_msg_type_msg_decoder_H
#define HASH_TABLE_client_msg_type_msg_decoder_H
#include <stdbool.h>
#include "message.h"

typedef struct ht_node_client_msg_type_msg_decoder {
	struct ht_node_client_msg_type_msg_decoder* next;
	int key;
	client_msg_decoder val;
} ht_node_client_msg_type_msg_decoder;

typedef struct ht_client_msg_type_msg_decoder {
	struct ht_node_client_msg_type_msg_decoder* buckets[10];
	int bucket_count;
} ht_client_msg_type_msg_decoder;

ht_client_msg_type_msg_decoder ht_client_msg_type_msg_decoder_create();
void 			 	 ht_client_msg_type_msg_decoder_put(ht_client_msg_type_msg_decoder*, int, client_msg_decoder);
void 			 	 ht_client_msg_type_msg_decoder_remove(ht_client_msg_type_msg_decoder*, int);
bool 				 ht_client_msg_type_msg_decoder_contains_key(ht_client_msg_type_msg_decoder*, int);
bool 				 ht_client_msg_type_msg_decoder_contains_val(ht_client_msg_type_msg_decoder*, client_msg_decoder);
client_msg_decoder* 		   ht_client_msg_type_msg_decoder_get(ht_client_msg_type_msg_decoder*, int);
void 				 ht_client_msg_type_msg_decoder_print(ht_client_msg_type_msg_decoder*);

#endif
