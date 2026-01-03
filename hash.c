#define _CRT_SECURE_NO_WARNINGS  1 
#pragma once
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "graph.h"
#include "read_config.h"
#include <stdbool.h>
#include "hash.h"

int isPrime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0)
            return 0;
    return 1;
}

int nextPrime(int n) {
    while (!isPrime(n))
        n++;
    return n;
}

int str_hash_size = 0;
int id_hash_size = 0;

int hash_fun_id(int id, int key) {
    return id % key;
}

int hash_fun_str(const char* str, int key) {

    unsigned long long int hash_val = 0;
    const char* temp = str;
    while (*temp != '\0') {
        hash_val = (hash_val * 31 + *temp) % key; // 31用于减少冲突
        temp++; // 指针后移
    }
    return hash_val % key;
}

void rehash(HashTable* hash) {
    if (hash->count * 1.0 / hash->size > MAX_FZ) {
        int newSize = nextPrime(hash->size * 2);
        if (hash->type == 0) {
            change_config_int(FILE_NAME, "str_hash_size", newSize);
            str_hash_size = newSize;
        }
        else {
            change_config_int(FILE_NAME, "id_hash_size", newSize);
            id_hash_size = newSize;
        }
        HashNode** newTable = (HashNode**)malloc(sizeof(HashNode*) * newSize);
        if (newTable == NULL) {
            printf("哈希表自动扩容失败\n");
            return;
        }
        for (int i = 0; i < newSize; i++)
            newTable[i] = NULL;

        // 重新哈希
        for (int i = 0; i < hash->size; i++) {
            HashNode* p = hash->table[i];
            while (p) {
                HashNode* next = p->next;

                int index = 0;
                if (hash->type == 0) {
                    index = hash_fun_str(p->data->name, newSize);
                }
                else {
                    index = hash_fun_id(p->data->id, newSize);
                }


                p->next = newTable[index];
                newTable[index] = p;

                p = next;
            }
        }

        free(hash->table);
        hash->table = newTable;
        hash->size = newSize;
        printf("\n哈希表自动扩容完成，新容量：%d\n", newSize);
    }
    return;
}



HashTable* initHashTable(int type, int size) {
    HashTable* ans = (HashTable*)malloc(sizeof(HashTable));
    if (ans == NULL) {
        return NULL;
    }
    ans->count = 0;
    ans->type = type;
    ans->size = size;
    ans->table = (HashNode**)malloc(size * sizeof(HashNode*));
    if (ans->table == NULL) {
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        *(ans->table + i) = NULL;
    }
    return ans;
}

// 头插法
int insertLocation(HashTable* hash, location* loc) {
    int index = 0;
    int table_size = hash->size;
    HashNode** hashTable = hash->table;
    if (hash->type == 0) {
        index = hash_fun_str(loc->name, table_size);
    }
    else {
        index = hash_fun_id(loc->id, hash->size);
    }

    // 检查是否重复
    HashNode* p = hashTable[index];
    while (p) {
        if (p->data->id == loc->id || strcmp(p->data->name, loc->name) == 0)
            return 0;  // 已存在
        p = p->next;
    }

    HashNode* node = (HashNode*)malloc(sizeof(HashNode));
    if (node == NULL) {
        return -1;
    }
    node->data = loc;
    node->dist_length = -1;
    node->next = hashTable[index];
    node->road = NULL;
    hashTable[index] = node;

    hash->count++;
    rehash(hash);
    return 1;
}

