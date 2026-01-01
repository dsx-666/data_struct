#define _CRT_SECURE_NO_WARNINGS  1 
#include "graph.h"
#include "hash.h"
#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
int find_min_distance_by_id(HashTable* hash, int id_start, int id_end) {
	HashNode* node_start = findHashNode_by_id(hash, id_start);
	HashNode* node_end = findHashNode_by_id(hash, id_end);
	if (node_start == NULL || node_end == NULL) {
		return -1;
	}


}




