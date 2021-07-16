#ifndef SKIPLIST_H
#define SKIPLIST_H
#include "./LinkedTable.h"
#include "./NullPointMacro.h"
#include "./PrintHelper.h"

struct SkipListNode {
    struct Item* item;
    int level;
    struct SkipListNode** next;
};

struct SkipList {
    double p;
    int max_level;
    // key1 < key2 ? -1 :
    // key1 > key2 ? 1
    // : 0
    int (*key_compare)(void* key1, void* key2);
    struct SkipListNode* head;
};

#endif