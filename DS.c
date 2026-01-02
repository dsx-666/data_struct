#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"graph.h"
#include"hash.h"
#include"heap.h"
#include"read_config.h"
#include<stdbool.h>
#include<windows.h>
void input_int(int* input);
void input_char(char* c);
//校园导游程序的菜单面板
void menu() {
	printf("-------------------------------------------------------------\n");
	printf("                      校园导游程序                           \n");
	printf("0：退出                                                      \n");
	printf("1：查询某个景点的基本信息                                    \n");
	printf("2：查询任意两个景点之间的最短路径                            \n");
	printf("3：查询任意两个景点的所有路径                                \n");
	printf("4：添加景点基本信息                                          \n");
	printf("5：删除某景点和其对应的路径信息                              \n");
	printf("6：更新某景点的基本信息                                      \n");
	printf("7：添加两个景点的路径信息                                    \n");
	printf("8：删除两个景点间的路径信息                                  \n");
	printf("9：更新两个对应景点的路径信息                                \n");
	printf("10：查看所有景点和其相关信息                                 \n");
	printf("11：显示所有路径信息                                         \n");
	printf("-------------------------------------------------------------\n");
	printf("请输入你的选择：");
}

//全局组件，各个方法都可能用到
HashTable* hash_str = NULL;
HashTable* hash_id = NULL;
HashTable* hash_vis = NULL;
int str_hash_size;
int id_hash_size;



//初始化所有要用到的组件
int init_all_part()
{
	//从read_config.h读取哈希表初始大小
	
	read_config_int(FILE_NAME, "str_hash_size", &str_hash_size);
	read_config_int(FILE_NAME, "id_hash_size", &id_hash_size);
	//开始去初始化哈希表
	hash_str = initHashTable(0, str_hash_size);
	hash_id = initHashTable(1, id_hash_size);
	if (hash_str == NULL || hash_id == NULL) {
		printf("hash表初始化不成功\n");
		return -1;
	}
	
	return 1;
}



void clearInputBuffer() {
	while (getchar() != '\n'); 
}

//查询某个景点的基本信息
void find_location()
{
	printf("\n\n");
	printf("====================== 查询景点基本信息 ======================\n");
	printf("\n");
	printf("1. 按ID查询  2. 按名称查询 0. 退出查询\n请选择：");
	int choice;
	location* loc = NULL;
	scanf("%d", &choice);
	//检验用户输入的选项是否正确
	while (true) {
		if (choice == 1 || choice == 2) {
			break;
		}
		else if (choice == 0) {
			return;
		}
		else {
			input_int(&choice);
		}
	}
	if (choice == 1) {
		printf("\n");
		printf("请输入景点id:");
		
		int id;
		scanf("%d", &id);
		if (id == 0) {
			return;
		}
		loc = findLocation_by_id(hash_id, id);
		while (loc == NULL) {
			input_int(&id);
			if (id == 0) {
				return;
			}
			loc = findLocation_by_id(hash_id, id);
		}
	}
	else {
		printf("\n");
		printf("请输入景点名称:");
		
		char name[NAME_MAX];
		scanf("%s", name);
		if (strcmp(name, "0") == 0) {
			return;
		}
		loc = findLocation_by_str(hash_str, name);
		while (loc == NULL) {
			input_char(name);
			if (strcmp(name, "0") == 0) {
				return;
			}
			loc = findLocation_by_str(hash_str, name);
		}
	}
	if (loc) {
		printf("\n\n");
		printf("===== 景点信息 =====\n");
		printf("ID：%d\n名称：%s\n简介：%s\n", loc->id, loc->name, loc->des);
		printf("\n");
		Sleep(2000);
	}
	else {
		printf("\n\n");
		printf("未找到该景点！\n");
		printf("\n");
	}

}

