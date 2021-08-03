#ifndef ALLOCATION_H
#define ALLOCATION_H

#include "./lifeinterval.h"

#define MAX_NUM 1000
#define PHYSICAL_REGISTER_NUM 20

//基本框架
void walkIntervals(IntervalList* unhandled);

//将itList从A移动到B
void MoveitFromAtoB(list_entry_t* A, list_entry_t* B, IntervalList* itList);

int getPhisicalRegState(int reg, int pos);
void set_free_pos(Interval* interval, int pos);
void set_use_pos(Interval* interval, int pos);
void set_block_pos(Interval* interval, int pos);
//判断是否it是否包含position
int isCoverd(Interval* it, int position);

//能否直接分配一个空闲的寄存器
int tryAllocateFreeRegister(Interval* current, list_entry_t* active_list_head, list_entry_t* inactive_list_head);

//设置free_pos
void set_free_pos(int* free_pos, Interval* it, int value);

//计算current与inactiveList 中Interval的交点

int calculateIntersection(Interval* current)
#endif
