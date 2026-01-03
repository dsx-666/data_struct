#define _CRT_SECURE_NO_WARNINGS  1 
#include "graph.h"
#include "hash.h"
#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include "Dijkstra.h"
#include <limits.h> 
#include <windows.h>
#include <string.h>

extern HashTable* hash_id;
extern HashTable* hash_vis;
extern int id_hash_size;
extern int* shortest_path;
void clearInputBuffer();

void dijkstra_shortest_path()
{
	printf("\n\n");
	printf("================== 查询两景点之间的最短路径（暴力Dijkstra） ==================\n");
	printf("\n");
	int startId, endId;
	printf("请输入起点ID:");
	scanf("%d", &startId);
	printf("\n");
	printf("请输入终点ID:");
	scanf("%d", &endId);
	printf("\n");
	if (!findLocation_by_id(hash_id, startId) || !findLocation_by_id(hash_id, endId)) {
		printf("输入的id不正确，无法找到对应景点\n");
		clearInputBuffer();
		Sleep(1000);
		return;
	}
	if (startId == endId) {
		printf("起点和终点不能相等\n");
		clearInputBuffer();
		Sleep(1000);
		return;
	}


	hash_vis = initHashTable(1, id_hash_size);

	//这个是用来记录查找的起点到每个对应id最短的距离
	HashTable* dist = initHashTable(1, id_hash_size);
	//这个是用来记录最短路径上每个节点的上一个节点的id，注意这个dist_length存放的是上一个节点的id
	HashTable* prev = initHashTable(1, id_hash_size);
	int* roads = (int*)malloc((hash_id->count + 1) * sizeof(int));
	int index = 0;
	int* path = (int*)malloc((hash_id->count + 1) * sizeof(int));
	shortest_path = (int*)malloc((hash_id->count + 1) * sizeof(int));
	if (dist == NULL || prev == NULL || hash_vis == NULL || path == NULL || roads == NULL) {
		printf("内存分配出问题\n");
		freeHashTable(dist);
		freeHashTable(prev);
		freeHashTable(hash_vis);
		free(path);
		free(roads);
		return;
	}
	location* news = malloc(sizeof(location));
	*(news) = *findLocation_by_id(hash_id, startId);
	insertDistHash(dist, news, 0);
	roads[index] = findLocation_by_id(hash_id, startId)->id;
	index++;
	location* temp_prev = (location*)malloc(sizeof(location));
	*temp_prev = *findLocation_by_id(hash_id, startId);
	insertDistHash(prev, temp_prev, -1);
	//现在最短路径节点数
	int now_count = 0;
	while (now_count < dist->count) {
		//离起点最近的节点id
		int clo_id = -1;
		int min_dist = INT_MAX;
		for (int i = 0; i < index; i++) {
			int curr_id = roads[i];
			int curr_dist = findHashNode_by_id(dist, curr_id)->dist_length;
			if (findLocation_by_id(hash_vis, curr_id) == NULL && curr_dist < min_dist) {
				min_dist = curr_dist;
				clo_id = curr_id;
			}
		}
		
		location* temp_loc = (location*)malloc(sizeof(location));
		*temp_loc = *findLocation_by_id(hash_id, clo_id);
		insertLocation(hash_vis, temp_loc);
		if (clo_id == -1 || clo_id == endId) break;
		Road_Link* p = findHashNode_by_id(hash_id, clo_id)->road;
		while (p != NULL) {
			int v = p->id;
			int len = p->length;
			int dist_clo = findHashNode_by_id(dist, clo_id)->dist_length;
			if (findHashNode_by_id(dist, v) == NULL || findHashNode_by_id(dist, v)->dist_length > dist_clo + len) {
				if (findHashNode_by_id(dist, v) == NULL) {
					location* temp_v = (location*)malloc(sizeof(location));
					*temp_v = *findLocation_by_id(hash_id, v);
					insertDistHash(dist, temp_v, dist_clo + len);
					roads[index++] = v;
				}
				else {
					HashNode* node_v = findHashNode_by_id(dist, v);
					node_v->dist_length = dist_clo + len;
				}
				if (findHashNode_by_id(prev, v) == NULL) {
					location* temp_prev = (location*)malloc(sizeof(location));
					*temp_prev = *findLocation_by_id(hash_id, v);
					insertDistHash(prev, temp_prev, clo_id);
				}
				else {
					HashNode* node_prev = findHashNode_by_id(prev, v);
					node_prev->dist_length = clo_id;


				}
			}
			p = p->next;
		}
		now_count++;
	}
	if (findHashNode_by_id(dist, endId) == NULL) {
		printf("没有可达路径\n");
		Sleep(1000);
	}
	else {
		int path_len = 0;
		for (int curr = endId; curr != -1; curr = findHashNode_by_id(prev, curr)->dist_length) {
			path[path_len++] = curr;
		}
		printf("从 %s 到 %s 的最短路径为：\n",
			findLocation_by_id(hash_id, startId)->name, findLocation_by_id(hash_id, endId)->name);
		for (int i = path_len - 1; i >= 0; i--) {
			int curr_id = path[i];
			location* curr_loc = findLocation_by_id(hash_id, curr_id);
			printf("%s(%d)", curr_loc->name, curr_loc->id);
			if (i != 0) printf("->");
		}
		printf("  总长度：%d m\n", findHashNode_by_id(dist, endId)->dist_length);
		memcpy(shortest_path, path, path_len * sizeof(int));
	}
	freeHashTable(dist);
	freeHashTable(prev);
	free(path);
	free(roads);
	freeHashTable(hash_vis);
	hash_vis = NULL;
	clearInputBuffer();
	Sleep(1000);
}



