#define _CRT_SECURE_NO_WARNINGS 1
#ifndef SPOT_DATA_GEN_H
#define SPOT_DATA_GEN_H

/**
 * @brief 生成1000条符合要求的景点数据，写入指定文本文件
 * @param file_path 文本文件路径（如："./spot_data.txt"）
 * @return 成功返回0，失败返回-1
 * 数据约束：
 * 1. ID：1~1000（>0、int小值、日常使用级）
 * 2. 名称：无空格、长度<100字符（适配100字符数组）
 * 3. 描述：无空格、长度<1000字符（适配1000字符数组）
 * 4. 格式：每行 "id 名称 描述"（空格分隔，1000行）
 */
int gen_spot_data_to_file(const char* file_path);

#endif // SPOT_DATA_GEN_H