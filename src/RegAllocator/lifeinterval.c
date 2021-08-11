#include "lifeinterval.h"

#include "LRA.h"

int __lowBitToNum(unsigned long long int n) {
    int j = 0;
    while ((n = n >> 1)) {
        j++;
    }
    return j;
}

Interval* create_new_interval(int reg_num, Interval* parent) {
    MALLOC(interval, Interval, 1);
    interval->reg_num = reg_num;
    interval->childrenNum = 0;
    interval->phisical_reg = -1;
    interval->is_fixed = 0;
    interval->split_parent = parent;
    interval->spill_slot = 0;
    MALLOC_WITHOUT_DECLARE(interval->range_list, RangeList, 1);
    MALLOC_WITHOUT_DECLARE(interval->split_childer, IntervalList, 1);
    MALLOC_WITHOUT_DECLARE(interval->usepostion, usepositionList, 1);
    list_init(&interval->range_list->link);
    list_init(&interval->split_childer->link);
    list_init(&interval->usepostion->link);
    return interval;
}

void free_interval(Interval* it) {
    list_entry_t* head = &it->range_list->link;
    while (!list_empty(head)) {
        list_entry_t* elem = list_next(head);
        list_del(elem);
        RangeList* rl = le2RangeList(elem);
        free(rl);
    }
    free(it->range_list);

    head = &it->split_childer->link;
    while (!list_empty(head)) {
        list_entry_t* elem = list_next(head);
        list_del(elem);
        IntervalList* il = le2IntervalList(elem);
        free_interval(il->value);
        free(il);
    }
    free(it->split_childer);

    head = &it->usepostion->link;
    while (!list_empty(head)) {
        list_entry_t* elem = list_next(head);
        list_del(elem);
        usepositionList* ul = le2UsePositionList(elem);
        free(ul);
    }
    free(it->usepostion);

    free(it);
}

Interval* splitInterval(Interval* interval, int split_pos, list_entry_t* unhandled_list_head) {
    if (split_pos >= getFirstRange(interval)->begin || split_pos <= getLastRange(interval)->end)
        PrintErrExit("inappropriate split point");

    Interval* child = create_new_interval(interval->reg_num, interval);
    child->reg_num = interval->reg_num;
    child->spill_slot = interval->spill_slot;

    MALLOC(node, IntervalList, 1);
    node->value = child;
    list_add_before(&interval->split_childer->link, &node->link);

    //process range
    list_entry_t* child_range_head = &child->range_list->link;
    list_entry_t* range_head = &interval->range_list->link;
    list_entry_t* range_elem = list_next(range_head);
    while (range_head != range_elem) {
        RangeList* range_value = le2RangeList(range_elem);
        range_elem = list_next(range_elem);

        if (range_value->end <= split_pos) continue;

        if (range_value->begin < split_pos) {
            MALLOC(new_range, RangeList, 1);
            new_range->begin = split_pos;
            new_range->end = range_value->end;
            range_value->end = split_pos;
            list_add_before(child_range_head, &new_range->link);
        } else {
            list_del(range_elem);
            list_add_before(child_range_head, range_elem);
        }
    }

    //process usepoint
    list_entry_t* child_usepos_head = &child->usepostion->link;
    list_entry_t* usepos_head = &interval->usepostion->link;
    list_entry_t* usepos_elem = list_next(usepos_head);
    while (usepos_elem != usepos_head) {
        int usepos_value = le2UsePositionList(usepos_elem)->position;
        usepos_elem = list_next(usepos_elem);
        if (usepos_value < split_pos) continue;

        list_del(usepos_elem);
        list_add_before(child_usepos_head, usepos_elem);
    }

    //insert into unhandle while maintain the order
    list_entry_t* unhandled_elem = list_next(unhandled_list_head);
    while (unhandled_list_head != unhandled_elem) {
        Interval* unhandled_value = le2IntervalList(unhandled_elem)->value;
        if (getFirstRange(unhandled_value)->begin > getFirstRange(child)->begin) break;
        unhandled_elem = list_next(unhandled_elem);
    }

    MALLOC(inter_node, IntervalList, 1);
    inter_node->value = child;
    list_add_before(unhandled_elem, &inter_node->link);
    return child;
}

void makeRoomForCurrent(Interval* current, Interval* it, list_entry_t* unhandled, struct DequeList* blocks) {
    int n = getNextIntersect(it, current);
    Interval* child = splitInterval(it, n, unhandled);
    int off = get_reg_offset(it->reg_num);
    child->is_spilled = 1;

    list_entry_t* add_before_entry = getArmIrByOpid(blocks, n);
    struct ArmIr* arm_ir = NULL;
    void* arm_op3 = NULL;
    void* arm_op1 = newRegister(PHISICAL, it->phisical_reg);
    void* arm_op2 = newRegister(PHISICAL, FP);
    if (off > -4095 && off < 4095) {
        arm_op3 = newImmi_12(off);
        arm_ir = newArmIr(ARM_STR_I, NULL, arm_op1, arm_op2, arm_op3, NULL);
        list_add_before(add_before_entry, &arm_ir->ir_link);
    } else {
        arm_ir = newArmIr(ARM_MOVW, NULL, newRegister(PHISICAL, IP), newImmi_16(off), NULL, NULL);
        list_add_before(add_before_entry, &arm_ir->ir_link);
        if (off > INT16_MAX || off < INT16_MIN) {
            arm_ir = newArmIr(ARM_MOVT, NULL, newRegister(PHISICAL, off), newImmi_16(off >> 16), NULL, NULL);
            list_add_before(add_before_entry, &arm_ir->ir_link);
        }
        arm_op3 = newRegister(PHISICAL, IP);
        arm_ir = newArmIr(ARM_STR_R, NULL, arm_op1, arm_op2, arm_op3, NULL);
        list_add_before(add_before_entry, &arm_ir->ir_link);
    }
    return;
}

