#include "../utils/Malloc.h"
#include "../utils/link.h"
#include "./lifeinterval.h"
#include "alloca.h"

void walkIntervals(IntervalList* unhandled_list) {
    list_entry_t* unhandled_list_head = &unhandled_list->link;
    list_init(unhandled_list_head);

    MALLOC(active_list, IntervalList, 1);
    MALLOC(inactive_list, IntervalList, 1);
    active_list->value = 0;
    inactive_list->value = 0;
    list_entry_t* active_list_head = &active_list->link;
    list_entry_t* inactive_list_head = &inactive_list->link;
    list_init(active_list_head);
    list_init(inactive_list_head);

    while (!list_empty(unhandled_list_head)) {
        list_entry_t* first = list_next(unhandled_list_head);
        IntervalList* currrent = le2struct(first, IntervalList, link);
        int position = le2struct(list_next(currrent->value->range_list), RangeList, link)->begin;
    }
}