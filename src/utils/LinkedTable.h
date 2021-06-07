#ifndef UTILS_STRING_LINKED_TABLE_H
#define UTILS_STRING_LINKED_TABLE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./NullPointMacro.h"
#include "./PrintHelper.h"

struct Item {
    void* key;    // only supprt point
    void* value;  // only support point
};

struct LinkedElem {
    struct Item* item;
    struct LinkedElem* prev;
    struct LinkedElem* next;
};

// use key and value to find or store something
// key and value only support point
struct LinkedTable {
    int size;
    int (*key_equal)(void* key_1, void* key_2);
    struct LinkedElem* head;  // have head elem
};

struct LinkedElem* newLinkedElem(void* key, void* value, struct LinkedElem* prev);
void removeLinkedELem(struct LinkedElem* target);
struct LinkedElem* findLinkedElem(struct LinkedElem* head, void* key, int (*key_equal)(void*, void*));

/**
 * 创建一个链表
 * 
 * :param (int (*)(void*,void*)) key_equal 判断两个key是否相等的函数
 * 
 * :return (struct LinkedTable*) 一个指向LinkedTable的指针
 */
struct LinkedTable* newLinkedTable(int (*key_equal)(void*, void*));

/**
 * 在LinkedTable中设定 key以及对应的value
 * 
 * :param (struct LinkedTable*) linked_table 一个指向HashTable的指针
 * :param (void*) key 指向key的指针
 * :param (void*) value 指向value的指针
 * 
 * :return (void*) 当key对应的位置有数据时，返回原value的指针。否则返回NULL
 */
void* setLinkedTable(struct LinkedTable* linked_table, void* key, void* value);

/**
 * 在LinkedTable中获取key对应的value
 * 
 * :param (struct LinkedTable*) linked_table 一个指向HashTable的指针
 * :param (void*) key 指向key的指针
 * 
 * :return (void*) 指向value的指针。
 *      当key对应的位置有数据时，返回原value的指针。否则返回NULL
 */
void* getLinkedTable(struct LinkedTable* linked_table, void* key);

/**
 * 在LinkedTable中删除key对应的value
 * 
 * :param (struct LinkedTable*) linked_table 一个指向HashTable的指针
 * :param (void*) key 指向key的指针
 * 
 * :return (struct Item*) 指向item的指针。
 *      当key对应的位置有数据时，返回item的指针。否则返回NULL
 *      当不使用item指针指向的结构时，需要把内存free掉
 */
struct Item* removeLinkedTable(struct LinkedTable* linked_table, void* key);

/**
 * 在LinkedTable中删除链表末尾的元素
 * 
 * :param (struct LinkedTable*) linked_table 一个指向HashTable的指针
 * 
 * :return (struct Item*) 指向item的指针。
 *      当表中有数据时，返回item的指针。否则返回NULL
 *      当不使用item指针指向的结构时，需要把内存free掉
 */
struct Item* popLinkedTable(struct LinkedTable* linked_table);

/**
 * 释放当前LinkedTable
 * 要求当前LinkedTable为空
 * 
 * :param (struct LinkedTable**) linked_table_p 一个指向（指向LinkedTable的指针）的指针
 * 
 * :return (int) 0:释放成功
 *      -1： 表中还有数据
 */
int freeLinkedTable(struct LinkedTable** linked_table_p);
#endif