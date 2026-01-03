#define _CRT_SECURE_NO_WARNINGS  1 
#include"graph.h"
#include "hash.h"
#include <Windows.h>
#include<string.h>
#include"load_data.h"
extern HashTable* hash_id;
extern HashTable* hash_str;

void init_test_data()
{

	Sleep(500);
	// ID景点 1~2147483646
	// ID：1,999,9999, 99999999, 100000000, 500000000, 1000000000, 2000000000, 2147483646
	int big_ids[] = { 1, 9999, 999999, 99999999, 100000000, 500000000, 1000000000, 2000000000, 2147483646 };
	char* big_names[] = { "校门", "图书馆", "教学楼", "体育馆", "欣苑食堂", "欣苑宿舍", "信息科学楼", "行政楼", "校医院" };
	char* big_des[] = {
		"学校主入口，唯一进出通道",
		"藏书百万册，全校最大图书馆",
		"主要授课区域",
		"室内外运动场地",
		"三层餐饮区",
		"学生住宿区域",
		"启明星工作室所在地",
		"学校行政办公区域",
		"校内医疗机构"
	};
	int big_len = sizeof(big_ids) / sizeof(int);
	// 批量插入大跨度核心景点
	for (int i = 0; i < big_len; i++)
	{
		location* loc = (location*)malloc(sizeof(location));
		loc->id = big_ids[i];
		strcpy(loc->name, big_names[i]);
		strcpy(loc->des, big_des[i]);
		insertLocation(hash_id, loc);
		insertLocation(hash_str, loc);
	}

	// 连续ID景点 100~500 连续ID，共400个景点
	for (int i = 100; i <= 500; i++)
	{
		location* loc = (location*)malloc(sizeof(location));
		loc->id = i;
		char name_buf[NAME_MAX] = { 0 };
		char des_buf[DES_MAX] = { 0 };
		sprintf(name_buf, "景点_%d", i);
		sprintf(des_buf, "校园景观区域，编号%d，测试数据", i);
		strcpy(loc->name, name_buf);
		strcpy(loc->des, des_buf);
		insertLocation(hash_id, loc);
		insertLocation(hash_str, loc);
	}

	// 零散随机跨度ID景点
	int rand_ids[] = { 7, 15, 23, 47, 89, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144 };
	int rand_len = sizeof(rand_ids) / sizeof(int);
	for (int i = 0; i < rand_len; i++)
	{
		location* loc = (location*)malloc(sizeof(location));
		loc->id = rand_ids[i];
		char name_buf[NAME_MAX] = { 0 };
		char des_buf[DES_MAX] = { 0 };
		sprintf(name_buf, "随机点位_%d", rand_ids[i]);
		sprintf(des_buf, "随机ID测试点位，无实际用途，ID=%d", rand_ids[i]);
		strcpy(loc->name, name_buf);
		strcpy(loc->des, des_buf);
		insertLocation(hash_id, loc);
		insertLocation(hash_str, loc);
	}

	// 爆炸级路径生成 路径量极大，全覆盖边界条件 
	// 所有大跨度核心ID 互相绑定双向路径，长度随机1~999，无重复
	for (int i = 0; i < big_len; i++)
	{
		for (int j = i + 1; j < big_len; j++)
		{
			int len1 = rand() % 999 + 1; // 路径长度必须>0，符合你的add_path规范
			int len2 = rand() % 999 + 1;
			add_path(big_ids[i], big_ids[j], len1);  // 正向路径
			add_path(big_ids[j], big_ids[i], len2);  // 反向路径
		}
	}

	// 连续小ID(100~500) ，每个节点绑定前后10个节点的路径，形成网状结构
	for (int i = 100; i <= 500; i++)
	{
		int bound = i + 10 > 500 ? 500 : i + 10;
		for (int j = i + 1; j <= bound; j++)
		{
			int len = rand() % 50 + 5; // 短路径为主，测试最短路径优先级
			add_path(i, j, len);
			add_path(j, i, len);
		}
		// 每个小ID都绑定1个大跨度ID，测试跨区间路径
		add_path(i, big_ids[rand() % big_len], rand() % 1000 + 100);
	}

	// 随机ID 绑定核心大ID+连续小ID，形成全连通图，无孤立节点
	for (int i = 0; i < rand_len; i++)
	{
		// 绑定3个核心大ID
		for (int j = 0; j < 3; j++)
		{
			add_path(rand_ids[i], big_ids[j], rand() % 2000 + 50);
			add_path(big_ids[j], rand_ids[i], rand() % 2000 + 50);
		}
		// 绑定5个连续小ID
		int rand_small = rand() % 400 + 100;
		for (int j = 0; j < 5; j++)
		{
			add_path(rand_ids[i], rand_small + j, rand() % 100 + 10);
			add_path(rand_small + j, rand_ids[i], rand() % 100 + 10);
		}
	}

	// 添加孤立节点（无任何路径），测试DFS/Dijkstra的边界判断：无路径可达
	location* lonely1 = (location*)malloc(sizeof(location));
	lonely1->id = 666666666;
	strcpy(lonely1->name, "孤立节点1");
	strcpy(lonely1->des, "无任何路径，纯测试孤立节点边界条件");
	insertLocation(hash_id, lonely1);
	insertLocation(hash_str, lonely1);

	location* lonely2 = (location*)malloc(sizeof(location));
	lonely2->id = 888888888;
	strcpy(lonely2->name, "孤立节点2");
	strcpy(lonely2->des, "无任何路径，纯测试孤立节点边界条件");
	insertLocation(hash_id, lonely2);
	insertLocation(hash_str, lonely2);

	// 添加超长路径（长度极大），测试最短路径算法的数值
	add_path(1, 2147483646, 999999);
	add_path(2147483646, 1, 999999);


	Sleep(1000);
}