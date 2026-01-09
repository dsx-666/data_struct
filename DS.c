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
#include "Dijkstra.h"
#include"kmp.h"

#ifdef _WIN32
#include <windows.h>
#define SLEEP_MS(ms) Sleep(ms)
#else
#include <unistd.h>
#define SLEEP_MS(ms) usleep(ms * 1000)
#endif

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
	printf("7：添加两个景点的路径                                        \n");
	printf("8：删除两个景点间的路径信息                                  \n");
	printf("9：更新两个对应景点之间的路径长度                            \n");
	printf("10：查看所有景点和其相关信息                                 \n");
	printf("11：显示所有路径信息                                         \n");
	printf("12：模糊查询景点基本信息                                     \n");
	printf("13：模糊查询景点路径                                         \n");
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
	system("cls");
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
	printf("请输入任意键退出");
	char a = getchar();
	a = getchar();
	system("cls");
}

//添加景点基本信息
void add_location()
{
	system("cls");
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
	strcpy(loc->name, name);
	
	strcpy(loc->des, des);
	
	int res1 = insertLocation(hash_id, loc);
	int res2 = insertLocation(hash_str, loc);
	if (res1 == 1 && res2 == 1) {
		printf("插入成功\n");
		Sleep(1000);
	}
	else if (res1 == 0||res2 == 0) {
		if (res1 == 0) {
			printf("景点id已存在\n");
			deleteLocation_by_char(hash_str, loc->name);

		}
		else {
			printf("景点名称已存在\n");
			deleteLocation_by_id(hash_id, loc->id);
		}
		free(loc);
		Sleep(1000);
	}
	else {
		printf("插入失败\n");
		free(loc);
		Sleep(1000);
	}
	system("cls");
}

//更新某景点的基本信息
void update_location()
{
	system("cls");
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
		printf("当前输入id不存在，如果不想继续输入，输入0即可退出该流程\n");
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
	if (findLocation_by_str(hash_str, name) != NULL) {
		printf("名称已存在，更新失败\n");
		Sleep(1000);
		system("cls");
		return;
	}
	strcpy(new_loc.name, name);

	strcpy(new_loc.des, des);

	int res = updateLocation(hash_id, &new_loc);
	if (res == 1) {
		printf("更新成功\n");
		Sleep(1000);
	}
	else {
		printf("更新失败\n");
		Sleep(1000);
	}
	system("cls");
}

//查看所有景点和其相关信息
void show_all_locations()
{
	system("cls");
	int page;
	int num = 0;
	num=showAllLocations(hash_id, 0);
	printf("总共%d页,%d条查询结果\n", (num + 4) / 5, num);
	printf("请输入要查询的页码(每页显示5条记录，按0可回到菜单):");
	scanf("%d", &page);
	int start = (page - 1) * 5+1;
	int count;
	
	while (page < 0||page>(num + 4) / 5) {
		printf("页码输入错误\n");
		Sleep(1000);
		system("cls");
		printf("总共%d页,%d条查询结果\n", (num + 4) / 5, num);
		printf("请重新输入要查询的页码(每页显示5条记录，按0可回到菜单):");
		scanf("%d", &page);
		int start = (page - 1) * 5 + 1;
	}
	
	if (page == 0) {
		system("cls");
		return;
	}
	
	while (page != 0) {
		count = 0;
		printf("第%d页查询结果如下：\n", page);
		if (page < 0 || page>(num + 4) / 5) {
			system("cls");
		}
		count = showAllLocations(hash_id, start);
		printf("总共%d页,%d条查询结果\n", (count + 4) / 5, count);
		printf("请输入要查询的页码(每页显示5条记录,按0可回到上一步):");
		scanf("%d", &page);
		start = (page - 1) * 5 + 1;
		system("cls");
	}
	
	//printf("\n请输入任意键退出");
	//char a = getchar();
	//a = getchar();
	///*Sleep(3000);*/
	//system("cls");

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
	system("cls");
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
	system("cls");
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
	system("cls");
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
	system("cls");
}