//添加景点基本信息
void add_location()
{
	printf("\n\n");
	printf("==================== 添加景点基本信息 ======================\n");
	printf("\n");
	location* loc = (location*)malloc(sizeof(location));
	if (loc == NULL) {
		printf("景点信息location初始化失败\n");
		return;
	}
	int id;
	char name[NAME_MAX], des[DES_MAX];
	printf("输入景点id:");
	scanf("%d", &id);
	printf("\n");
	printf("输入景点名称:");
	scanf("%s", name);
	printf("\n");
	printf("输入景点描述:");
	scanf("%s", des);
	printf("\n");
	clearInputBuffer();
	loc->id = id;
	strncpy(loc->name, name, NAME_MAX);
	strncpy(loc->des, des, DES_MAX);
	int res1 = insertLocation(hash_id, loc);
	int res2 = insertLocation(hash_str, loc);
	if (res1 == 1 && res2 == 1) {
		printf("插入成功\n");
		Sleep(1000);
	}
	else if (res1 != 1 || res2 != 1) {
		printf("景点id已存在\n");
		
		free(loc);
		Sleep(1000);
	}
	else {
		printf("插入失败\n");
		free(loc);
		Sleep(1000);
	}
}

//更新某景点的基本信息
void update_location()
{
	printf("\n\n");
	printf("==================== 更新景点的基本信息 =====================\n");
	printf("\n");
	printf("请输入要修改的景点id:");
	int id;
	scanf("%d", &id);
	if (id == 0) {
		return;
	}
	//通过id查找看景点存不存在
	location* pre_loc = findLocation_by_id(hash_id, id);
	while (pre_loc == NULL) {
		printf("当前输入id不存在，如果不像继续输入，输入0即可退出该流程\n");
		Sleep(1000);
		input_int(&id);
		if (id == 0) {
			return;
		}
		pre_loc = findLocation_by_id(hash_id, id);
	}
	location new_loc;
	new_loc.id = id;
	char name[NAME_MAX], des[DES_MAX];
	printf("\n");
	printf("输入新名称（原：%s）：", pre_loc->name);
	
	scanf("%s", name);
	printf("\n");
	printf("输入新简介（原：%s）：", pre_loc->des);
	
	scanf("%s", des);
	printf("\n");
	strncpy(new_loc.name, name, NAME_MAX);
	strncpy(new_loc.des, des, DES_MAX);
	int res = updateLocation(hash_id, &new_loc);
	if (res == 1) {
		printf("更新成功\n");
		Sleep(1000);
	}
	else {
		printf("更新失败\n");
		Sleep(1000);
	}
}

//查看所有景点和其相关信息
void show_all_locations()
{
	showAllLocations(hash_id);
	Sleep(3000);
}


//添加两个景点的路径信息
int add_path(int startId, int endId, int length)
{
	if (startId == endId || length <= 0) {
		return -2;
	}
	int ans = insertRoad_Link(hash_id, startId, endId, length);
	return ans;
}
void operate_add_path()
{
	printf("\n\n");
	printf("==================== 添加两个景点的路径信息 ====================\n");
	printf("\n");
	int startId, endId,length;
	printf("请输入起点Id:");
	scanf("%d", &startId);
	printf("\n");
	printf("请输入终点Id:");
	scanf("%d", &endId);
	printf("\n");
	printf("请输入路径长度:");
	scanf("%d", &length);
	printf("\n");
	int res=add_path(startId, endId, length);
	if (res == 1) {
		printf("添加成功\n");
		Sleep(1000);
	}
	else if (res == -1) {
		printf("输入的起点或者终点不存在\n");
		Sleep(1000);

	}
	else if (res == -2) {
		printf("参数有误（起点和终点的id一样或者长度为负数）\n");
		Sleep(1000);
	}
	else if(res == -3){
		printf("申请内存失败\n");
		Sleep(1000);
	}
	else {
		printf("已有对应的路径\n");
		Sleep(1000);
	}

}

//删除两个景点间的路径信息
int delete_path(int startId, int endId)
{
	if (startId == endId) {
		return -2;
	}
	
	int ans = eraseRoad_Link(hash_id, startId, endId);
	return ans;
 }
