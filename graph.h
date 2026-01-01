#pragma once
#define NAME_MAX 100
#define DES_MAX 1000
typedef struct location {
	int id;
	char name[NAME_MAX];
	char des[DES_MAX];
	
}location; 
typedef struct Road_Link {
    int id;  //对应景点id
    int length;   //路径长度
    struct Road_Link* next;  //指向下一个路的指针
} Road_Link;  //链表存放景点数据




