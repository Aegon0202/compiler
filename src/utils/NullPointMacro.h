#ifndef NULL_POINT_MACRO_H
#define NULL_POINT_MACRO_H
#include <stdio.h>
#define EnsureNotNull(p)                                                                   \
    do                                                                                     \
    {                                                                                      \
        if ((p) == NULL)                                                                   \
        {                                                                                  \
            fprintf(stderr, "NULL POINT " #p " AT FILE %s LINE %d\n", __FILE__, __LINE__); \
            exit(-1);                                                                      \
        }                                                                                  \
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