location* findLocation_by_str(HashTable* hash, const char* name) {
    int index = 0;
    int table_size = hash->size;
    HashNode** hashTable = hash->table;
    index = hash_fun_str(name, table_size);

    HashNode* p = hashTable[index];

    while (p) {
        if (strcmp(p->data->name, name) == 0)
            return p->data;
        p = p->next;
    }
    return NULL;
}
location* findLocation_by_id(HashTable* hash, int id) {
    int index = 0;
    int table_size = hash->size;
    HashNode** hashTable = hash->table;
    index = hash_fun_id(id, table_size);

    HashNode* p = hashTable[index];

    while (p) {
        if (p->data->id == id)
            return p->data;
        p = p->next;
    }
    return NULL;
}
int deleteLocation_by_id(HashTable* hash, int id) {
    int index = 0;
    int table_size = hash->size;
    HashNode** hashTable = hash->table;
    index = hash_fun_id(id, hash->size);
    HashNode* p = hashTable[index], * pre = NULL;

    while (p) {
        if (p->data->id == id) {
            if (pre)
                pre->next = p->next;
            else
                hashTable[index] = p->next;
            if (p->road) {
                Road_Link* p_road = p->road->next;
                while (p_road) {
                    eraseRoad_Link(hash, p->data->id, p_road->id);
                    p_road = p_road->next;
                }
            }
            free(p);
            hash->count--;
            return 1;
        }
        pre = p;
        p = p->next;
    }

    return 0;
}

int deleteLocation_by_char(HashTable* hash, const char* name) {
    int index = 0;
    int table_size = hash->size;
    HashNode** hashTable = hash->table;
    index = hash_fun_str(name, hash->size);
    HashNode* p = hashTable[index], * pre = NULL;

    while (p) {
        if (strcmp(p->data->name, name) == 0) {
            if (pre)
                pre->next = p->next;
            else
                hashTable[index] = p->next;
            
            hash->count--;
            return 1;
        }
        pre = p;
        p = p->next;
    }
    return 0;
}
int updateLocation(HashTable* table, location* loc) {
    location* p = findLocation_by_id(table, loc->id);
    if (p == NULL) {
        return -1;
    }
    strcpy(p->name, loc->name);
    strcpy(p->des, loc->des);
    
    return 1;
}

int showAllLocations(HashTable* hash,int boundry) {
    printf("\n==================== 所有景点信息 ====================\n");
    int count = 0;
    for (int i = 0; i < hash->size; i++) {
        HashNode* p = hash->table[i];
        while (p) {
            count++;
            if (count >= boundry && count <= boundry + 5) {
                printf("ID:%d\n名称:%s\n简介:%s\n\n",
                    p->data->id, p->data->name, p->data->des);
            }
            p = p->next;
        }
  
    }
    if (count == 0) {
        printf("当前无数据\n");
        return 0;
    }
	else if (count < boundry) {
		printf("该页无数据\n");
	}

    printf("======================================================\n");
    return count;
}

int show_Locations_by_ID(HashTable* hash, int id) {

    int index = hash_fun_id(id, hash->size);
    HashNode* p = hash->table[index];
    while (p) {
        if (p->data->id == id) {
            printf("ID:%d\n名称:%s\n简介:%s\n\n", p->data->id, p->data->name, p->data->des);
            return 1;
        }
        p = p->next;
    }

    return 0;
}
HashNode* findHashNode_by_str(HashTable* hash, const char* name) {
    int index = 0;
    int table_size = hash->size;
    HashNode** hashTable = hash->table;
    index = hash_fun_str(name, table_size);

    HashNode* p = hashTable[index];

    while (p) {
        if (strcmp(p->data->name, name) == 0)
            return p;
        p = p->next;
    }
    return NULL;
}
HashNode* findHashNode_by_id(HashTable* hash, int id) {
    int index = 0;
    int table_size = hash->size;
    HashNode** hashTable = hash->table;
    index = hash_fun_id(id, table_size);

    HashNode* p = hashTable[index];

    while (p) {
        if (p->data->id == id)
            return p;
        p = p->next;
    }
    return NULL;
}
int findRoad_Link(HashTable* hash, int from_id, int to_id) {
    HashNode* from_node = findHashNode_by_id(hash, from_id);
	HashNode* to_node = findHashNode_by_id(hash, to_id);
	if (from_node == NULL || to_node == NULL) {
		return -1; // 节点不存在
	}

	// 查找from_node到to_node的链接
	Road_Link* p = from_node->road;
	while (p) {
		if (p->id == to_id) {
			return 1; // 链接存在
		}
		p = p->next;
	}

	return -2; // 道路不存在


}
int insertRoad_Link(HashTable* hash, int from_id, int to_id, int length) {
	HashNode* from_node = findHashNode_by_id(hash, from_id);
	HashNode* to_node = findHashNode_by_id(hash, to_id);
	if (from_node == NULL || to_node == NULL) {
		return -1; // 节点不存在
	}
    if(findRoad_Link(hash, from_id, to_id) == 1) {
		return -4; // 道路已存在
	}
	// 创建新的道路链接
	Road_Link* new_link = (Road_Link*)malloc(sizeof(Road_Link));
	if (new_link == NULL) {
		return -3; // 内存分配失败
	}
	new_link->next = to_node->road;
	new_link->length = length;
    to_node->road = new_link;
    new_link->id = from_node->data->id;
	// 由于是无向图，继续添加链接
	Road_Link* reverse_link = (Road_Link*)malloc(sizeof(Road_Link));
	if (reverse_link == NULL) {
		return -3; // 内存分配失败
	}
	reverse_link->next = from_node->road;
	reverse_link->length = length;
	from_node->road = reverse_link;
    reverse_link->id = to_node->data->id;

	return 1; // 插入成功
}

