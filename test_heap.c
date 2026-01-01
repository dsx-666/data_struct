/*
#define _CRT_SECURE_NO_WARNINGS  1 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "heap.h"

#define DATA_COUNT  100000

// 纯遍历数组找最小值
int find_min_by_scan(Value** arr, int len, Value* res)
{
	if (arr == NULL || len <= 0 || res == NULL)
		return -1;
	int min_idx = 0;
	for (int i = 1; i < len; i++)
	{
		if (arr[i]->value < arr[min_idx]->value)
		{
			min_idx = i;
		}
	}
	*res = *arr[min_idx];
	return 1;
}

//  纯数组选择排序从小到大完整排序 (纯暴力 O(n*n))
void select_sort(Value** arr, int len)
{
	if (arr == NULL || len <= 1) return;
	for (int i = 0; i < len - 1; i++)
	{
		int min_idx = i;
		// 遍历找最小值下标
		for (int j = i + 1; j < len; j++)
		{
			if (arr[j]->value < arr[min_idx]->value)
			{
				min_idx = j;
			}
		}
		// 交换到已排序区
		Value* temp = arr[i];
		arr[i] = arr[min_idx];
		arr[min_idx] = temp;
	}
}

// 泡排序从小到大完整排序 (纯暴力 O(n*n))
void bubble_sort(Value** arr, int len)
{
	if (arr == NULL || len <= 1) return;
	for (int i = 0; i < len - 1; i++)
	{
		for (int j = 0; j < len - 1 - i; j++)
		{
			if (arr[j]->value > arr[j + 1]->value)
			{
				Value* temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

// 校验是否严格升序
int check_array_sort(Value** arr, int len)
{
	if (arr == NULL || len <= 1) return 1;
	for (int i = 0; i < len - 1; i++)
	{
		if (arr[i]->value > arr[i + 1]->value)
		{
			return 0;
		}
	}
	return 1;
}

int main() {
	srand((unsigned int)time(NULL));
	clock_t start, end;
	double cost_time;

	
	// 原数组：生成数据
	Value** normal_arr = (Value**)malloc(DATA_COUNT * sizeof(Value*));
	// 选择排序数组：拷贝原数据
	Value** select_arr = (Value**)malloc(DATA_COUNT * sizeof(Value*));
	// 冒泡排序数组：拷贝原数据
	Value** bubble_arr = (Value**)malloc(DATA_COUNT * sizeof(Value*));

	if (normal_arr == NULL || select_arr == NULL || bubble_arr == NULL)
	{
		printf("数组内存申请失败！\n");
		return -1;
	}
	// 生成随机数据，三个数组存相同数据
	for (int i = 0; i < DATA_COUNT; i++) {
		Value* val = (Value*)malloc(sizeof(Value));
		val->value = rand() % 1000000;  
		
		normal_arr[i] = val;
		select_arr[i] = val;
		bubble_arr[i] = val;
	}

	// 小根堆 插入 弹出完整升序
	Value_Array* heap = init_value_array();
	if (heap == NULL) {
		printf("小根堆初始化失败！\n");
		return -1;
	}
	// 堆插入数据
	start = clock();
	for (int i = 0; i < DATA_COUNT; i++) {
		insert_heap(heap, normal_arr[i]);
	}
	end = clock();
	cost_time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("小根堆 (时间复杂度 O(n logn))\n");
	printf("数据插入完成，堆size = %d\n", heap->size);
	printf("插入耗时：%.4lf 秒\n", cost_time);
	double temp = cost_time;
	// 堆弹出所有数据
	Value res;
	int last_val = -1;
	int heap_ok = 1;
	start = clock();
	while (empty_value_array(heap) != 1) {
		erase_heap(heap, &res);
		if (res.value < last_val) { heap_ok = 0; break; }
		last_val = res.value;
	}
	end = clock();
	cost_time = (double)(end - start) / CLOCKS_PER_SEC;
	if (heap_ok) printf("全部数据弹出完成，严格升序，无错误\n");
	else printf("小根堆弹出数据无序！\n");
	printf("弹出升序耗时：%.4lf 秒\n", cost_time);
	printf("小根堆总耗时(插入+弹出)：%.4lf 秒\n\n", temp + (double)(clock() - start) / CLOCKS_PER_SEC);

	// 选择排序从小到大完整排序 
	start = clock();
	select_sort(select_arr, DATA_COUNT);
	end = clock();
	cost_time = (double)(end - start) / CLOCKS_PER_SEC;
	int select_ok = check_array_sort(select_arr, DATA_COUNT);
	printf("纯数组选择排序 (纯暴力 O(n*n))\n");
	if (select_ok) printf(" 排序完成，数组严格升序，无错误\n");
	else printf("选择排序失败，数组无序\n");
	printf("排序耗时：%.4lf 秒\n\n", cost_time);

	// 纯数组冒泡排序从小到大完整排序
	start = clock();
	bubble_sort(bubble_arr, DATA_COUNT);
	end = clock();
	cost_time = (double)(end - start) / CLOCKS_PER_SEC;
	int bubble_ok = check_array_sort(bubble_arr, DATA_COUNT);
	printf("纯数组冒泡排序 (纯暴力 O(n*n))\n");
	if (bubble_ok) printf("排序完成，数组严格升序，无错误\n");
	else printf("冒泡排序失败，数组无序\n");
	printf("排序耗时：%.4lf 秒\n\n", cost_time);

	// 纯遍历找最小值对比
	Value heap_min, scan_min;
	find_min_by_scan(normal_arr, DATA_COUNT, &scan_min);
	heap_min = *show_array(heap, 0);
	printf("最小值一致性校验\n");
	if (heap_min.value == scan_min.value)
	{
		printf("小根堆和遍历找到的最小值一致\n");
	}
	else
	{
		printf("两个方法找到的最小值不一致！\n");
	}

	for (int i = 0; i < DATA_COUNT; i++) free(normal_arr[i]);
	free(normal_arr);
	free(select_arr);
	free(bubble_arr);
	free_Value_Array(heap);

	return 0;
}
*/