#include "allocation.h"

#include "../utils/Malloc.h"
#include "../utils/link.h"
#include "./lifeinterval.h"
#include "LRA.h"
#define PHISICAL_REG_NUM 15
int free_pos[PHISICAL_REG_NUM];
int use_pos[PHISICAL_REG_NUM];
int blocked_pos[PHISICAL_REG_NUM];

void set_use_pos(Interval* interval, int pos) {
    int reg = getInterval_assigned_reg(interval);
    use_pos[reg] = pos < use_pos[reg] ? pos : use_pos[reg];
}
void set_free_pos(Interval* interval, int pos) {
    int reg = getInterval_assigned_reg(interval);
    free_pos[reg] = pos < free_pos[reg] ? pos : free_pos[reg];
}
void set_blocked_pos(Interval* interval, int pos) {
    int reg = getInterval_assigned_reg(interval);
    blocked_pos[reg] = pos < blocked_pos[reg] ? pos : blocked_pos[reg];
}

int get_max_pos(int array[]) {
    int ans = -1;
    int max_num = -1;
    for (int i = 0; i < PHISICAL_REG_NUM; i++) {
        if (array[i] > max_num) {
            max_num = array[i];
            ans = i;
        }
    }
    return ans;
}
int get_max_free_pos() {
    return get_max_pos(free_pos);
}
int get_max_use_pos() {
    return get_max_pos(use_pos);
}

void __init_unhandled_list(list_entry_t* unhandled_head, struct DequeList* blocks) {
}

void walkIntervals(struct DequeList* blocks) {
    MALLOC(active_list, IntervalList, 1);
    MALLOC(inactive_list, IntervalList, 1);
    MALLOC(handled_list, IntervalList, 1);
    MALLOC(unhandled_list, IntervalList, 1);
    handled_list->value = 0;
    active_list->value = 0;
    inactive_list->value = 0;
    list_entry_t* unhandled_list_head = &unhandled_list->link;
    list_entry_t* active_list_head = &active_list->link;
    list_entry_t* inactive_list_head = &inactive_list->link;
    list_entry_t* handled_list_head = &handled_list->link;
    list_init(unhandled_list_head);
    list_init(active_list_head);
    list_init(inactive_list_head);
    list_init(handled_list_head);
    __init_unhandled_list(unhandled_list_head, blocks);
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

void allocate_blocked_reg(Interval* current, list_entry_t* active, list_entry_t* inactive, list_entry_t* unhandled) {
    memset(blocked_pos, 0x3f, sizeof blocked_pos);
    memset(use_pos, 0x3f, sizeof use_pos);

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
    int reg = get_max_use_pos();
    int current_first_usePos = getFirstUsePos(current);
    if (use_pos[reg] < current_first_usePos) {
        //assign spill slot to current
        //split current before first usePos
        int optimal_pos = getOptimalPos(getFirstUsePos(current));
        splitInterval(current, optimal_pos, unhandled);

        //add to unhandle
    } else if (blocked_pos[reg] > getLastRange(current)->end) {
        assign_reg2interval(current, reg);
        //spill and split it in active and inactive
        active_elem = list_next(active);
        while (active_elem != active) {
            Interval* it = le2IntervalList(active_elem);
            active_elem = list_next(active);
            makeRoomForCurrent(current, it, unhandled);
        }
        inactive_elem = list_next(inactive);
        while (inactive_elem != inactive) {
            Interval* it = le2IntervalList(inactive_elem);
            inactive_elem = list_next(inactive_elem);
            if (!isIntersect(it, current)) continue;
            makeRoomForCurrent(current, it, unhandled);
        }
    } else {
        //spill current
        Interval* child_interval;
        int current_split = getOptimalPos(blocked_pos[reg]);
        splitInterval(current, current_split, unhandled);

        //split it
        active_elem = list_next(active);
        while (active_elem != active) {
            Interval* it = le2IntervalList(active_elem);
            active_elem = list_next(active);
            makeRoomForCurrent(current, it, unhandled);
        }
        inactive_elem = list_next(inactive);
        while (inactive_elem != inactive) {
            Interval* it = le2IntervalList(inactive_elem);
            inactive_elem = list_next(inactive_elem);
            if (!isIntersect(it, current)) continue;
            makeRoomForCurrent(current, it, unhandled);
        }
    }
}

int tryAllocateFreeRegister(Interval* current, list_entry_t* active_list_head, list_entry_t* inactive_list_head, list_entry_t* unhandled) {
    memset(free_pos, 0x3f, sizeof free_pos);

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
    int reg = get_max_free_pos();

    if (free_pos[reg] == 0)
        return 0;
    else if (free_pos[reg] > getLastRange(current)->end) {
        assign_reg2interval(current, reg);
    } else {
        assign_reg2interval(current, reg);
        int optimal_pos = getOptimalPos(free_pos[reg]);
        splitInterval(current, optimal_pos, unhandled);
        //add to unhandle
    }
}