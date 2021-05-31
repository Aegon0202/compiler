#ifndef UTILS_PRINTHELPER_H
#define UTILS_PRINTHELPER_H
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define PrintErr(...) fprintf(stderr, __VA_ARGS__);
#define PrintErrExit(...)                                   \
    {                                                       \
        PrintErr("\nAT FILE %s:%d:\n", __FILE__, __LINE__); \
        PrintErr(__VA_ARGS__);                              \
        PrintErr("\nCREATE CORE DUMP.\n");                  \
        raise(SIGABRT);                                     \
    }

const char *EnumTypeToString(int type);

#endif