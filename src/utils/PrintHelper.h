#ifndef UTILS_PRINTHELPER_H
#define UTILS_PRINTHELPER_H
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * macro: 输出对应信息到stderr，使用方式类似于printf
 * 等价于fprintf(stderr,...)
 */
#define PrintErr(...) fprintf(stderr, __VA_ARGS__);

/**
 * macro: 输出对应信息到stderr，附带信息产生的文件名和行号
 * 终止此程序，尝试产生（core dump）文件
 * 使用方式类似于printf
 */
#define PrintErrExit(...)                                   \
    {                                                       \
        PrintErr("\nAT FILE %s:%d:\n", __FILE__, __LINE__); \
        PrintErr(__VA_ARGS__);                              \
        PrintErr("\nCREATE CORE DUMP.\n");                  \
        raise(SIGABRT);                                     \
    }

/**
 * 将部分enum转成文本（内部使用switch-case实现的）
 * 需要扩充就要修改此函数
 * 
 * :param (int) type enum数字或名称
 */
const char *EnumTypeToString(int type);

#endif