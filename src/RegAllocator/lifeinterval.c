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

Interval* splitInterval(Interval* interval, int split_pos) {
    if (split_pos >= getFirstRange(interval)->begin || split_pos <= getLastRange(interval))
        PrintErrExit("inappropriate split point");

    Interval* child = create_new_interval(interval->reg_num, interval);
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
        if (range_value->end >= split_pos) continue;

        list_del(range_elem);
        list_add_before(child_range_head, range_elem);
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

    return child;
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

static void __build_interval_read_op(OPERAND_TYPE* op, IR_TYPE* ir, int block_from) {
    if (op->type != REGISTER) {
        return;
    }
    int op_id = ir->operation_id;
    Interval* inter = getIntervalByVal(op->operand.reg_idx);
    add_range(inter, create_new_range(block_from, op_id + id_inc - 1));
    add_use_pos(inter, create_new_useposition(op_id));
}

static void __build_interval_write_op(OPERAND_TYPE* op, IR_TYPE* ir, int block_to) {
    if (op->type != REGISTER) {
        return;
    }
    int op_id = ir->operation_id;
    Interval* inter = getIntervalByVal(op->operand.reg_idx);
    add_range(inter, create_new_range(op_id, block_to));
    add_use_pos(inter, create_new_useposition(op_id));
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
        IR_TYPE* ir = le2struct(ir_elem, IR_TYPE, ir_link);
#define READ_OP(op) __build_interval_read_op(op, ir, block_from)
#define WRITE_OP(op) __build_interval_write_op(op, ir, block_to)
        IR_OP_READ_WRITE(ir, READ_OP, WRITE_OP, ;)
#undef READ_OP
#undef WRITE_OP
        if (ir->type == CALL) {
            last_call = ir;
            Interval* inter = getFixIntervalByReg(R0);
            int op_id = ir->operation_id;
            add_range(inter, create_new_range(op_id, op_id + id_inc - 1));
            add_use_pos(inter, create_new_useposition(op_id));
        } else if (ir->type == PARAM) {
            Interval* inter = NULL;
            switch (ir->op2->operand.v.intValue) {
                case 0:
                    inter = getFixIntervalByReg(R0);
                    break;
                case 1:
                    inter = getFixIntervalByReg(R1);
                    break;
                case 2:
                    inter = getFixIntervalByReg(R2);
                    break;
                case 3:
                    inter = getFixIntervalByReg(R3);
                    break;
                default:
                    break;
            }
            if (inter) {
                int op_id = ir->operation_id;
                int call_id = last_call->operation_id;
                add_range(inter, create_new_range(op_id, call_id + id_inc - 1));
                add_use_pos(inter, create_new_useposition(op_id));
                add_use_pos(inter, create_new_useposition(call_id));
            }
        }
        ir_elem = list_prev(ir_elem);
    }
}

void build_interval(struct DequeList* start) {
    gothrough_BlockBeginNode_list_reverse(start, build_interval_block, NULL);
}
