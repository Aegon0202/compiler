#ifndef ALLOCATION_H
#define ALLOCATION_H

#include "./lifeinterval.h"

void walkIntervals(IntervalList* unhandled);
void MoveitFromAtoB(list_entry_t* A, list_entry_t* B, IntervalList* itList);
int isCover(Interval* it, int position);

#endif
