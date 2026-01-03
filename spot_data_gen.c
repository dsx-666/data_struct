#define _CRT_SECURE_NO_WARNINGS 1
#include "spot_data_gen.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// 静态辅助函数：生成无空格的短字符串（内部使用）
static void gen_no_space_str(char* buf, int max_len, int id) {
    // 生成极简名称（无空格、长度<100，如：spot_1、spot_2）
    sprintf(buf, "spot_%d", id);
    // 强制截断（防止极端情况超长，适配100字符数组）
    if (strlen(buf) >= max_len) {
        buf[max_len - 1] = '\0';
    }
}

// 核心函数：生成1000条数据到文本文件
int gen_spot_data_to_file(const char* file_path) {
    // 初始化随机数（仅初始化一次）
    static int seed_init = 0;
    if (!seed_init) {
        srand((unsigned int)time(NULL));
        seed_init = 1;
    }

    // 打开文本文件（覆盖写入，不存在则创建）
    FILE* fp = fopen(file_path, "w");
    if (fp == NULL) {
        perror("生成数据失败：文件打开失败");
        return -1;
    }

    // 生成1000条数据（ID从1到1000，小数值、>0）
    for (int id = 1; id <= 1000; id++) {
        // 1. 生成名称（无空格、<100字符）
        char name[100] = { 0 };
        gen_no_space_str(name, 99, id); // 留1位给字符串结束符

        // 2. 生成描述（无空格、<1000字符，极简版：desc_spot_1）
        char desc[1000] = { 0 };
        sprintf(desc, "desc_%s", name); // 基于名称扩展，长度远小于1000

        // 3. 写入文本文件（每行：id 名称 描述）
        fprintf(fp, "%d %s %s\n", id, name, desc);
    }

    // 关闭文件
    fclose(fp);
    printf("1000条数据已生成到文本文件：%s\n", file_path);
    return 0;
}