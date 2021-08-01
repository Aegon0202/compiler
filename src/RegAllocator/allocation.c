#include "../utils/Malloc.h"
#include "../utils/link.h"
#include "./lifeinterval.h"
#include "alloca.h"

void walkIntervals(IntervalList* unhandled_list) {
    list_entry_t* unhandled_list_head = &unhandled_list->link;
    list_init(unhandled_list_head);

    MALLOC(active_list, IntervalList, 1);
    MALLOC(inactive_list, IntervalList, 1);
    MALLOC(handled_list, IntervalList, 1)
    handled_list->value = 0;
    active_list->value = 0;
    inactive_list->value = 0;
    list_entry_t* active_list_head = &active_list->link;
    list_entry_t* inactive_list_head = &inactive_list->link;
    list_entry_t* handled_list_head = &handled_list->link;
    list_init(active_list_head);
    list_init(inactive_list_head);
    list_init(handled_list_head);

    //to be modified
    while (!list_empty(unhandled_list_head)) {
        list_entry_t* first = list_next(unhandled_list_head);
        IntervalList* currrent = le2struct(first, IntervalList, link);
        int position = le2struct(list_next(currrent->value->range_list), RangeList, link)->begin;
        while (!list_empty(active_list_head)) {
            //获取当前active_list第一个iterval
            IntervalList* itList = le2struct(list_next(active_list_head), IntervalList, link);
            Interval* it = itList->value;
            //获取当前Interval的最后一个range的后部区间
            int itposition = le2struct(list_next(it->range_list), RangeList, link)->begin;
            if (itposition < position) {
                MoveitFromAtoB(unhandled_list_head, active_list_head, itList);
            } else if (1) {
            }
        }
    }
}

void MoveitFromAtoB(list_entry_t* A, list_entry_t* B, IntervalList* itList) {
    list_del(&itList->link);
    list_add_after(B, &itList->link);
}