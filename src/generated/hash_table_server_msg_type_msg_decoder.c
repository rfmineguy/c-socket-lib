#include "hash_table_server_msg_type_msg_decoder.h"
#include "hash_table_server_msg_type_msg_decoder_spec.c"
#include <stdlib.h>
#include <stdio.h>

/**
 *    User is required to implement a few select functions in their own spec file
 *    	- ht_server_msg_type_msg_decoder_compare_key(KEY, KEY)
 *    		+ return 0 on equal
 *    		+ return -1 on less
 *    		+ return 1 on greater
 *    	- ht_server_msg_type_msg_decoder_compare_val(VAL, VAL)
 *    		+ return 0 on equal
 *    		+ return -1 on less
 *    		+ return 1 on greater
 *    	- ht_server_msg_type_msg_decoder_node_print(VAL)
 *    		+ you decide how to print the node data
 *    	- ht_server_msg_type_msg_decoder_hash(KEY)
 *    		+ you decide how to generate a hash for the key
 */

ht_server_msg_type_msg_decoder ht_server_msg_type_msg_decoder_create() {
	ht_server_msg_type_msg_decoder ht = {0};
	ht.bucket_count = 10;
	return ht;
}
void ht_server_msg_type_msg_decoder_put(ht_server_msg_type_msg_decoder* ht, int key, server_msg_decoder val) {
	// generate hash and node
	int hash = ht_server_msg_type_msg_decoder_hash(key, 10);
	ht_node_server_msg_type_msg_decoder* node = calloc(1, sizeof(ht_node_server_msg_type_msg_decoder));
	node->key = key;
	node->val = val;

	// put the value into the right bucket
	if (ht->buckets[hash] == (void*)0) {
		ht->buckets[hash] = node;
		return;
	}
	else {
		ht_node_server_msg_type_msg_decoder* t = ht->buckets[hash];
		ht->buckets[hash] = node;
		node->next = t;
		return;
	}
}
void ht_server_msg_type_msg_decoder_remove(ht_server_msg_type_msg_decoder* ht, int key) {
	int hash = ht_server_msg_type_msg_decoder_hash(key, 10);
	if (ht->buckets[hash] == (void*)0) return;
	ht_node_server_msg_type_msg_decoder* prev = 0;
	ht_node_server_msg_type_msg_decoder* t = ht->buckets[hash];
	while (t && ht_server_msg_type_msg_decoder_compare_key(key, t->key) != 0) {
		prev = t;
		t = t->next;
	}
	if (t && prev) {
		prev->next = t->next;
		free(t);
	}
	if (t && !prev) {
		ht->buckets[hash] = t->next;
		free(t);
	}
}
bool ht_server_msg_type_msg_decoder_contains_key(ht_server_msg_type_msg_decoder* ht, int key) {
	int hash = ht_server_msg_type_msg_decoder_hash(key, 10);
	if (ht->buckets[hash] == (void*)0) return false;
	ht_node_server_msg_type_msg_decoder* n = ht->buckets[hash];
	while (n) {
		if (ht_server_msg_type_msg_decoder_compare_key(n->key, key) == 0) {
			return true;
		}
		n = n->next;
	}
	return false;
}
bool ht_server_msg_type_msg_decoder_contains_val(ht_server_msg_type_msg_decoder* ht, server_msg_decoder val) {
	for (int i = 0; i < 10; i++) {
		if (!ht->buckets[i]) continue;
		ht_node_server_msg_type_msg_decoder* n = ht->buckets[i];
		while (n) {
			if (ht_server_msg_type_msg_decoder_compare_val(n->val, val) == 0) {
				return true;
			}
			n = n->next;
		}
	}
	return false;
}
server_msg_decoder* ht_server_msg_type_msg_decoder_get(ht_server_msg_type_msg_decoder* ht, int key) {
	int hash = ht_server_msg_type_msg_decoder_hash(key, 10);
	if (ht->buckets[hash] == (void*)0) return false;
	ht_node_server_msg_type_msg_decoder* n = ht->buckets[hash];
	while (n) {
		if (ht_server_msg_type_msg_decoder_compare_key(n->key, key) == 0) {
			return &n->val;
		}
		n = n->next;
	}
	return (void*)0;
}
void ht_server_msg_type_msg_decoder_print(ht_server_msg_type_msg_decoder* ht) {
	for (int i = 0; i < 10; i++) {
		if (!ht->buckets[i]) continue;
		ht_node_server_msg_type_msg_decoder* n = ht->buckets[i];
		printf("%d  ::  ", i);
		while (n) {
			ht_server_msg_type_msg_decoder_node_print(n); printf(" -> ");
			n = n->next;
		}
		printf("\n");
	}
}
