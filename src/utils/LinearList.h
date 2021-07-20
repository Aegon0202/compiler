#ifndef UTILS_LINEAR_LIST_H
#define UTILS_LINEAR_LIST_H

#include "./Item.h"
struct LinearListElem {
    union {
        struct LinearListElem** content;
        struct Item* item;
    };
    int rest_level;
};
struct LinearList {
    struct LinearListElem** content;
};

/**
 * 创建一个线性表
 * 
 * :param (int (*)(void*,void*)) key_equal 判断两个key是否相等的函数
 * 
 * :return (struct LinearList*) 一个指向LinearList的指针
 */
struct LinearList* newLinearList();

/**
 * 在LinearList中设定index以及对应的value
 * 
 * :param (struct LinearList*) linear 一个指向LinearList的指针
 * :param (unsigned long long int) index 元素的index
 * :param (void*) value 指向value的指针
 * 
 * :return (void*) 当key对应的位置有数据时，返回原value的指针。否则返回NULL
 */
void* setLinearList(struct LinearList* linear, unsigned long long int index, void* value);

/**
 * 在LinearList中获取index对应的value
 * 
 * :param (struct LinearList*) linear 一个指向LinearList的指针
 * :param (unsigned long long int) index 元素的index
 * 
 * :return (void*) 指向value的指针。
 *      当key对应的位置有数据时，返回原value的指针。否则返回NULL
 */
void* getLinearList(struct LinearList* linear, unsigned long long int index);

/**
 * 在LinearList中删除index对应的value
 * 
 * :param (struct LinearList*) linear 一个指向LinearList的指针
 * :param (unsigned long long int) index 元素的index
 * 
 * :return (void*) 指向value的指针。
 */
void* removeLinearList(struct LinearList* linear, unsigned long long int index);

/**
 * 在LinearList中删除线性表的一个元素
 * 
 * :param (struct LinearList*) linear 一个指向LinearList的指针
 * 
 * :return (void*) 指向value的指针。
 */
void* popLinearList(struct LinearList* linear);

/**
 * 释放当前LinearList
 * 要求当前LinearList为空
 * 
 * :param (struct LinearList**) linear_p 一个指向（指向LinearList的指针）的指针
 * 
 * :return (int) 0:释放成功
 *      -1： 表中还有数据
 */
int freeLinearList(struct LinearList** linear_p);

#endif