void operate_delete_path()
{
	printf("\n\n");
	printf("==================== 删除两个景点间的路径信息 ====================\n");
	printf("\n");
	int startId, endId;
	printf("请输入起点id:");
	scanf("%d", &startId);
	printf("\n");
	printf("请输入终点Id:");
	scanf("%d", &endId);
	printf("\n");
	int res = delete_path(startId, endId);
	if (res == 1) {
		printf("删除成功\n");
		Sleep(1000);
	}
	else if(res==-1) {
		printf("输入错误，无法从输入的Id找到对应景点\n");
		Sleep(1000);
	}
	else if (res == -3) {
		printf("没有对应的路径\n");
		Sleep(1000);
	}
	else {
		printf("起点id和终点id不能相同\n");
		Sleep(1000);
	}
}

//更新两个对应景点的路径信息
void update_path()
{
	printf("\n\n");
	printf("==================== 更新两个对应景点的路径信息 ====================\n");
	printf("\n");
	int startId, endId,length;
	printf("请输入起点id:");
	scanf("%d", &startId);
	printf("\n");
	printf("请输入终点Id:");
	scanf("%d", &endId);
	printf("\n");
	printf("请输入新的路径长度:");
	scanf("%d", &length);
	printf("\n");
	int res1=delete_path(startId, endId);
	if(res1!=1) {
		printf("输入错误，无法从输入的Id找到对应景点\n");
		Sleep(1000);
		return;
	}
	int res2 = add_path(startId, endId, length);
	if (res2 == 1) {
		printf("添加成功\n");
		Sleep(1000);
	}
	while (res2 != 1) {
		if (res2 == -2) {
			printf("参数有误（起点和终点的id一样或者长度为负数）\n");
			Sleep(1000);
		}
		else {
			printf("申请内存失败\n");
			Sleep(1000);
		}
		printf("如果不想更改了，按0即可退出该流程\n");
		printf("请重新输入:");
		scanf("%d", &length);
		if (length == 0) {
			return;
		}
		res2=add_path(startId, endId, length);
		if (res2 == 1) {
			printf("添加成功\n");
			Sleep(1000);
		}
	}
	
}

//显示所有路径信息
void show_all_path()
{
	if (hash_id == NULL) {
		printf("hash表未初始化，无法查找到路径\n");
		return;
	}
	printf("\n\n");
	printf("==================== 显示所有路径信息 ====================\n");
	printf("\n");
	int has_location = 0;
	for (int i = 0;i < hash_id->size;i++) {
		HashNode* p = hash_id->table[i];
		while (p!=NULL) {
			has_location = 1;
			location* loc = p->data;
			if (loc == NULL) {
				p = p->next;
				continue;
			}
			int current_id = loc->id;
			Road_Link* path = findHashNode_by_id(hash_id, current_id)->road;
			printf("\n\n");
			printf("景点【名称：%s（ID：%d）】的路径：\n", loc->name, current_id);
			if (path == NULL) {
				printf("\n\n");
				printf("该景点无任何路径\n");
				Sleep(1000);
			}
			else {
				while (path != NULL) {
					location* end_loc = findLocation_by_id(hash_id, path->id);
					if (end_loc == NULL) {
						printf("\n\n");
						printf("  → 未知景点（ID：%d），长度：%d米\n", path->id, path->length);
						
					}
					else {
						printf("\n\n");
						printf("→ 名称：%s(ID:%d),距离:%d m\n", end_loc->name, path->id, path->length);
						
					}
					path = path->next;
				}
				Sleep(1000);
			}
			printf("\n");
			p = p->next;
		}
	}
	if (!has_location) {
		printf("\n\n");
		printf("无任何景点\n");
		Sleep(1000);
	}
}


//查询任意两个景点的所有路径
int* path=NULL;                  
int path_length=0;     
int path_count=0;      

