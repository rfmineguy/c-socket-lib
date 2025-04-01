#include "message.h"
#include "hash_table_client_msg_type_msg_decoder.h"
#include <stdio.h>

int ht_client_msg_type_msg_decoder_hash(int a, int max) {
	int hash = a % max;
	return a % max;
}

int ht_client_msg_type_msg_decoder_compare_key(int a, int b) {
	if (a < b) return -1;
	if (a > b) return 1;
	return 0;
}

int ht_client_msg_type_msg_decoder_compare_val(client_msg_decoder a, client_msg_decoder b) {
	if (a < b) return -1;
	if (a > b) return 1;
	return 0;
}

void ht_client_msg_type_msg_decoder_node_print(struct ht_node_client_msg_type_msg_decoder* n) {
	printf("%p", n->val);
}