void dijkstra_shortest_path_heap()
{
	printf("\n\n");
	printf("================== 查询两景点之间的最短路径（堆优化Dijkstra） ==================\n");
	printf("\n");
	int startId, endId;
	printf("请输入起点ID:");
	scanf("%d", &startId);
	printf("\n");
	printf("请输入终点ID:");
	scanf("%d", &endId);
	printf("\n");
	if (!findLocation_by_id(hash_id, startId) || !findLocation_by_id(hash_id, endId)) {
		printf("输入的id不正确，无法找到对应景点\n");
		clearInputBuffer();
		Sleep(1000);
		return;
	}
	if (startId == endId) {
		printf("起点和终点不能相等\n");
		clearInputBuffer();
		Sleep(1000);
		return;
	}


	hash_vis = initHashTable(1, id_hash_size);

	//这个是用来记录查找的起点到每个对应id最短的距离
	HashTable* dist = initHashTable(1, id_hash_size);
	//这个是用来记录最短路径上每个节点的上一个节点的id，注意这个dist_length存放的是上一个节点的id
	HashTable* prev = initHashTable(1, id_hash_size);

	int index = 0;
	int* path = (int*)malloc((hash_id->count + 1) * sizeof(int));
	shortest_path = (int*)malloc((hash_id->count + 1) * sizeof(int));
	if (dist == NULL || prev == NULL || hash_vis == NULL || path == NULL ) {
		printf("内存分配出问题\n");
		freeHashTable(dist);
		freeHashTable(prev);
		freeHashTable(hash_vis);
		free(path);
		return;
	}
	location* news = malloc(sizeof(location));
	*(news) = *findLocation_by_id(hash_id, startId);
	insertDistHash(dist, news, 0);
	
	location* temp_prev = (location*)malloc(sizeof(location));
	*temp_prev = *findLocation_by_id(hash_id, startId);
	insertDistHash(prev, temp_prev, -1);
	//现在最短路径节点数
	Value_Array* ds_heap = init_value_array();
	Value* start_value = (Value*)malloc(sizeof(Value));
	
	start_value->value = 0;
	start_value->id = news->id;
	insert_heap(ds_heap, start_value);
	while (!empty_value_array(ds_heap)) {
		//离起点最近的节点id
		Value* temp_value = (Value*)malloc(sizeof(Value));

		erase_heap(ds_heap, temp_value);
		int clo_id = temp_value->id;
		if (findLocation_by_id(hash_vis, clo_id) != NULL) {

			free(temp_value);
			continue;
		}

		int min_dist = temp_value->value;

		location* temp_loc = (location*)malloc(sizeof(location));

		*temp_loc = *findLocation_by_id(hash_id, clo_id);
		insertLocation(hash_vis, temp_loc);

		if (clo_id == -1 || clo_id == endId) break;
		Road_Link* p = findHashNode_by_id(hash_id, clo_id)->road;
		while (p != NULL) {
			int v = p->id;
			int len = p->length;
			int dist_clo = findHashNode_by_id(dist, clo_id)->dist_length;
			if (findHashNode_by_id(dist, v) == NULL || findHashNode_by_id(dist, v)->dist_length > dist_clo + len) {
				if (findHashNode_by_id(dist, v) == NULL) {
					location* temp_v = (location*)malloc(sizeof(location));
					*temp_v = *findLocation_by_id(hash_id, v);
					insertDistHash(dist, temp_v, dist_clo + len);
					
				}
				else {
					HashNode* node_v = findHashNode_by_id(dist, v);
					node_v->dist_length = dist_clo + len;
				}
				Value* new_value = (Value*)malloc(sizeof(Value));
				new_value->id = findLocation_by_id(hash_id, v)->id;

				new_value->value = dist_clo + len;
				insert_heap(ds_heap, new_value);

				if (findHashNode_by_id(prev, v) == NULL) {
					location* temp_prev = (location*)malloc(sizeof(location));
					*temp_prev = *findLocation_by_id(hash_id, v);
					insertDistHash(prev, temp_prev, clo_id);
				}
				else {
					HashNode* node_prev = findHashNode_by_id(prev, v);
					node_prev->dist_length = clo_id;
				}
			}
			p = p->next;
		}
		free(temp_value);
	}
	if (findHashNode_by_id(dist, endId) == NULL) {
		printf("没有可达路径\n");
		Sleep(1000);
	}
	else {
		int path_len = 0;
		for (int curr = endId; curr != -1; curr = findHashNode_by_id(prev, curr)->dist_length) {
			path[path_len++] = curr;
		}
		printf("从 %s 到 %s 的最短路径为：\n",
			findLocation_by_id(hash_id, startId)->name, findLocation_by_id(hash_id, endId)->name);
		for (int i = path_len - 1; i >= 0; i--) {
			int curr_id = path[i];
			location* curr_loc = findLocation_by_id(hash_id, curr_id);
			printf("%s(%d)", curr_loc->name, curr_loc->id);
			if (i != 0) printf("->");
		}
		printf("  总长度：%d m\n", findHashNode_by_id(dist, endId)->dist_length);
		memcpy(shortest_path, path, path_len * sizeof(int));
	}
	freeHashTable(dist);
	freeHashTable(prev);
	free(path);

	freeHashTable(hash_vis);
	
	hash_vis = NULL;
	clearInputBuffer();
	Sleep(1000);
}
