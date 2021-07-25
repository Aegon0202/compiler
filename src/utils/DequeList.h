#ifndef UTILS_DEQUEUE_LIST_H
#define UTILS_DEQUEUE_LIST_H
#include "./LinearList.h"

struct DequeList {
    struct LinearList* content;
    unsigned long long int head;
    unsigned long long int tail;
};

struct DequeList* newDequeList();
int pushBackDequeList(struct DequeList* deque, void* value);
int pushFrontDequeList(struct DequeList* deque, void* value);
void* popBackDequeList(struct DequeList* deque);
void* popFrontDequeList(struct DequeList* deque);
void* getFrontDequeList(struct DequeList* deque);
void* getBackDequeList(struct DequeList* deque);
unsigned long long int sizeDequeList(struct DequeList* deque);
int isEmptyDequeList(struct DequeList* deque);
int freeDequeList(struct DequeList** deque_p);
#endif
