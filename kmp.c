#define _CRT_SECURE_NO_WARNINGS  1 
#pragma once
#include<string.h>
#include<stdlib.h>
#include "hash.h"
#include "graph.h"

int kmp_find(const char* string, const char* substr) {
	int str = strlen(string);
	int sub = strlen(substr);
	int* next = (int*)malloc(sub * sizeof(int));
	if (next == NULL) {
		return -1;
	}
	next[0] = 0;
	for (int i = 1; i < sub; i++) {
		int temp = i - 1;
		while (temp >= 0) {
			temp = next[temp];
			if (substr[temp] == substr[i]) {
				break;
			}
			temp--;
		}
		next[i] = temp + 1;
	}
	int index = 0;
	for (int i = 0; i < str; i++) {
		while (index > 0 && string[i] != substr[index]) {
			index = next[index - 1];
		}
		if (string[i] == substr[index]) {
			index++;
		}
		if (index == sub) {
			free(next);
			return 1;
		}
	}
	free(next);
	return -1;
}
location* kmp_search_location(HashTable* hash, const char* name) {
	location* ans = NULL;
	int index = 0;
	int size = hash->size;
	HashNode** table = hash->table;
	for (int i = 0; i < size; i++) {
		HashNode* p = table[i];
		while (p) {
			if (kmp_find(p->data->name, name) == 1) {

				location* temp = (location*)realloc(ans, (index + 1) * sizeof(location));
				if (temp != NULL) {
					ans = temp;
					ans[index] = *(p->data);
					index++;
				}
			}
			p = p->next;
		}
	}
	return ans;
}



