#ifndef UTILS_PRINTHELPER_H
#define UTILS_PRINTHELPER_H

#define PrintErr(...) fprintf(stderr, __VA_ARGS__);
#define PrintErrExit(...)                                     \
    PrintErr("AT FILE %s: LINE %d:\r\n", __FILE__, __LINE__); \
    PrintErr(__VA_ARGS__)                                     \
        exit(-1);

#endif