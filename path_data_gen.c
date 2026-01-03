#define _CRT_SECURE_NO_WARNINGS 1
#include "path_data_gen.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 核心函数：生成路径数据（关联景点ID）
int gen_path_data_to_file(const char* file_path, int min_spot_id, int max_spot_id) {
    // 初始化随机数（仅初始化一次）
    static int seed_init = 0;
    if (!seed_init) {
        srand((unsigned int)time(NULL));
        seed_init = 1;
    }

    // 校验景点ID范围（防止传入非法值）
    if (min_spot_id >= max_spot_id) {
        printf("错误：景点ID最小值不能大于等于最大值！\n");
        return -1;
    }

    // 打开文本文件（覆盖写入）
    FILE* fp = fopen(file_path, "w");
    if (fp == NULL) {
        perror("生成路径数据失败：文件打开失败");
        return -1;
    }

    // 生成1000条路径数据
    for (int i = 0; i < 1000; i++) {
        // 1. 生成起点ID（1~1000）
        int id1 = rand() % (max_spot_id - min_spot_id + 1) + min_spot_id;

        // 2. 生成终点ID（1~1000，且≠起点ID）
        int id2;
        do {
            id2 = rand() % (max_spot_id - min_spot_id + 1) + min_spot_id;
        } while (id2 == id1); // 确保id1≠id2

        // 3. 生成路径长度（10~99，>0的小整数）
        int len = rand() % 90 + 10; // 10~99

        // 4. 写入文件（格式：id1 id2 len）
        fprintf(fp, "%d %d %d\n", id1, id2, len);
    }

    // 关闭文件
    fclose(fp);
    printf("1000条路径数据已生成到：%s\n", file_path);
    return 0;
}