#define _CRT_SECURE_NO_WARNINGS 1
#ifndef PATH_DATA_GEN_H
#define PATH_DATA_GEN_H

/**
 * @brief 生成1000条路径数据（关联景点ID），写入指定文本文件
 * @param file_path 输出文件路径（如："./path_data.txt"）
 * @param min_spot_id 景点ID最小值（固定为1，和景点脚本一致）
 * @param max_spot_id 景点ID最大值（固定为1000，和景点脚本一致）
 * @return 成功返回0，失败返回-1
 * 数据约束：
 * 1. 起点ID/终点ID：1~1000（和景点ID完全重合，保证存在）
 * 2. 起点ID ≠ 终点ID
 * 3. 路径长度：10~99（>0的小整数，日常使用级）
 * 4. 格式：每行 "起点ID 终点ID 路径长度"（空格分隔，1000行）
 */
int gen_path_data_to_file(const char* file_path, int min_spot_id, int max_spot_id);

#endif // PATH_DATA_GEN_H