//更新两个对应景点的路径信息
void update_path()
{
	system("cls");
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
	system("cls");
}

//显示所有路径信息
void show_all_path()
{
	system("cls");
	if (hash_id == NULL) {
		printf("hash表未初始化，无法查找到路径\n");
		return;
	}
	printf("\n\n");
	printf("==================== 显示所有路径信息 ====================\n");
	printf("\n");
	int page;
	int num = 0;
	//计算总共有多少条路径
	for (int i = 0;i < hash_id->size;i++) {
		HashNode* p = hash_id->table[i];

		while (p != NULL) {
			location* loc = p->data;
			if (loc == NULL) {
				p = p->next;
				continue;
			}
			int current_id = loc->id;
			Road_Link* path = findHashNode_by_id(hash_id, current_id)->road;
			while (path != NULL) {
				
				path = path->next;
			}
			num++;
			p = p->next;
		}

	}
	printf("总共%d页,%d条查询结果\n", (num + 2) / 3, num);

	printf("请输入要查询的页码(每页显示3条记录，按0可回到上一步):");
	scanf("%d", &page);
	
	int start = (page - 1) * 3 + 1;	
	while (page < 0) {
		printf("页码输入错误\n");
		Sleep(1000);
		system("cls");
		printf("总共%d页,%d条查询结果\n", (num + 2) / 3, num);
		printf("请重新输入要查询的页码(每页显示3条记录，按0可回到上一步):");

		scanf("%d", &page);
		int start = (page - 1) * 3 + 1;
	}
	if (page == 0) {
		system("cls");
		return;
	}
	while (page != 0&&start+3< (num + 2) / 3) {
		printf("第%d页查询结果如下：\n", page);
		int count = 0;
		int has_location = 0;
		for (int i = 0;i < hash_id->size;i++) {
			HashNode* p = hash_id->table[i];

			while (p != NULL) {
				count++;
				has_location = 1;
				location* loc = p->data;
				if (loc == NULL) {
					p = p->next;
					continue;
				}
				int current_id = loc->id;
				Road_Link* path = findHashNode_by_id(hash_id, current_id)->road;
				if (count >= start - 1 && count < start + 2) {
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
				}
				p = p->next;
				
			}

		}

		if (!has_location) {
			printf("\n\n");
			printf("无任何景点\n");
			Sleep(1000);
		}


		printf("总共%d页,%d条查询结果\n", (count + 2) / 3, count);
		printf("请输入要查询的页码(每页显示3条记录,按0可回到菜单):");
		scanf("%d", &page);
		system("cls");
		if (page == 0) break;
		start = (page - 1) * 3 + 1;
	}
	if (start + 3 >= (num + 2) / 3) {
		printf("没有更多页面了\n");

		Sleep(1000);
		system("cls");
	}
	/*printf("\n请按任意键退出");
	char a = getchar();
	a = getchar();
	system("cls");*/
}


//查询任意两个景点的所有路径
int* path=NULL;                  
int path_length=0;     
int path_count=0;      
//分页查询所用的某页其实的位数
int start_path_dfs=0;

