#include "allocation.h"

#include "../utils/Malloc.h"
#include "../utils/link.h"
#include "./lifeinterval.h"
#include "./moveResolver.h"
#include "LRA.h"

#define PHISICAL_REG_NUM 10
int free_pos[PHISICAL_REG_NUM];
int use_pos[PHISICAL_REG_NUM];
int blocked_pos[PHISICAL_REG_NUM];

int tryAllocateFreeRegister(Interval* current, list_entry_t* active_list_head, list_entry_t* inactive_list_head, list_entry_t* unhandled, struct DequeList* blocks);
void allocate_blocked_reg(Interval* current, list_entry_t* active, list_entry_t* inactive, list_entry_t* unhandled, struct DequeList* blocks);
void __insert_interval_to_unhandled(Interval* it, list_entry_t* unhandled_list);

void set_use_pos(Interval* interval, int pos) {
    int reg = getInterval_assigned_reg(interval);
    use_pos[reg] = pos < use_pos[reg] ? pos : use_pos[reg];
}
void set_free_pos(Interval* interval, int pos) {
    int reg = getInterval_assigned_reg(interval);
    free_pos[reg] = pos < free_pos[reg] ? pos : free_pos[reg];
}
void set_block_pos(Interval* interval, int pos) {
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

void __init_unhandled_list(void* value, void* args) {
    Interval* it = (Interval*)value;
    list_entry_t* head = (list_entry_t*)args;
    if (it->is_fixed)
        return;
    __insert_interval_to_unhandled(it, head);
}

void walkIntervals(struct DequeList* blocks) {
    struct MoveSolver* ms = newMoveSolver();
    int last_position = -1;
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
    forEachLinearList(reg2Intival, __init_unhandled_list, unhandled_list_head);
    //__init_unhandled_list(unhandled_list_head, blocks);
    //to be modified

    for (int i = 0; i < PHISICAL_REG_NUM; i++) {
        MALLOC(l, IntervalList, 1);
        l->value = getIntervalByVal(i);
        list_add_before(inactive_list_head, &l->link);
    }

    while (!list_empty(unhandled_list_head)) {
        list_entry_t* first = list_next(unhandled_list_head);
        IntervalList* current = le2struct(first, IntervalList, link);
        list_del(first);  //拿到并删除unhandleList的头结点
        int position = le2struct(list_next(&current->value->range_list->link), RangeList, link)->begin;
        // position += (position & 0x1);

        list_entry_t* active_list_tmp = list_next(active_list_head);

        while (active_list_tmp != active_list_head) {
            //遍历active_list
            IntervalList* itList = le2struct(active_list_tmp, IntervalList, link);
            Interval* it = itList->value;
            //获取当前Interval的最后一个range的后部区间
            int itposition = le2struct(list_prev(&it->range_list->link), RangeList, link)->end;
            if (itposition < position) {
                active_list_tmp = list_next(active_list_tmp);
                MoveitFromAtoB(active_list_head, handled_list_head, itList);
            } else if (!isCoverd(it, position)) {
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
            int initposition = le2struct(list_prev(&init->range_list->link), RangeList, link)->end;
            if (initposition < position) {
                inactive_list_tmp = list_next(inactive_list_tmp);
                MoveitFromAtoB(inactive_list_head, handled_list_head, initList);
            } else if (isCoverd(init, position)) {
                inactive_list_tmp = list_next(inactive_list_tmp);
                MoveitFromAtoB(inactive_list_head, active_list_head, initList);
            } else {
                inactive_list_tmp = list_next(inactive_list_tmp);
            }
        }

        int isAllocatedFree = tryAllocateFreeRegister(current->value, active_list_head, inactive_list_head, unhandled_list_head, blocks);
        if (!isAllocatedFree) {
            allocate_blocked_reg(current->value, active_list_head, inactive_list_head, unhandled_list_head, blocks);
        }
        if (current->value->phisical_reg >= 0) {
            list_add_before(active_list_head, &current->link);
        } else {
            list_entry_t* head = &current->value->usepostion->link;
            ASSERT(head == head->next);
        }

        if (last_position == -1) {
            last_position = position;
        } else {
            if (last_position != position) {
                solveMove(ms);
                last_position = position;
                setAddBeforeEntry(getArmIrByOpid(blocks, position), ms);
            }
            Interval* it = current->value;
            Interval* parent = it->split_parent;
            if (parent != NULL) {
                Interval* from = NULL;
                list_entry_t* prev = NULL;
                list_entry_t* head = &parent->split_childer->link;
                list_entry_t* elem = list_next(head);
                while (head != elem) {
                    if (le2IntervalList(elem)->value == it) {
                        prev = list_prev(elem);
                    }
                    elem = list_next(elem);
                }
                if (prev == head) {
                    from = parent;
                } else {
                    from = le2IntervalList(prev)->value;
                }

                if (from->phisical_reg != it->phisical_reg) {
                    if (le2struct(ms->add_before_entry, struct ArmIr, ir_link)->type != ARM_LABEL) {
                        addIntervalMap(from, it, ms);
                    }
                }
            }
        }
    }

    freeMoveSolver(ms);
}

void MoveitFromAtoB(list_entry_t* A, list_entry_t* B, IntervalList* itList) {
    list_del(&itList->link);
    list_add_after(B, &itList->link);
}

void allocate_blocked_reg(Interval* current, list_entry_t* active, list_entry_t* inactive, list_entry_t* unhandled, struct DequeList* blocks) {
    memset(blocked_pos, 0x3f, sizeof blocked_pos);
    memset(use_pos, 0x3f, sizeof use_pos);

    int current_from = getFirstRange(current)->begin;
    list_entry_t* active_elem = list_next(active);
    while (active_elem != active) {
        Interval* active_elem_value = le2IntervalList(active_elem)->value;
        active_elem = list_next(active_elem);
        if (isIntervalFixed(active_elem_value)) {
            set_block_pos(active_elem_value, 0);
            set_use_pos(active_elem_value, 0);
            continue;
        }
        int n = getNextUsage(active_elem_value, current_from);
        set_use_pos(active_elem_value, n);
    }

    list_entry_t* inactive_elem = list_next(inactive);
    while (inactive_elem != inactive) {
        Interval* inactive_elem_value = le2IntervalList(inactive_elem)->value;
        inactive_elem = list_next(inactive_elem);
        if (isIntervalFixed(inactive_elem_value)) {
            if (isIntervalsect(inactive_elem_value, current)) {
                int inter_pos = getNextIntersect(inactive_elem_value, current);
                set_block_pos(inactive_elem_value, inter_pos);
                set_use_pos(inactive_elem_value, inter_pos);
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
        current->phisical_reg = -1;
        if (current_first_usePos != 0x3f3f3f3f) {
            int optimal_pos = getOptimalPos(getFirstRange(current)->begin + 1, getFirstUsePos(current) - 1, blocks);

            Interval* child = splitInterval(current, optimal_pos);
            __insert_interval_to_unhandled(child, unhandled);
            child->is_spilled = 1;
        }

    } else if (blocked_pos[reg] > getLastRange(current)->end) {
        assign_reg2interval(current, reg);
        //spill and split it in active and inactive
        active_elem = list_next(active);
        while (active_elem != active) {
            Interval* it = le2IntervalList(active_elem)->value;
            active_elem = list_next(active_elem);
            if (it->phisical_reg == reg)
                makeRoomForCurrent(current, it, unhandled, blocks);
        }
        inactive_elem = list_next(inactive);
        while (inactive_elem != inactive) {
            Interval* it = le2IntervalList(inactive_elem)->value;
            inactive_elem = list_next(inactive_elem);
            if (!isIntervalsect(it, current) || it->phisical_reg != reg) continue;
            makeRoomForCurrent(current, it, unhandled, blocks);
        }
    } else {
        //spill current
        current->phisical_reg = reg;
        int current_split = getOptimalPos(getFirstRange(current)->begin + 1, blocked_pos[reg] - 1, blocks);
        Interval* child = splitInterval(current, current_split);
        __insert_interval_to_unhandled(child, unhandled);

        child->is_spilled = 2;
        //split it
        active_elem = list_next(active);
        while (active_elem != active) {
            Interval* it = le2IntervalList(active_elem)->value;
            active_elem = list_next(active);
            if (it->phisical_reg == reg)
                makeRoomForCurrent(current, it, unhandled, blocks);
        }
        inactive_elem = list_next(inactive);
        while (inactive_elem != inactive) {
            Interval* it = le2IntervalList(inactive_elem)->value;
            inactive_elem = list_next(inactive_elem);
            if (!isIntervalsect(it, current) || it->reg_num != reg) continue;
            makeRoomForCurrent(current, it, unhandled, blocks);
        }
    }
}

int tryAllocateFreeRegister(Interval* current, list_entry_t* active_list_head, list_entry_t* inactive_list_head, list_entry_t* unhandled, struct DequeList* blocks) {
    memset(free_pos, 0x3f, sizeof free_pos);

    list_entry_t* active_list_tmp = list_next(active_list_head);
    while (active_list_tmp != active_list_head) {
        IntervalList* itList = le2struct(active_list_tmp, IntervalList, link);
        Interval* it = itList->value;
        set_free_pos(it, 0);
        active_list_tmp = list_next(active_list_tmp);
    }

    list_entry_t* inactive_list_tmp = list_next(inactive_list_head);
    while (inactive_list_tmp != inactive_list_head) {
        IntervalList* initList = le2struct(inactive_list_tmp, IntervalList, link);
        Interval* init = initList->value;
        if (isIntervalsect(current, init)) {
            int inter_pos = getNextIntersect(current, init);
            set_free_pos(init, inter_pos);
        }
        inactive_list_tmp = list_next(inactive_list_tmp);
    }
    int reg = get_max_free_pos();

    if (free_pos[reg] == 0)
        return 0;
    else if (free_pos[reg] > getLastRange(current)->end) {
        assign_reg2interval(current, reg);
    } else {
        assign_reg2interval(current, reg);
        int optimal_pos = getOptimalPos(getFirstRange(current)->begin + 1, free_pos[reg] - 1, blocks);
        Interval* child = splitInterval(current, optimal_pos);
        __insert_interval_to_unhandled(child, unhandled);
        child->is_spilled = 2;
    }
    return 1;
}

void __insert_interval_to_unhandled(Interval* it, list_entry_t* unhandled_list) {
    if (it->is_fixed) {
        PrintErrExit("try to add fixed interval to unhandled list");
    }
    list_entry_t* elem = list_next(unhandled_list);
    int begin = getFirstRange(it)->begin;
    while (elem != unhandled_list) {
        Interval* cur = le2IntervalList(elem)->value;
        if (getFirstRange(cur)->begin >= begin) {
            break;
        }
        elem = list_next(elem);
    }
    MALLOC(l, IntervalList, 1);
    l->value = it;
    list_add_before(elem, &l->link);
}