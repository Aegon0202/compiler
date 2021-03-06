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
    value = setLinearList(deque->content, deque->tail, value);
    IfNotNull(value, PrintErrExit("Must Be Empty"));
    deque->tail--;
    return 0;
}

int pushFrontDequeList(struct DequeList* deque, void* value) {
    EnsureNotNull(deque);
    EnsureNotNull(value);
    IfNull(deque, return -1;);
    IfNull(deque->content, return -1;);
    if (deque->head + 1 == deque->tail) {
        return -1;
    }
    value = setLinearList(deque->content, deque->head, value);
    IfNotNull(value, PrintErrExit("Must Be Empty"));
    deque->head++;
    return 0;
}

void* popBackDequeList(struct DequeList* deque) {
    EnsureNotNull(deque);
    EnsureNotNull(deque->content);
    if (deque->head - 1 == deque->tail) {
        return NULL;
    }
    void* value = removeLinearList(deque->content, deque->tail + 1);
    deque->tail++;
    return value;
}

void* popFrontDequeList(struct DequeList* deque) {
    EnsureNotNull(deque);
    EnsureNotNull(deque->content);
    if (deque->head - 1 == deque->tail) {
        return NULL;
    }
    void* value = removeLinearList(deque->content, deque->head - 1);
    deque->head--;
    return value;
}

void* getBackDequeList(struct DequeList* deque) {
    EnsureNotNull(deque);
    EnsureNotNull(deque->content);
    if (deque->head - 1 == deque->tail) {
        return NULL;
    }
    void* value = getLinearList(deque->content, deque->tail + 1);
    return value;
}

void* getFrontDequeList(struct DequeList* deque) {
    EnsureNotNull(deque);
    EnsureNotNull(deque->content);
    if (deque->head - 1 == deque->tail) {
        return NULL;
    }
    void* value = getLinearList(deque->content, deque->head - 1);
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

void* getDequeList(struct DequeList* deque, unsigned long long int index) {
    EnsureNotNull(deque);
    EnsureNotNull(deque->content);
    unsigned long long int size = deque->head - deque->tail + UINT64_MAX;
    if (index >= size) {
        return NULL;
    }
    return getLinearList(deque->content, deque->tail + index + 1);
}

int freeDequeList(struct DequeList** deque_p) {
    IfNull(deque_p, return 0;);
    struct DequeList* deque = *deque_p;
    IfNull(deque, return 0;);
    if (deque->content != NULL) {
        if (isEmptyDequeList(deque)) {
            freeLinearList(&(deque->content));
            deque->content = NULL;
        } else {
            return -1;
        }
    }
    deque->head = 0;
    deque->tail = 0;
    free(deque);
    *deque_p = NULL;
    return 0;
}

struct LinearList* convertToLinearList(struct DequeList* queue, unsigned long long int* num) {
    unsigned long long int i = 0;
    struct LinearList* linear = newLinearList();
    void* value = NULL;
    while ((value = popBackDequeList(queue))) {
        setLinearList(linear, i, value);
        i++;
    }
    freeDequeList(&queue);
    *num = i;
    return linear;
}

void freeFuncDequeList(struct DequeList* queue, void (*free_func)(void*, void*), void* args) {
    freeFuncLinearList(queue->content, free_func, args);
    free(queue);
}