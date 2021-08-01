#ifndef LIFE_TIME_INTERVAL_H
#define LIFE_TIME_INTERVAL_h

#include "../utils/LinearList.h"
#include "../utils/link.h"

typedef struct Interval {
    int reg_num;
    list_entry_t* range_begin;  //range链表[4,8]->[12,16]->[20,24] 节点类型；Range
    list_entry_t* usepostion;   //useposition链表4->12->16 节点类型:int
    Interval* split_parent;
    list_entry_t* split_childer;  //分割子区间链表[child1]->[child2] 节点类型:Interva;
    int childrenNum;
} Interval;

typedef struct IntervalList {
    Interval* value;
    IntervalList* next;
} IntervalList;

typedef struct Range {
    int begin;
    int end;
} Range;

#endif