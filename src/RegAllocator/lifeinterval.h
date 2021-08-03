#ifndef LIFE_TIME_INTERVAL_H
#define LIFE_TIME_INTERVAL_H

#include "../utils/LinearList.h"
#include "../utils/link.h"

typedef struct Interval Interval;

typedef struct IntervalList {
    Interval* value;
    list_entry_t link;
} IntervalList;

typedef struct RangeList {
    int begin;
    int end;
    list_entry_t link;
} RangeList;

typedef struct usepositionList {
    int position;
    list_entry_t link;
} usepositionList;

typedef struct Interval {
    int reg_num;
    RangeList* range_list;        //range链表[4,8]->[12,16]->[20,24] 节点类型；Range
    usepositionList* usepostion;  //useposition链表4->12->16 节点类型:int
    struct Interval* split_parent;
    IntervalList* split_childer;  //分割子区间链表[child1]->[child2] 节点类型:Interval
    int phisical_reg;
    int childrenNum;
    int is_fixed;
} Interval;

Interval* getIntervalByVal(int reg_num);
Interval* getFixIntervalByReg(int reg_num);
RangeList* getFirstRange(Interval* interval);
RangeList* getLastRange(Interval* interval);
void build_interval(struct DequeList* start);
int isIntervalFix(Interval*);

#define le2IntervalList(elem) le2struct(elem, IntervalList, link)
#define le2RangeList(elem) le2struct(elem, RangeList, link)
#define le2UsePositionList(elem) le2struct(elem, usepositionList, link)
#endif