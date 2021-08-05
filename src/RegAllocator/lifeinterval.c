#include "lifeinterval.h"

#include "LRA.h"
int __lowBitToNum(unsigned long long int n) {
    int j = 0;
    while (n = n >> 1) {
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
    MALLOC_WITHOUT_DECLARE(interval->range_list, RangeList, 1);
    MALLOC_WITHOUT_DECLARE(interval->split_childer, IntervalList, 1);
    MALLOC_WITHOUT_DECLARE(interval->usepostion, usepositionList, 1);
    list_init(&interval->range_list->link);
    list_init(&interval->split_childer->link);
    list_init(&interval->usepostion->link);
    return interval;
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
    }
    list_add(head, &range->link);
}

void add_use_pos(Interval* inter, usepositionList* use_pos) {
    list_add(&inter->usepostion->link, &use_pos->link);
}

static void __build_interval_read_reg(struct Register* reg, struct ArmIr* arm_ir, int block_from) {
    int op_id = arm_ir->id;
    Interval* inter = NULL;
    if (reg->type == REGISTER) {
        inter = getIntervalByVal(reg->reg);
    } else {
        inter = getFixIntervalByReg(reg->reg);
    }
    add_range(inter, create_new_range(block_from, op_id + id_inc - 1));
    add_use_pos(inter, create_new_useposition(op_id));
}

static void __build_interval_write_reg(struct Register* reg, struct ArmIr* arm_ir, int block_to) {
    int op_id = arm_ir->id;
    Interval* inter = NULL;
    if (reg->type == REGISTER) {
        inter = getIntervalByVal(reg->reg);
    } else {
        inter = getFixIntervalByReg(reg->reg);
    }
    add_range(inter, create_new_range(op_id, block_to));
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
    IR_LIST_TYPE* last_call = NULL;
    while (head != ir_elem) {
        struct ArmIr* ir = le2struct(ir_elem, struct ArmIr, ir_link);
#define READ_REG(op) __build_interval_read_reg(op, ir, block_from)
#define READ_OP2(op) __build_interval_op2(op, ir, block_from)
#define WRITE_REG(op) __build_interval_write_reg(op, ir, block_to)
        ARM_IR_OP_READ_WRITE(ir, READ_REG, READ_OP2, WRITE_REG, PrintErrExit("not support arm ir type"););
#undef READ_REG
#undef READ_OP2
#undef WRITE_REG
        ir_elem = list_prev(ir_elem);
    }
}

void build_interval(struct DequeList* start) {
    gothrough_BlockBeginNode_list_reverse(start, build_interval_block, NULL);
}