#include "./SkipList.h"

#include <assert.h>

#include "./Malloc.h"

struct SkipListNode* newSkipListNode(int level_num, void* key, void* value) {
    struct SkipListNode* node = (struct SkipListNode*)malloc(sizeof(struct SkipListNode));
    EnsureNotNull(node);
    node->item = (struct Item*)malloc(sizeof(struct Item));
    EnsureNotNull(node->item);
    node->next = (struct SkipListNode**)malloc(sizeof(struct SkipListNode*) * level_num);
    EnsureNotNull(node->next);
    memset(node->next, 0, sizeof(struct SkipListNode*) * level_num);

    node->item->key = key;
    node->item->value = value;
    node->level = level_num;
    return node;
}

struct Item* freeSkipListNode(struct SkipListNode** node) {
    struct Item* item = (*node)->item;
    free((*node)->next);
    free(*node);
    *node = NULL;
    return item;
}

struct SkipList* newSkipList(int max_level, double p, int (*key_compare)(void*, void*)) {
    struct SkipList* list = (struct SkipList*)malloc(sizeof(struct SkipList));
    EnsureNotNull(list);
    list->key_compare = key_compare;
    list->max_level = max_level;
    list->p = p;
    list->head = newSkipListNode(max_level, NULL, NULL);
    struct SkipListNode* node = newSkipListNode(max_level, NULL, NULL);
    for (int i = 0; i < list->max_level; i++) {
        list->head->next[i] = node;
    }
    return list;
}

double rand_0_1() {
    return (double)rand() / (double)RAND_MAX;
}

struct SkipListNode* findSkipListNode(struct SkipList* list, void* key) {
    EnsureNotNull(list);
    EnsureNotNull(key);
    struct SkipListNode* node = list->head;
    for (int i = 0; i < node->level;) {
        if (node->next[i]->item->key == NULL) {
            i++;
            continue;
        }
        int com_r = list->key_compare(key, node->next[i]->item->key);
        if (com_r == 0) {
            return node->next[i];
        } else if (com_r < 0) {
            i++;
        } else if (com_r > 0) {
            node = node->next[i];
            i = 0;
        }
    }
    return NULL;
}

void* setSkipList(struct SkipList* list, const void* key, void* value) {
    EnsureNotNull(list);
    EnsureNotNull(key);
    EnsureNotNull(value);
    int level = 1;
    while (rand_0_1() < list->p && level < list->max_level) {
        level++;
    }

    struct SkipListNode* node = findSkipListNode(list, key);
    if (node != NULL) {
        void* r_value = node->item->value;
        node->item->value = value;
        return r_value;
    }

    MALLOC(update, struct SkipListNode*, list->max_level);
    EnsureNotNull(update);

    free(update);
    return NULL;
}