void dfs(int currentId, int endId, int currentLen,int max_id_plus)
{
	if (currentId < 0 || currentId > max_id_plus) {
		return;
	}
	if (currentId == endId) {
		path_count++;
		printf("路径 %d：", path_count);
		for (int i = 0;i < path_length;i++) {
			location* loc = findLocation_by_id(hash_id, path[i]);
			if (loc == NULL) {
				printf("未知景点(%d)", path[i]);
			}
			else {
				printf("%s(%d)", loc->name, loc->id);
			}
			if (i != path_length - 1) {
				printf("->");
			}
		}
		printf("  总长度：%d m\n", currentLen);
		
		return;
	}
	if (currentId > max_id_plus) return;
	location* temp = (location*)malloc(sizeof(location));
	if (temp != NULL) {
		*temp = *findLocation_by_id(hash_id, currentId);
	}
	else {
		exit(0);
	}
	
	insertLocation(hash_vis, temp);
	Road_Link* p = findHashNode_by_id(hash_id, currentId)->road;
	while (p != NULL) {
		int next_id = p->id;
		if (findLocation_by_id(hash_vis,next_id) == NULL) {
			path[path_length++] = next_id;
			dfs(next_id, endId, currentLen + p->length,max_id_plus);
			
			path_length--;
		}
		
		p = p->next;
	}
	deleteLocation_by_id(hash_vis, currentId);
}
void query_two_all_paths()
{
	printf("\n\n");
	printf("================== 查询任意两个景点的所有路径 ==================\n");
	printf("\n");
	int startId, endId;
	printf("请输入起点id:");
	scanf("%d", &startId);
	printf("\n");
	printf("请输入终点Id:");
	scanf("%d", &endId);
	printf("\n");
	if (!findLocation_by_id(hash_id, startId) || !findLocation_by_id(hash_id, endId)) {
		printf("\n");
		printf("输入的id不正确，无法找到对应景点\n");
		
		clearInputBuffer();
		Sleep(1000);
		return;
	}
	if (startId == endId) {
		printf("\n");
		printf("起点和终点不能相等\n");
		
		clearInputBuffer();
		Sleep(1000);
		return;
	}
	path = (int*)malloc((hash_id->count+1) * sizeof(int));
	hash_vis = initHashTable(1, id_hash_size);
	if (path == NULL || hash_vis == NULL) {
		printf("内存分配出问题\n");
		free(path);
		freeHashTable(hash_vis);
		return;
	}
	path_length = 0;
	path[path_length++] = startId;
	path_count = 0;
	printf("\n从 %s 到 %s 的所有路径：\n",
		findLocation_by_id(hash_id, startId)->name, findLocation_by_id(hash_id, endId)->name);
	dfs(startId, endId, 0,hash_id->count+1);
	if (path_count == 0) {
		printf("没有找到路径\n");
	}else{
		printf("共找到 %d 条路径\n", path_count);
	}
	free(path);
	freeHashTable(hash_vis);
	
	path = NULL;
	hash_vis = NULL;
	Sleep(1000);
}

//删除某景点和其对应的路径信息
void delete_location_path()
{
	printf("\n\n");
	printf("================== 删除某景点和其对应的路径信息 ==================\n");
	printf("\n");
	printf("请输入要删除的景点ID:");
	int id;
	scanf("%d", &id);
	printf("\n");
	location* loc = findLocation_by_id(hash_id, id);
	if (loc == NULL) {
		printf("该景点ID不存在！\n");
		Sleep(1000);
		return;
	}
	for (int i = 0;i < hash_id->size;i++) {
		HashNode* p = hash_id->table[i];
		while (p != NULL) {
			int p_id = p->data->id;
			if (p_id != id) {
				delete_path(id, p_id);
			}
			p = p->next;
		}
	}
	int res1 = deleteLocation_by_id(hash_id, id);
	int res2 = deleteLocation_by_char(hash_str, loc->name);
	if (res1 && res2) {
		printf("景点及其关联路径删除成功！\n");
		free(loc);
		Sleep(1000);
	}
	else {
		printf("景点删除失败！\n");
		
		Sleep(1000);
	}
}


