#ifndef UTILS_HASH_TABLE_H
#define UTILS_HASH_TABLE_H
#include "./LinkedTable.h"

struct HashTable {
    int size;       // elem num
    int capacity;   // hash table size
    int threshold;  // if load fator over this, table resize to a new size (capacity * 2)
    int (*hash_key)(void* key);
    int (*key_equal)(void* key_1, void* key_2);
    struct LinkedTable** table;
};

/**
 * 创建一个哈希表
 * 
 * :param (int) capacity 容量
 * :param (int) threshold 负载系数的阈值
 * :param (int (*)(void*)) hash_key 由key去求hash值的函数
 * :param (int (*)(void*,void*)) key_equal 判断两个key是否相等的函数
 * 
 * :return (struct HashTable*) 一个指向HashTable的指针
 */
struct HashTable* newHashTable(int capacity, int threshold, int (*hash_key)(void* key), int (*key_equal)(void* key_1, void* key_2));

/**
 * 对HashTable进行缩放
 * 
 * :param (struct HashTable*) hash_table 一个指向HashTable的指针
 * :param (int) capacity 新的HashTable容量
 * 
 * :return void 没有返回值 
 */
void resizeHashTable(struct HashTable* hash_table, int capacity);

/**
 * 在HashTable中设定 key以及对应的value
 * 
 * :param (struct HashTable*) hash_table 一个指向HashTable的指针
 * :param (void*) key 指向key的指针
 * :param (void*) value 指向value的指针
 * 
 * :return (void*) 当key对应的位置有数据时，返回原value的指针。否则返回NULL
 */
void* setHashTable(struct HashTable* hash_table, void* key, void* value);

/**
 * 在HashTable中获取key对应的value
 * 
 * :param (struct HashTable*) hash_table 一个指向HashTable的指针
 * :param (void*) key 指向key的指针
 * 
 * :return (void*) 指向value的指针。
 *      当key对应的位置有数据时，返回原value的指针。否则返回NULL
 */
void* getHashTable(struct HashTable* hash_table, void* key);

/**
 * 在HashTable中删除key对应的value
 * 
 * :param (struct HashTable*) hash_table 一个指向HashTable的指针
 * :param (void*) key 指向key的指针
 * 
 * :return (struct Item*) 指向item的指针。
 *      当key对应的位置有数据时，返回item的指针。否则返回NULL
 *      当不使用item指针指向的结构时，需要把内存free掉
 */
struct Item* removeHashTable(struct HashTable* hash_table, void* key);

/**
 * 释放当前HashTable
 * 要求当前HashTable为空
 * 
 * :param (struct HashTable**) hash_table_p 一个指向（指向HashTable的指针）的指针
 * 
 * :return (int) 0:释放成功
 *      -1： 表中还有数据
 */
int freeHashTable(struct HashTable** hash_table_p);

#endif