void dfs(int currentId, int endId, int currentLen,int max_id_plus)
{
	
	if (currentId < 0 || currentId > max_id_plus) {
		return;
	}
	if (path_count >= start_path_dfs + 3) {
		return;
	}
	if (currentId == endId) {
		
		path_count++;
		if (path_count >= start_path_dfs && path_count < start_path_dfs + 3) {
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
			printf("\n  总长度：%d m\n\n\n", currentLen);
			
		}
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
	system("cls");
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
	int page;
	printf("请输入要查询的页数(每页显示3条路径,按0可回到主界面):");
	scanf("%d", &page);
	start_path_dfs = (page - 1) * 3 + 1;
	while (page < 0) {
		printf("页码输入错误\n");
		Sleep(1000);
		system("cls");
		printf("请重新输入要查询的页数(每页显示3条路径,按0可回到主界面):");
		scanf("%d", &page);
		start_path_dfs = (page - 1) * 3 + 1;
	}
	if (page == 0) {
		system("cls");
		return;
	}
	while (page != 0) {
		printf("第%d页查询结果如下：\n", page);
		path = (int*)malloc((hash_id->count + 1) * sizeof(int));
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

		dfs(startId, endId, 0, hash_id->count + 1);
		if (path_count == 0) {
			printf("没有找到路径\n");
		}/*else{
			printf("共找到 %d 条路径\n", path_count);
		}*/
		free(path);
		freeHashTable(hash_vis);

		path = NULL;
		hash_vis = NULL;
		Sleep(1000);
		printf("请输入要查询的页数(每页显示3条路径,按0可回到主界面):");
		scanf("%d", &page);
		system("cls");
		if (page == 0) break;
		start_path_dfs = (page - 1) * 3 + 1;
	}
	/*printf("\n请输入任意键退出");
	char a = getchar();
	a = getchar();
	start_path_dfs = 0;
	system("cls");*/
}

//删除某景点和其对应的路径信息
void delete_location_path()
{
	system("cls");
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
		system("cls");
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
	system("cls");
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
	system("cls");
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
	printf("请输入任意键退出");
	char a = getchar();
	a = getchar();
	system("cls");
}

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


////模糊查询景点基本信息
void fuzzy_query_location()
{
	system("cls");
	printf("\n\n");
	printf("================== 模糊查询景点基本信息 ==================\n");
	printf("\n");
	printf("请输入要查询的景点名称关键字:");
	char keyword[NAME_MAX];
	scanf("%s", keyword);
	printf("\n");
	int page;
	int num = 0;
	kmp_search_location(hash_str, keyword, &num);
	printf("总共%d页,%d个查询结果\n", (num + 4) / 5, num);
	printf("请输入要查询的页码(每页显示5条记录，输入0退出查询):");
	
	
	scanf("%d", &page);
	while (page < 0||page>(num + 4) / 5) {
		printf("页码输入错误\n");
		Sleep(1000);
		system("cls");
		printf("总共%d页,%d个查询结果\n", (num + 4) / 5, num);
		printf("请重新输入要查询的页码(每页显示5条记录，输入0退出查询):");
		scanf("%d", &page);
	}
	if (page == 0) {
		system("cls");
		return;
	}
	while (page != 0) {
		printf("第%d页查询结果如下：\n", page);
		int count = 0;
		location* locs = kmp_search_location(hash_str, keyword, &count);
		if (locs == NULL) {
			printf("未找到相关景点信息\n");
			Sleep(1000);
			system("cls");
			return;
		}
		if (count == 0) printf("未找到相关景点信息\n");
		int index = 0;
		int start = (page - 1) * 5;
		if (start >= count) {
			printf("该页码无数据\n");
			free(locs);
			Sleep(1000);
			system("cls");
			return;
		}
		int end = page * 5;
		printf("\n\n");
		printf("===== 查询结果 =====\n");
		for (int i = start; i < end && i < count; i++) {
			if (locs[i].id == 0) {
				break;
			}
			printf("ID：%d\n名称：%s\n简介：%s\n", locs[i].id, locs[i].name, locs[i].des);
			printf("\n");
			index++;
		}
		printf("总共%d页,%d个查询结果\n", (count + 4) / 5, count);
		free(locs);
		printf("\n请输入要查询的页码(每页显示5条记录，输入0退出查询):");
		scanf("%d", &page);
		system("cls");

	}
	/*printf("\n请输入任意键退出");
	char a = getchar();
	a = getchar();
	system("cls");*/

}

//模糊查询景点路径
void fuzzy_query_path()
{
	system("cls");
	printf("\n\n");
	printf("================== 模糊查询景点路径信息 ==================\n");
	printf("\n");
	printf("请输入要查询的景点名称关键字:");
	char keyword[NAME_MAX];
	scanf("%s", keyword);
	printf("\n");
	int num = 0;

	kmp_search_location(hash_str, keyword, &num);
	printf("总共%d页,%d个查询结果\n", (num + 4) / 5, num);
	printf("请输入要查询的页码(每页显示5条记录，输入0退出查询):");
	int page;
	scanf("%d", &page);
	while (page < 0||page>(num + 4) / 5) {
		printf("页码输入错误\n");
		Sleep(1000);
		system("cls");
		printf("总共%d页,%d个查询结果\n", (num + 4) / 5, num);
		printf("请重新输入要查询的页码(每页显示5条记录，输入0退出查询):");
		scanf("%d", &page);
	}
	if (page == 0) {
		system("cls");
		return;
	}
	while (page != 0) {
		printf("第%d页查询结果如下：\n", page);
		int count = 0;
		location* locs = kmp_search_location(hash_str, keyword, &count);
		if (locs == NULL) {
			printf("未找到相关景点信息\n");
			Sleep(1000);
			system("cls");
			return;
		}
		if (count == 0) printf("未找到相关景点信息\n");
		int index = 0;
		int start = (page - 1) * 5;
		if (start >= count) {
			printf("该页码无数据\n");
			free(locs);
			Sleep(1000);
			system("cls");
			return;
		}
		int end = page * 5;
		printf("\n\n");
		printf("===== 查询结果 =====\n");
		for (int i = start; i < end && i < count; i++) {
			if (locs[i].id == 0) {
				break;
			}
			printf("景点【名称：%s（ID：%d）】的路径：\n", locs[i].name, locs[i].id);
			Road_Link* path = findHashNode_by_id(hash_id, locs[i].id)->road;
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
			index++;
		}
		printf("总共%d页,%d个查询结果\n", (count + 4) / 5, count);
		free(locs);
		printf("\n请输入要查询的页码(每页显示5条记录，输入0退出查询):");
		scanf("%d", &page);
		system("cls");

	}
	/*printf("\n请输入任意键退出");
	char a = getchar();
	a = getchar();
	system("cls");*/

}

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
//主界面组件
void print_header() {
	printf("***********************************************************\n");
	printf("  ____    _    __  __ ____  _   _ ____    ____ _   _ ___ ____  _____ \n");
	printf(" / ___|  / \\  |  \\/  |  _ \\| | | / ___|  / ___| | | |_ _|  _ \\| ____|\n");
	printf("| |     / _ \\ | |\\/| | |_) | | | \\___ \\ | |  _| | | || || | | |  _|  \n");
	printf("| |___ / ___ \\| |  | |  __/| |_| |___) || |_| | |_| || || |_| | |___ \n");
	printf(" \\____/_/   \\_\\_|  |_|_|    \\___/|____/  \\____|\\___/|___|____/|_____|\n");
	printf("\n***********************************************************\n");
	printf("                 欢迎来到校园导游咨询系统\n\n");
}

void loading_bar() {
	int i;
	printf("系统加载中，请稍候...\n");
	printf("[");
	for (i = 0; i <= 50; i++) {
		printf(" "); 
	}
	printf("] 0%%");

	for (i = 0; i <= 100; i++) {
		
		printf("\r[");
		int pos = i / 2;
		for (int j = 0; j < 50; j++) {
			if (j < pos) printf("=");
			else if (j == pos) printf(">");
			else printf(" ");
		}
		printf("] %d%%", i);

		fflush(stdout); 
		SLEEP_MS(30);   
	}
	printf("\n\n加载完成！按任意键进入系统...");
	getchar();
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
	
	//测试用的生成的海量数据集
	/*init_test_data();*/
	// 1. 清屏
	#ifdef _WIN32
	system("cls");
	#else
	system("clear");
	#endif

	// 2. 显示标题
	print_header();

	// 3. 显示加载条
	loading_bar();

	system("cls");
	while (true) {
		menu();
		int choice = judge_validity_input();
		while (true) {
			if (choice >= 0 && choice <= 13) {
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
			dijkstra_shortest_path_heap();
			/*query_shortest_path_dfs();*/
			/*dijkstra_shortest_path();*/
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
			fuzzy_query_location();
			break;
		case 13:
			fuzzy_query_path();
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