//查询任意两个景点之间的最短路径
int shortest_len = INT_MAX;
int* shortest_path = NULL;
int shortest_path_len = 0;   //最短路径上面的节点数目
//使用DFS查找最短路径
void dfs_shortest_path(int currentId, int endId, int currentLen, int* path, int path_len, int max_id_plus)
{
	if (currentId == endId) {
		if (currentLen < shortest_len) {
			shortest_len = currentLen;
			shortest_path_len = path_len;
			memcpy(shortest_path, path, path_len * sizeof(int));
		}
		return;
	}
	location* temp = (location*)malloc(sizeof(location));
	if (temp != NULL) {
		*temp = *findLocation_by_id(hash_id, currentId);
	}
	else {
		exit(0);
	}
	insertLocation(hash_vis, temp);
	Road_Link* p = findHashNode_by_id(hash_id, currentId)->road;
	while (p != NULL) {
		int next_id = p->id;
		if (findLocation_by_id(hash_vis, next_id) == NULL) {
			path[path_len] = next_id;
			dfs_shortest_path(next_id, endId, currentLen + p->length, path, path_len++, max_id_plus);
		}
		p = p->next;
	}
	deleteLocation_by_id(hash_vis, currentId);
}
void query_shortest_path_dfs()
{
	printf("\n\n");
	printf("================== 查询两景点之间的最短路径 ==================\n");
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
	
	int* path = (int*)malloc((hash_id->count + 1) * sizeof(int));
	hash_vis = initHashTable(1, id_hash_size);
	shortest_path = (int*)malloc((hash_id->count + 1) * sizeof(int));
	if (path == NULL || hash_vis == NULL || shortest_path == NULL) {
		printf("内存分配出问题\n");
		free(path);
		freeHashTable(hash_vis);
		free(shortest_path);
		return;
	}
	shortest_len = INT_MAX;
	shortest_path_len = 0;
	path[0] = startId;
	dfs_shortest_path(startId, endId, 0, path, 1, hash_id->count+1);
	if (shortest_len == INT_MAX) {
		printf("没有可达路径\n");
		Sleep(1000);
	}
	else {
		printf("从 %s 到 %s 的最短路径为：\n",
			findLocation_by_id(hash_id, startId)->name, findLocation_by_id(hash_id, endId)->name);
		for (int i = 0;i < shortest_path_len;i++) {
			location* loc = findLocation_by_id(hash_id, shortest_path[i]);
			printf("%s(%d)", loc->name, loc->id);
			if (i != shortest_path_len - 1) {
				printf("->");
			}
		}
		printf("  总长度：%d m\n", shortest_len);
		
	}
	free(path);
	freeHashTable(hash_vis);
	free(shortest_path);
	shortest_path = NULL;
	Sleep(1000);
}

