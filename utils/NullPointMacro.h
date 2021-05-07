#ifndef NULL_POINT_MACRO_H
#define NULL_POINT_MACRO_H

#define EnsureNotNull(p)                                                                 \
    do                                                                                   \
    {                                                                                    \
        if ((p == NULL))                                                                 \
        {                                                                                \
            fprintf(stderr, "NULL POINT " #p " AT FILE %s LINE %d", __FILE__, __LINE__); \
            exit(-1);                                                                    \
        }                                                                                \
    } while (0)

#define IfNotNull(p, stmt) \
    do                     \
    {                      \
        if (p != NULL)     \
            stmt           \
    } while (0)

#endif