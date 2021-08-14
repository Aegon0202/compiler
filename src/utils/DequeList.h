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

/**
 * 从DequeList中获取 deque->tail + index + 1 位置的元素
 * 即index的取值范围是 [0, size)
 * DequeList中 head 和 tail 指向的都是空块
 * 返回对应元素位置的指针
 */
void* getDequeList(struct DequeList* deque, unsigned long long int index);

/**
 * 将DequeList转成LinearList，并且释放掉原有的DequeList。
 * 元素存放在[0,num-1]中
 * 
 * :param (struct DequeList*) deque 要转化的DequeList
 * :param (unsigned long long int*) 生成的LinearList中的元素个数
 * 
 * :return (struct LinearList*) 生成的LinearList，是新创建的，不需要时记得释放。
 */
struct LinearList* convertToLinearList(struct DequeList* queue, unsigned long long int* num);

void freeFuncDequeList(struct DequeList* queue, void (*free_func)(void*, void*), void* args);
#endif