//使用dijkstra找最短路径（暴力）
int* get_all_loc_ids(HashTable* hash, int* out_count) {
	int count = 0;
	for (int i = 0; i < hash->size; i++) {
		for (HashNode* p = hash->table[i]; p; p = p->next) {
			count++;
		}
	}
	
	int* ids = (int*)malloc(count * sizeof(int));
	int idx = 0;
	for (int i = 0; i < hash->size; i++) {
		for (HashNode* p = hash->table[i]; p; p = p->next) {
			ids[idx++] = p->data->id;
		}
	}
	*out_count = count;
	return ids;
}
Road_Link* get_road_link(int id)
{
	int idx = id % hash_id->size;
	HashNode* p = hash_id->table[idx];
	while (p != NULL) {
		if (p->data->id == id) {
			return p->road;
		}
		p = p->next;
	}
	return NULL;
}
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
	int loc_count = 0;
	int* all_ids = get_all_loc_ids(hash_id, &loc_count);
	if (all_ids == NULL) {
		printf("获取景点ID失败\n");
		Sleep(1000);
		return;
	}

	hash_vis = initHashTable(1, id_hash_size);

	int all_nodes = 0;
	for (int i = 0; i < loc_count; i++) {
		if (all_ids[i] > all_nodes) all_nodes = all_ids[i];
	}
	
	int use_size = all_nodes + 10;
	
	//这个是用来记录查找的起点到每个对应id最短的距离
	int* dist = (int*)malloc(use_size * sizeof(int));
	//这个是用来记录最短路径上每个节点的上一个节点的id
	int* prev = (int*)malloc(use_size * sizeof(int));
	
	int* path = (int*)malloc(use_size * sizeof(int));
	/*int* shortest_path = (int*)malloc(use_size * sizeof(int));*/
	if (dist == NULL || prev == NULL || hash_vis == NULL || path == NULL ) {
		printf("内存分配出问题\n");
		free(dist);
		free(prev);
		freeHashTable(hash_vis);
		free(path);
		
		return;
	}
	for (int i = 0;i < use_size;i++) {
		dist[i] = INT_MAX;
		prev[i] = -1;
	}
	dist[startId] = 0;

	//现在最短路径节点数
	int now_count = 0;
	while (now_count < loc_count) {
		//离起点最近的节点id
		int clo_id = -1;
		int min_dist = INT_MAX;
		for (int i = 0;i < loc_count;i++) {
			int curr_id = all_ids[i];
			
			if (findLocation_by_id(hash_vis, curr_id) == NULL && dist[curr_id] < min_dist) {
				min_dist = dist[curr_id];
				clo_id = curr_id;
			}
		}
		if (clo_id == -1 || clo_id == endId) break;
		location* temp_loc = (location*)malloc(sizeof(location));
		*temp_loc = *findLocation_by_id(hash_id, clo_id);
		insertLocation(hash_vis, temp_loc);
		
		now_count++;
		Road_Link* p = get_road_link(clo_id);
		while (p != NULL) {
			int v = p->id;
			int len = p->length;
			if (dist[clo_id] != INT_MAX && dist[v] > dist[clo_id] + len) {
				dist[v] = dist[clo_id] + len;
				prev[v] = clo_id;
			}
			p = p->next;
		}
	}
	if (dist[endId] == INT_MAX) {
		printf("没有可达路径\n");
		Sleep(1000);
	}
	else {
		int path_len = 0;
		for (int curr = endId;curr != -1;curr = prev[curr]) {
			path[path_len++] = curr;
		}
		printf("从 %s 到 %s 的最短路径为：\n",
			findLocation_by_id(hash_id, startId)->name, findLocation_by_id(hash_id, endId)->name);
		for (int i = path_len - 1;i >= 0;i--) {
			int curr_id = path[i];
			location* curr_loc = findLocation_by_id(hash_id, curr_id);
			printf("%s(%d)", curr_loc->name, curr_loc->id);
			if (i != 0) printf("->");
		}
			printf("  总长度：%d m\n", dist[endId]);
			memcpy(shortest_path, path, path_len * sizeof(int));
		}
	free(dist);
	free(prev);
	free(path);
	
	freeHashTable(hash_vis);
	hash_vis = NULL;
	free(all_ids);
	clearInputBuffer();
	Sleep(1000);
}

