#include "./DequeList.h"

#include <stdint.h>

#include "./Malloc.h"
#define OPERATE_SUCCESS 0
#define OPERATE_FAILURE -1
struct DequeList* newDequeList() {
    MALLOC(deque, struct DequeList, 1);
    deque->content = newLinearList();
    deque->head = 1;
    deque->tail = 0;
    return deque;
}

int pushBackDequeList(struct DequeList* deque, void* value) {
    EnsureNotNull(value);
    IfNull(deque, return -1;);
    IfNull(deque->content, return -1;);
    if (deque->head + 1 == deque->tail) {
        return -1;
    }
    void* value = setLinearList(deque, deque->tail, value);
    IfNull(value, PrintErrExit("Must Be Empty"));
    deque->tail--;
    return 0;
}

int pushFrontDequeList(struct DequeList* deque, void* value) {
    EnsureNotNull(value);
    IfNull(deque, return -1;);
    IfNull(deque->content, return -1;);
    if (deque->head + 1 == deque->tail) {
        return -1;
    }
    void* value = setLinearList(deque, deque->head, value);
    IfNull(value, PrintErrExit("Must Be Empty"));
    deque->head++;
    return 0;
}

void* popBackDequeList(struct DequeList* deque) {
    EnsureNotNull(deque);
    EnsureNotNull(deque->content);
    if (deque->head - 1 == deque->tail) {
        return NULL;
    }
    void* value = removeLinearList(deque, deque->tail + 1);
    deque->tail++;
    return value;
}

void* popFrontDequeList(struct DequeList* deque) {
    EnsureNotNull(deque);
    EnsureNotNull(deque->content);
    if (deque->head - 1 == deque->tail) {
        return NULL;
    }
    void* value = removeLinearList(deque, deque->head - 1);
    deque->head--;
    return value;
}

unsigned long long int sizeDequeList(struct DequeList* deque) {
    EnsureNotNull(deque);
    EnsureNotNull(deque->content);
    return deque->head - deque->tail + UINT64_MAX;
}

int isEmptyDequeList(struct DequeList* deque) {
    EnsureNotNull(deque);
    EnsureNotNull(deque->content);
    return deque->head - 1 == deque->tail;
}

int freeDequeList(struct DequeList** deque_p) {
    IfNull(deque_p, return NULL;);
    struct DequeList* deque = *deque_p;
    IfNull(deque, return NULL);
    IfNotNull(deque->content, {
        if (isEmptyDequeList(deque)) {
            free(deque->content);
        } else {
            return -1;
        }
    });
    deque->content = NULL;
    deque->head = 0;
    deque->tail = 0;
    free(deque);
    *deque_p = NULL;
    return 0;
}