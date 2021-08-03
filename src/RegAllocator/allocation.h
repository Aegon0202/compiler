#ifndef ALLOCATION_H
#define ALLOCATION_H

#include "./lifeinterval.h"

#define MAX_NUM 1000

//基本框架
void walkIntervals(IntervalList* unhandled);

//将itList从A移动到B
void MoveitFromAtoB(list_entry_t* A, list_entry_t* B, IntervalList* itList);

//判断是否it是否包含position
int isCoverd(Interval* it, int position);

//能否直接分配一个空闲的寄存器
int tryAllocateFreeRegister(Interval* current, list_entry_t* active_list_head, list_entry_t* inactive_list_head);
#endif
