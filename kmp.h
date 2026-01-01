#pragma once
typedef struct location location;

typedef struct HashTable HashTable;

// KMP字符串匹配算法，返回子串在字符串中的位置，未找到返回-1
int kmp_find(const char* string, const char* substr);

// 通过KMP算法在哈希表中搜索包含指定子串的景点，返回匹配的景点数组

location* kmp_search_location(HashTable* hash, const char* name);



