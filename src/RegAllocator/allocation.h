#ifndef ALLOCATION_H
#define ALLOCATION_H

#include "./lifeinterval.h"

void walkIntervals(IntervalList* unhandled);
void MoveitFromAtoB(list_entry_t* A, list_entry_t* B, IntervalList* itList);
int isCover(Interval* it, int position);

int getPhisicalRegState(int reg, int pos);
void set_free_pos(Interval* interval, int pos);
void set_use_pos(Interval* interval, int pos);
void set_block_pos(Interval* interval, int pos);
#endif
