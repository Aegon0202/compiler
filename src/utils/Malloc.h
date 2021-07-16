#ifndef MALLOC_H
#define MALLOC_H
#include <stdlib.h>
#define MALLOC(varname, type, num) type* varname = (type*)malloc(sizeof(type) * (num))

#endif