RangeList* create_new_range(int from, int to) {
    MALLOC(range, RangeList, 1);
    range->begin = from;
    range->end = to;
    list_init(&range->link);
    return range;
}

usepositionList* create_new_useposition(int useposition) {
    MALLOC(use, usepositionList, 1);
    use->position = useposition;
    list_init(&use->link);
    return use;
}

void add_range(Interval* inter, RangeList* range) {
    list_entry_t* head = &inter->range_list->link;
    if (list_next(head) == head) {
        list_add(head, &range->link);
        return;
    }
    RangeList* first_range = le2struct(list_next(head), RangeList, link);
    if (first_range->begin == range->begin) {
        free(range);
        return;
    } else if (first_range->begin < range->begin) {
        first_range->begin = range->begin;
        free(range);
        return;
    } else {
        list_add(head, &range->link);
    }
}

void add_use_pos(Interval* inter, usepositionList* use_pos) {
    list_add(&inter->usepostion->link, &use_pos->link);
}

static void __build_interval_read_reg(struct Register* reg, struct ArmIr* arm_ir, int block_from) {
    int op_id = arm_ir->id;
    Interval* inter = NULL;
    inter = getIntervalByVal(reg->reg);

    add_range(inter, create_new_range(block_from, op_id + id_inc - 1));
    add_use_pos(inter, create_new_useposition(op_id));
}

static void __build_interval_write_reg(struct Register* reg, struct ArmIr* arm_ir, int block_to) {
    int op_id = arm_ir->id;
    Interval* inter = NULL;
    inter = getIntervalByVal(reg->reg);

    getFirstRange(inter)->begin = arm_ir->id;
    add_use_pos(inter, create_new_useposition(op_id));
}

static void __build_interval_op2(struct Operand2* op2, struct ArmIr* arm_ir, int block_from) {
    if (op2->type != REGISTER) {
        return;
    }
    __build_interval_read_reg(op2->Rm.reg, arm_ir, block_from);
    if (op2->shift_op != REGISTER) {
        return;
    }
    __build_interval_read_reg(op2->shift.reg, arm_ir, block_from);
}

void build_interval_block(BlockBegin* block, void* args) {
    int block_from = getFirstOpId(block);
    int block_to = getLastOpId(block) + id_inc;
    struct DequeList* b_live_out = getBlock_out(block);
    int size = sizeDequeList(b_live_out);
    for (int i = 0; i < size; i++) {
        long long int* j = getDequeList(b_live_out, i);
        long long int tmp = *j;
        while (tmp != 0) {
            unsigned long long int bit = lowBit(tmp);
            int index = __lowBitToNum(bit) + 64 * i;
            Interval* inter = getIntervalByVal(index);
            add_range(inter, create_new_range(block_from, block_to));
            tmp -= bit;
        }
    }
    list_entry_t* head = getIrListFromBlock(block);
    list_entry_t* ir_elem = list_prev(head);

    while (head != ir_elem) {
        struct ArmIr* ir = le2struct(ir_elem, struct ArmIr, ir_link);
#define READ_REG(op) __build_interval_read_reg(op, ir, block_from)
#define READ_OP2(op) __build_interval_op2(op, ir, block_from)
#define WRITE_REG(op) __build_interval_write_reg(op, ir, block_to)
        ARM_IR_OP_READ_WRITE(ir, READ_REG, READ_OP2, WRITE_REG, PrintErrExit("not support arm ir type"););
#undef READ_REG
#undef READ_OP2
#undef WRITE_REG
        if (ir->type == ARM_BL && ir->op2 != NULL) {
            add_range(getIntervalByVal(A1), create_new_range(ir->id, ir->id + id_inc - 1));
            add_range(getIntervalByVal(A2), create_new_range(ir->id, ir->id + id_inc - 1));
            add_range(getIntervalByVal(A3), create_new_range(ir->id, ir->id + id_inc - 1));
            add_range(getIntervalByVal(A4), create_new_range(ir->id, ir->id + id_inc - 1));
            struct Register reg;
            int param_num = *(int*)(ir->op2);
            reg.type = PHISICAL;
#define tmp_macro(num)                                   \
    if (param_num >= num) {                              \
        reg.reg = A##num;                                \
        __build_interval_read_reg(&reg, ir, block_from); \
    }
            tmp_macro(1);
            tmp_macro(2);
            tmp_macro(3);
            tmp_macro(4);
#undef tmp_macro
        }
        if (ir->type == ARM_BX) {
            struct Register reg;
            reg.type = PHISICAL;
            reg.reg = A1;
            __build_interval_read_reg(&reg, ir, block_from);
        }
        ir_elem = list_prev(ir_elem);
    }
}

void build_interval(struct DequeList* start) {
    gothrough_BlockBeginNode_list_reverse(start, build_interval_block, NULL);
}
