#include "allocation.h"

#include "../utils/Malloc.h"
#include "../utils/link.h"
#include "./lifeinterval.h"

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
        list_del(first);  //拿到并删除unhandleList的头结点
        int position = le2struct(list_next(currrent->value->range_list), RangeList, link)->begin;

        list_entry_t* active_list_tmp = list_next(active_list_head);

        while (active_list_tmp != active_list_head) {
            //遍历active_list
            IntervalList* itList = le2struct(active_list_tmp, IntervalList, link);
            Interval* it = itList->value;
            //获取当前Interval的最后一个range的后部区间
            int itposition = le2struct(list_next(it->range_list), RangeList, link)->begin;
            if (itposition < position) {
                active_list_tmp = list_next(active_list_tmp);
                MoveitFromAtoB(active_list_head, handled_list_head, itList);
            } else if (isCoverd(it, position)) {
                active_list_tmp = list_next(active_list_tmp);
                MoveitFromAtoB(active_list_head, inactive_list_head, itList);
            } else {
                active_list_tmp = list_next(active_list_tmp);
            }
        }

        list_entry_t* inactive_list_tmp = list_next(inactive_list_head);
        while (inactive_list_tmp != inactive_list_head) {
            //遍历active_list
            IntervalList* initList = le2struct(inactive_list_tmp, IntervalList, link);
            Interval* init = initList->value;
            //获取当前Interval的最后一个range的后部区间
            int initposition = le2struct(list_next(init->range_list), RangeList, link)->begin;
            if (initposition < position) {
                inactive_list_tmp = list_next(active_list_tmp);
                MoveitFromAtoB(active_list_head, handled_list_head, initList);
            } else if (isCoverd(init, position)) {
                inactive_list_tmp = list_next(active_list_tmp);
                MoveitFromAtoB(active_list_head, inactive_list_head, initList);
            } else {
                inactive_list_tmp = list_next(active_list_tmp);
            }
        }

        int isAllocatedFree = 0;
        //Try to Allocate Free Register for Current
    }
}

void MoveitFromAtoB(list_entry_t* A, list_entry_t* B, IntervalList* itList) {
    list_del(&itList->link);
    list_add_after(B, &itList->link);
}
int isCoverd(Interval* it, int position) {
    int flag = 0;
    list_entry_t* range_list_head = &it->range_list->link;
    list_entry_t* range_list_tmp = list_next(range_list_head);
    while (range_list_tmp != range_list_head) {
        int tmp_begin = le2struct(range_list_tmp, RangeList, link)->begin;
        int tmp_end = le2struct(range_list_tmp, RangeList, link);
        if (tmp_begin <= position && tmp_end > position) {
            flag = 1;
        }
    }
    return flag;
}

void allocate_blocked_reg(Interval* current, list_entry_t* active, list_entry_t* inactive) {
    int current_from = getFirstRange(current)->begin;
    list_entry_t* active_elem = list_next(active);
    while (active_elem != active) {
        Interval* active_elem_value = le2IntervalList(active_elem)->value;
        active_elem = list_next(active_elem);
        if (isIntervalFix(active_elem_value)) {
            set_block_pos(active_elem_value, 0);
            continue;
        }
        int n = getNextUsage(active_elem_value, current_from);
        set_use_pos(active_elem_value, n);
    }

    list_entry_t* inactive_elem = list_next(inactive);
    while (inactive_elem != inactive) {
        Interval* inactive_elem_value = le2IntervalList(inactive_elem)->value;
        inactive_elem = list_next(inactive_elem);
        if (isIntervalFix(inactive_elem_value)) {
            if (isIntersect(inactive_elem_value, current)) {
                int inter_pos = getNextIntersect(inactive_elem_value, current);
                set_block_pos(inactive_elem_value, inter_pos);
            }
            continue;
        }
        int n = getNextUsage(inactive_elem_value, current_from);
        set_use_pos(inactive_elem_value, n);
    }
}
int tryAllocateFreeRegister(Interval* current, list_entry_t* active_list_head, list_entry_t* inactive_list_head) {
    int free_pos[PHYSICAL_REGISTER_NUM];
    memset(free_pos, MAX_NUM, PHYSICAL_REGISTER_NUM * sizeof(int));

    list_entry_t* active_list_tmp = list_next(active_list_head);
    while (active_list_tmp != active_list_head) {
        IntervalList* itList = le2struct(active_list_tmp, IntervalList, link);
        Interval* it = itList->value;
        set_free_pos(it, 0);
    }

    list_entry_t* inactive_list_tmp = list_next(inactive_list_head);
    while (inactive_list_tmp != inactive_list_head) {
        IntervalList* initList = le2struct(inactive_list_tmp, IntervalList, link);
        Interval* init = initList->value;
        if (isIntersect(current, init)) {
            int inter_pos = getNextIntersect(current, init);
            set_free_pos(init, inter_pos);
        }
    }
