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
    struct LinearListElem* content;
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

/**
 * 在当前列表中进行map操作，返回一个新列表
 * map的范围是从begin_index 逐渐 + 1 到end_index。包含 begin_index，不包含end_index
 * 
 * :param (struct LinearList**) linear 一个指向LinearList的指针
 * :param (unsigned long long int) begin_index map开始的index
 * :param (unsigned long long int) end_index map结束的index
 * :param (void* (*)(void* value, void* args)) map_func 一个函数，第一个参数为表中的元素，第二个参数为args
 *                                                      返回处理后的元素(void*)放到新列表中。不能产生NULL
 * :param (int) is_local 如果为真，将产生的数据放回原位置。你需要在map_func中释放原来的元素占据的内存
 * :param (void*) args 传进map_func的第二个参数
 * 
 * :return (struct LinearList*) map后的新LinearList，范围是从begin_index(含)到end_index(不含)。
 */
struct LinearList* mapLinearList(struct LinearList* linear, unsigned long long int begin_index, unsigned long long int end_index, void* (*map_func)(void*, void*), int is_local, void* args);

/**
 * 在当前列表中进行reduce操作，返回一个 void*
 * reduce的范围是从begin_index 逐渐 + 1 到end_index。包含 begin_index，不包含end_index
 * 从begin_index开始reduce。即 
 * reduce_func(... reduce_func(reduce_func(init_value, linear[begin_index]),linear[begin_index+1]) ... , linear[end_index-1])
 * 
 * :param (struct LinearList**) linear 一个指向LinearList的指针
 * :param (unsigned long long int) begin_index reduce开始的index
 * :param (unsigned long long int) end_index reduce结束的index
 * :param (void* (*)(void* total, void* value, void* args)) reduce_func 一个函数，
 * 第一个参数为上一次reduce 的结果，第二个参数为本次调用时的value，第三个参数为args
 * 返回reduce的结果
 * :param (void*) init_value 第一次调用reduce 时的初始值
 * :param (void*) args 传进reduce_func的第三个参数
 * 
 * :return (void*) reduce的最终结果
 */
void* reduceLinearList(struct LinearList* linear, unsigned long long int begin_index, unsigned long long int end_index, void* (*reduce_func)(void*, void*, void*), void* init_value, void* args);

#endif
