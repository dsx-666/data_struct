#pragma once
extern const char FILE_NAME[];
extern const char* result;
// 读取配置参数 int
int read_config_int(const char* file_name, const char* var, int* ans);

// 读取配置参数 char*
int read_config_char(const char* file_name, const char* var, char* ans);
// 修改配置文件 整型
int change_config_int(const char* file_name, const char* var, int new_val);

// 修改配置文件 字符串
int change_config_char(const char* file_name, const char* var, const char* new_val);




