#ifndef UTILS_NULL_POINT_MACRO_H
#define UTILS_NULL_POINT_MACRO_H
#include <stdio.h>

#include "PrintHelper.h"

/**
 * macro: 确保指针p非空
 * 如果指针p为空，打印对应的文件的名称和行号。并结束程序
 * （使用abort来尝试产生core dump）
 * 
 * :param 指针p
 */
#define EnsureNotNull(p)                                                           \
    do {                                                                           \
        if ((p) == NULL) {                                                         \
            PrintErrExit("NULL POINT " #p " AT FILE %s:%d\n", __FILE__, __LINE__); \
        }                                                                          \
    } while (0)

/**
 * macro: 如果指针p不为空，执行stmt对应的语句
 * 
 * :param p 指针p
 * :param stmt 一个合法的语句或一个合法的语句块
 * 
 * :example IfNotNull(p,return 0;);
 */
#define IfNotNull(p, stmt) \
    if ((p) != NULL)       \
    stmt

/**
 * macro: 如果指针p为空，执行stmt对应的语句
 * 
 * :param p 指针p
 * :param stmt 一个合法的语句或一个合法的语句块
 * 
 * :example IfNull(p,return 0;);
 */
#define IfNull(p, stmt) \
    if ((p) == NULL)    \
    stmt

/**
 * macro: 如果指针p为空，执行stmt1对应的语句，
 * 否则执行stmt2对应的语句
 * 
 * :param p 指针p
 * :param stmt1 一个合法的语句或一个合法的语句块
 * :param stmt2 一个合法的语句或一个合法的语句块
 * 
 * :example IfNullElse(p,return 0;,return 1;);
 */
#define IfNullElse(p, stmt1, stmt2) \
    if ((p) == NULL)                \
    stmt1 else stmt2

/**
 * macro: 如果指针不p为空，执行stmt1对应的语句，
 * 否则执行stmt2对应的语句
 * 
 * :param p 指针p
 * :param stmt1 一个合法的语句或一个合法的语句块
 * :param stmt2 一个合法的语句或一个合法的语句块
 * 
 * :example IfNotNullElse(p,return 0;,return 1;);
 */
#define IfNotNullElse(p, stmt1, stmt2) \
    if ((p) != NULL)                   \
    stmt1 else stmt2

#endif
