#ifndef UTILS_NULL_POINT_MACRO_H
#define UTILS_NULL_POINT_MACRO_H
#include <stdio.h>

#include "PrintHelper.h"
#define EnsureNotNull(p)                                                           \
    do {                                                                           \
        if ((p) == NULL) {                                                         \
            PrintErrExit("NULL POINT " #p " AT FILE %s:%d\n", __FILE__, __LINE__); \
        }                                                                          \
    } while (0)

#define IfNotNull(p, stmt) \
    if ((p) != NULL)       \
    stmt

#define IfNull(p, stmt) \
    if ((p) == NULL)    \
    stmt

#define IfNullElse(p, stmt1, stmt2) \
    if ((p) == NULL)                \
    stmt1 else stmt2

#define IfNotNullElse(p, stmt1, stmt2) \
    if ((p) != NULL)                   \
    stmt1 else stmt2

#endif