//使用dijkstra找最短路径（堆优化）
//void dijkstra_shortest_path_heap()
//{
//	printf("================== 查询两景点之间的最短路径（堆优化Dijkstra） ==================\n");
//	int startId, endId;
//	printf("请输入起点ID:");
//	scanf("%d", &startId);
//	printf("请输入终点ID:");
//	scanf("%d", &endId);
//	location* start_loc = findLocation_by_id(hash_id, startId);
//	location* end_loc = findLocation_by_id(hash_id, endId);
//	if (start_loc == NULL || end_loc == NULL) {
//		printf("输入的id不正确，无法找到对应景点\n");
//		clearInputBuffer();
//		return;
//	}
//	if (startId == endId) {
//		printf("起点和终点不能相等\n");
//		clearInputBuffer();
//		return;
//	}
//	/*hash_vis = initHashTable(1, id_hash_size);
//	if (hash_vis == NULL) {
//		printf("hash_vis初始化失败\n");
//		return;
//	}*/
//	int max_id = 0;
//	int loc_count = 0;
//	for (int i = 0; i < hash_id->size; i++) {
//		for (HashNode* p = hash_id->table[i]; p; p = p->next) {
//			loc_count++;
//			if (p->data->id > max_id) max_id = p->data->id;
//		}
//	}
//	int use_size = max_id + 10;
//	int* visited = (int*)calloc(use_size, sizeof(int));
//	int* dist = (int*)malloc(use_size * sizeof(int));
//	int* prev = (int*)malloc(use_size * sizeof(int));
//	/*int* path = (int*)malloc(use_size * sizeof(int));*/
//	/*int* shortest_path = (int*)malloc(use_size * sizeof(int));*/
//	if (dist == NULL || prev == NULL||visited==NULL) {
//		printf("内存分配出问题\n");
//		free(dist);
//		free(prev);
//		free(visited);
//		freeHashTable(hash_vis);
//		return;
//	}
//	for (int i = 0; i < use_size; i++) {
//		dist[i] = INT_MAX;
//		prev[i] = -1;
//	}
//	dist[startId] = 0;
//	Value_Array* min_heap = init_value_array();
//	if (min_heap == NULL) {
//		printf("堆初始化失败\n");
//		free(dist);
//		free(prev);
//		
//		freeHashTable(hash_vis);
//		return;
//	}
//
//	Value start_val;
//	/*start_val.data = start_loc;*/
//	start_val.data = NULL;
//	start_val.value = 0;
//	Value* heap_val = (Value*)malloc(sizeof(Value));
//
//	if (heap_val == NULL) {
//        printf("堆节点内存分配失败\n");
//        free_Value_Array(min_heap);
//        free(dist);
//		free(prev);
//        freeHashTable(hash_vis);
//        return;
//    }
//	*heap_val = start_val;
//    /*heap_val->data = start_loc;
//    heap_val->value = 0;*/
//    insert_heap(min_heap, heap_val);
//	int* heap_id_map = (int*)malloc(1000 * sizeof(int));
//	int heap_id_count = 0;
//	heap_id_map[heap_id_count++] = startId;
//	while (empty_value_array(min_heap) != 1) {
//		Value curr_val;
//		if (erase_heap(min_heap, &curr_val) == -1) {
//			break;
//		}
//		/*location* curr_loc = curr_val.data;
//		if (curr_loc == NULL) break;*/
//		/*int curr_id = curr_loc->id;*/
//		int curr_dist = curr_val.value;
//		int curr_id = -1;
//		if (heap_id_count > 0) {
//			curr_id = heap_id_map[--heap_id_count];
//		}
//		if (curr_id == -1) break;
//
//		if (findLocation_by_id(hash_vis, curr_id) != NULL) {
//			continue;
//		}
//
//		location* temp_loc = (location*)malloc(sizeof(location));
//		temp_loc->id = curr_id;
//		/*strncpy(temp_loc->name, curr_loc->name, NAME_MAX);
//		strncpy(temp_loc->des, curr_loc->des, DES_MAX);*/
//		strcpy(temp_loc->name, "");
//		strcpy(temp_loc->des, "");
//		insertLocation(hash_vis, temp_loc);
//
//		Road_Link* p_road = get_road_link(curr_id);
//		while (p_road != NULL) {
//			int next_id = p_road->id;
//			int path_len = p_road->length;
//			/*location* next_loc = findLocation_by_id(hash_id, next_id);*/
//
//			if (dist[curr_id] != INT_MAX && dist[next_id] > dist[curr_id] + path_len) {
//				dist[next_id] = dist[curr_id] + path_len;
//				prev[next_id] = curr_id;
//
//				/*if (next_loc == NULL) {
//					p_road = p_road->next;
//					continue;
//				}*/
//				Value next_val;
//				/*next_val.data = next_loc;*/
//				next_val.data = NULL;
//				next_val.value = dist[next_id];
//				Value* heap_next_val = (Value*)malloc(sizeof(Value));
//				*heap_next_val = next_val;
//				insert_heap(min_heap, heap_next_val);
//				heap_id_map[heap_id_count++] = next_id;
//			}
//			p_road = p_road->next;
//		}
//
//		if (curr_id == endId) {
//			break;
//		}
//	}
//	if (dist[endId] == INT_MAX) {
//		printf("没有可达路径\n");
//	}
//	else {
//		int path[100];
//		int path_len = 0;
//		for (int curr = endId; curr != -1; curr = prev[curr]) {
//			path[path_len++] = curr;
//		}
//		printf("从 %s 到 %s 的最短路径为：\n", start_loc->name, end_loc->name);
//		for (int i = path_len - 1; i >= 0; i--) {
//			int curr_id = path[i];
//			location* curr_loc = findLocation_by_id(hash_id, curr_id);
//			printf("%s(%d)", curr_loc->name, curr_loc->id);
//			if (i != 0) printf("->");
//		}
//		printf("  总长度：%d m\n", dist[endId]);
//		/*memcpy(shortest_path, path, path_len * sizeof(int));*/
//	}
//
//	free(dist);
//	free(prev);
//	/*free(path);
//	free(shortest_path);*/
//
//	if (min_heap != NULL && min_heap->arr != NULL) {
//		for (int i = 0; i < min_heap->size; i++) {
//			/*if (min_heap->arr[i] != NULL) {
//				free(min_heap->arr[i]);
//			}*/
//			free(min_heap->arr[i]);
//		}
//	}
//	free_Value_Array(min_heap);
//	freeHashTable(hash_vis);
//	hash_vis = NULL;
//	clearInputBuffer();
//}
//如果整数输入错误，重新输入
void input_int(int* input)
{
	printf("\n");
	printf("输入错误，请重新输入:");
	scanf("%d", input);
}

