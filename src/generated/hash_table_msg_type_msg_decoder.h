#ifndef HASH_TABLE_msg_type_msg_decoder_H
#define HASH_TABLE_msg_type_msg_decoder_H
#include <stdbool.h>
#include "message.h"

typedef struct ht_node_msg_type_msg_decoder {
	struct ht_node_msg_type_msg_decoder* next;
	int key;
	msg_decoder val;
} ht_node_msg_type_msg_decoder;

typedef struct ht_msg_type_msg_decoder {
	struct ht_node_msg_type_msg_decoder* buckets[10];
	int bucket_count;
} ht_msg_type_msg_decoder;

ht_msg_type_msg_decoder ht_msg_type_msg_decoder_create();
void 			 	 ht_msg_type_msg_decoder_put(ht_msg_type_msg_decoder*, int, msg_decoder);
void 			 	 ht_msg_type_msg_decoder_remove(ht_msg_type_msg_decoder*, int);
bool 				 ht_msg_type_msg_decoder_contains_key(ht_msg_type_msg_decoder*, int);
bool 				 ht_msg_type_msg_decoder_contains_val(ht_msg_type_msg_decoder*, msg_decoder);
msg_decoder* 		   ht_msg_type_msg_decoder_get(ht_msg_type_msg_decoder*, int);
void 				 ht_msg_type_msg_decoder_print(ht_msg_type_msg_decoder*);

#endif
