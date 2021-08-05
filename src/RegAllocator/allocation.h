#ifndef ALLOCATION_H
#define ALLOCATION_H

#include "./lifeinterval.h"

//基本框架
void walkIntervals(struct DequeList* unhandled);

//将itList从A移动到B
void MoveitFromAtoB(list_entry_t* A, list_entry_t* B, IntervalList* itList);
int getPhisicalRegState(int reg, int pos);
void set_free_pos(Interval* interval, int pos);
void set_use_pos(Interval* interval, int pos);
void set_block_pos(Interval* interval, int pos);
//判断是否it是否包含position

#endif