int eraseRoad_Link(HashTable* hash, int from_id, int to_id) {
    HashNode* from_node = findHashNode_by_id(hash, from_id);
	HashNode* to_node = findHashNode_by_id(hash, to_id);
	if (from_node == NULL || to_node == NULL) {
		return -1; // 节点不存在
	}
    int one = 0;
    int two = 0;
	// 删除from_node到to_node的链接
	Road_Link* p = from_node->road;
	Road_Link* pre = NULL;
	while (p) {
		if (p->id == to_id) {
            one = 1;
			if (pre)
				pre->next = p->next;
			else
				from_node->road = p->next;
			free(p);
			break;
		}
		pre = p;
		p = p->next;
	}
    
	// 删除to_node到from_node的链接
	p = to_node->road;
	pre = NULL;
	while (p) {
		if (p->id == from_id) {
            two = 1;
			if (pre)
				pre->next = p->next;
			else
				to_node->road = p->next;
			free(p);
			break;
		}
		pre = p;
		p = p->next;
	}
    if (one == 0 && two == 0) {
        return -3;
    }
	return 1; // 删除成功
}

void freeHashTable(HashTable* hash) {
    if (hash == NULL) { 
        return;
    }

    // 遍历哈希表的所有桶
    for (int i = 0; i < hash->size; i++) {
        HashNode* p = hash->table[i]; // 当前桶的头节点
        while (p != NULL) {
            HashNode* temp = p; // 临时保存当前节点，用于释放
            p = p->next;        // 先走到下一个节点，避免释放后断链

            // 释放当前哈希节点挂载的邻接边链表
            if (temp->road != NULL) {
                Road_Link* road_p = temp->road;
                while (road_p != NULL) {
                    Road_Link* road_temp = road_p;
                    road_p = road_p->next;
                    free(road_temp); // 释放每一条邻接边
                    road_temp = NULL;
                }
                temp->road = NULL; // 置空
            }

            // 删掉无效的 temp->data 判断

            // 释放哈希节点本身
            free(temp);
            temp = NULL;
        }
        hash->table[i] = NULL; // 桶置空
    }

    // 释放哈希表的桶数组空间
    if (hash->table != NULL) {
        free(hash->table);
        hash->table = NULL;
    }

    // 释放哈希表结构体本体
    free(hash);
}

int insertDistHash(HashTable* hash, location* loc, int length) {
    int index = 0;
    int table_size = hash->size;
    HashNode** hashTable = hash->table;
    if (hash->type == 0) {
        index = hash_fun_str(loc->name, table_size);
    }
    else {
        index = hash_fun_id(loc->id, hash->size);
    }

    // 检查是否重复
    HashNode* p = hashTable[index];
    while (p) {
        if (p->data->id == loc->id || strcmp(p->data->name, loc->name) == 0)
            return 0;  // 已存在
        p = p->next;
    }

    HashNode* node = (HashNode*)malloc(sizeof(HashNode));
    if (node == NULL) {
        return -1;
    }
    node->data = loc;
    node->dist_length = length;
    node->next = hashTable[index];
    node->road = NULL;
    hashTable[index] = node;

    hash->count++;
    rehash(hash);
    return 1;
}