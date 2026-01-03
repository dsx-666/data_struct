#pragma once

typedef struct location location;

typedef struct {
	int value;
	int id;
}Value;

typedef struct {
	Value** arr;
	int size;
}Value_Array;

// 初始化Value_Array，未成功返回NULL
Value_Array* init_value_array();

// 判断是否为空 1代表空 -1代表非空
int empty_value_array(Value_Array* a);
// 取对应index的array数据 不成功返回NULL
Value* show_array(Value_Array* a, int index);
// 放入value进Value_Array尾 成功返回1 不成功返回-1
int push_value(Value_Array* a, Value* data);
// 交换array中对应i和j的索引
void swap_array(Value_Array* a, int i, int j);
// 将Value_Array尾元素赋值到首元素并且删除尾元素 如果成功将temp复制首元素 不成功返回-1
int pop_value(Value_Array* a, Value* ans);

// 上浮小根堆
void up_heap(Value_Array* a, int size);
// 下沉小根堆
void down_heap(Value_Array* a, int parent);
// 堆插入 插入成功返回1，否则返回-1
int insert_heap(Value_Array* a, Value* data);
// 堆删除并且提出最小数据 删除成功返回1，否则返回-1
int erase_heap(Value_Array* a, Value* data);
// 释放Value_Array内存
void free_Value_Array(Value_Array* a);