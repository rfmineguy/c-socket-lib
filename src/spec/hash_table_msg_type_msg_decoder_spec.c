#include "message.h"
#include "hash_table_msg_type_msg_decoder.h"
#include <stdio.h>

int ht_msg_type_msg_decoder_compare_key(int a, int b) {
	if (a < b) return -1;
	if (a > b) return 1;
	return 0;
}

int ht_msg_type_msg_decoder_compare_val(msg_decoder a, msg_decoder b) {
	if (a < b) return -1;
	if (a > b) return 1;
	return 0;
}

void ht_msg_type_msg_decoder_node_print(struct ht_node_msg_type_msg_decoder* n) {
	printf("%p", n->val);
}

int ht_msg_type_msg_decoder_hash(int a, int max) {
	int hash = a % max;
	return a % max;
}
