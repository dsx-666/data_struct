#pragma once
#define MAX_FZ 0.75
// 哈希表

typedef struct location location;

typedef struct Road_Link Road_Link;

typedef struct HashNode {
	location* data;   // 景点信息
    struct HashNode* next;  // 链表指针（解决冲突）
    Road_Link* road; // 路链表存放路径信息
    int dist_length; // 迪杰斯特拉算法中存放距离或者prev节点ID
} HashNode;

typedef struct {
    HashNode** table;   // 桶数组
    int size;           // 表长
    int count;          // 元素个数
    int type;           // 判断否是字符串哈希(0)还是数组哈希(1)
} HashTable;

// 判断是否是素数
int isPrime(int n);
// 生成新哈希表的下一个素数
int nextPrime(int n);
// 字符串哈希表大小
extern int str_hash_size;
// ID哈希表大小
extern int id_hash_size;
// ID哈希函数
int hash_fun_id(int id, int key);
// 字符串哈希函数
int hash_fun_str(const char* str, int key);
// 哈希表自动扩容
void rehash(HashTable* hash);
// 初始化哈希表
HashTable* initHashTable(int type, int size);
// 头插法,插入风景信息
int insertLocation(HashTable* hash, location* loc);
// 通过字符串查找风景
location* findLocation_by_str(HashTable* hash, const char* name);
// 通过ID查找风景
location* findLocation_by_id(HashTable* hash, int id);
// 通过ID删除风景
int deleteLocation_by_id(HashTable* hash, int id);
// 通过字符串删除风景
int deleteLocation_by_char(HashTable* hash, const char* name);
// 通过ID删除风景
int updateLocation(HashTable* table, location* loc);
// 展示全部风景信息
int showAllLocations(HashTable* hash,int boundry);
// 通过名称展示风景信息
int show_Locations_by_ID(HashTable* hash, int id);
// 通过字符串查找哈希节点
HashNode* findHashNode_by_str(HashTable* hash, const char* name);
// 通过ID查找哈希节点
HashNode* findHashNode_by_id(HashTable* hash, int id);
// 插入道路链接，是无向网,成功返回1，否则返回-1
int insertRoad_Link(HashTable* hash, int from_id, int to_id, int length);
// 删除道路链接，是无向网，成功返回1，否则返回-1
int eraseRoad_Link(HashTable* hash, int from_id, int to_id);
// 寻找路径是否存在，存在返回1，ID风景不存在返回-1，不存在-2
int findRoad_Link(HashTable* hash, int from_id, int to_id);
// 释放哈希表内存
void freeHashTable(HashTable* hash);
// 插入HashNode(只对迪杰斯特拉的dist有用)
int insertDistHash(HashTable* hash, location* loc,int length);
