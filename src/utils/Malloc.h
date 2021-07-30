#ifndef UTILS_MALLOC_H
#define UTILS_MALLOC_H
#include <stdlib.h>
#include <string.h>

#include "./NullPointMacro.h"
#define MALLOC(varname, type, num)                       \
    type* varname = (type*)malloc(sizeof(type) * (num)); \
    EnsureNotNull(varname);                              \
    memset(varname, 0, sizeof(type) * (num));

#define MALLOC_WITHOUT_DECLARE(varname, type, num)   \
    (varname) = (type*)malloc(sizeof(type) * (num)); \
    EnsureNotNull(varname);                          \
    memset(varname, 0, sizeof(type) * (num));

#endif