void input_char(char* c)
{
	printf("输入错误，请重新输入:");
	scanf("%s", c);
}

//输入是否合规的检查方法
int judge_validity_input()
{
	int input;
	while (true) {
		if (scanf("%d", &input) == 1) {
			//输入合法，放行
			break;
		}
		else {
			//输入不合规，清除缓冲区
			while (getchar() != '\n');
			printf("输入不合规，请重新输入：");
		}
	}
	return input;
}

void enter_again() {
	printf("输入有误，重新输入");
}

//释放所有分配的内存
/*void free_all()
{
	if (roads != NULL&&hash_id!=NULL) {
		for (int i = 0;i < hash_id->size;i++) {
			HashNode* p1 = hash_id->table[i];
			while (p1 != NULL) {
				int delete_id = p1->data->id;
				if (delete_id >= 0) {
					Road_Link* p2 = roads[delete_id].link;
					while (p2 != NULL) {
						Road_Link* tmp = p2;
						p2 = p2->next;
						free(tmp);
					}
					roads[delete_id].link = NULL;
				}
				p1 = p1->next;
			}
		}
		free(roads);
		roads = NULL;
	}
	if (hash_id != NULL) {
		for (int i = 0;i < hash_id->size;i++) {
			HashNode* p = hash_id->table[i];
			while (p != NULL) {
				HashNode* tmp = p;
				p = p->next;
				free(tmp->data);
				free(tmp);
			}
		}
		free(hash_id->table);
		free(hash_id);
		hash_id = NULL;
	}
	if (hash_str != NULL) {
		for (int i = 0;i < hash_str->size;i++) {
			HashNode* p = hash_str->table[i];
			while (p != NULL) {
				HashNode* tmp = p;
				p = p->next;
				free(tmp);
			}
		}
		free(hash_str->table);
		free(hash_str);
		hash_str = NULL;
	}
}
*/

int main()
{
	if (init_all_part() == -1) {
		printf("初始化失败\n");
		return -1;
	}
	
	while (true) {
		menu();
		int choice = judge_validity_input();
		while (true) {
			if (choice >= 0 && choice <= 12) {
				break;
			}
			else {
				
				while (getchar() != '\n');
				input_int(&choice);
			}
		}
		switch (choice) {
		case 1:
			find_location();
			break;
		case 2:
			/*dijkstra_shortest_path_heap();*/
			/*query_shortest_path_dfs();*/
			dijkstra_shortest_path();
			break;
		case 3:
			query_two_all_paths();
			break;
		case 4:
			add_location();
			break;
		case 5:
			delete_location_path();
			break;
		case 6:
			update_location();
			break;
		case 7:
			operate_add_path();
			break;
		case 8:
			operate_delete_path();
			break;
		case 9:
			update_path();
			break;
		case 10:
			show_all_locations();
			break;
		case 11:
			show_all_path();
			break;
		case 12:
			break;
		case 0:
			printf("感谢您使用本校园导游系统！即将退出。\n");
			return 0;
		default:
			enter_again();
			break;
		}
	}
	

	return 0;
}
