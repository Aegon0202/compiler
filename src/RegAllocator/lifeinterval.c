#include "lifeinterval.h"

#include "LRA.h"
int __lowBitToNum(unsigned long long int n) {
    int j = 0;
    while (n = n >> 1) {
        j++;
    }
    return j;
}

Interval* create_new_interval() {
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

void build_interval(BlockBegin* start) {
    gothrough_BlockBeginNode_list_reverse(start, build_interval_block, NULL);
}