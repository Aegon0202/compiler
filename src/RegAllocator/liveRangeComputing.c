#include "LRA.h"

static int change;

int isBitMapEqual(struct DequeList* bm1, struct DequeList* bm2) {
    int size1 = sizeDequeList(bm1);
    int size2 = sizeDequeList(bm2);
    if (size1 != size2)
        PrintErrExit("please ensure Bitmaps have the same size");

    for (int i = 0; i < size1; i++) {
        long long int* ll1 = getDequeList(bm1, i);
        long long int* ll2 = getDequeList(bm2, i);
        if (*ll1 != *ll2)
            return 0;
    }
    return 1;
}

//bm1 = bm2
struct DequeList* newBitMap(int size) {
    struct DequeList* bm = newDequeList();
    for (int i = 0; i < size; i++) {
        MALLOC(j, long long int, 1);
        *j = 0;
        pushFrontDequeList(bm, j);
    }
    return bm;
}

void freeBitMap(struct DequeList* bm) {
    long long int* j = NULL;
    while ((j = popBackDequeList(bm)) != NULL) {
        free(j);
    }
    freeDequeList(&bm);
}

//bm1 = bm2
void BitMapCopy(struct DequeList* bm1, struct DequeList* bm2) {
    int size1 = sizeDequeList(bm1);
    int size2 = sizeDequeList(bm2);
    if (size1 != size2)
        PrintErrExit("please ensure Bitmaps have the same size");
    for (int i = 0; i < size1; i++) {
        long long int* ll1 = getDequeList(bm1, i);
        long long int* ll2 = getDequeList(bm2, i);
        *ll1 = *ll2;
    }
}

//bm3 = bm1|bm2
void BitMapUnion(struct DequeList* bm1, struct DequeList* bm2, struct DequeList* bm3) {
    int size1 = sizeDequeList(bm1);
    int size2 = sizeDequeList(bm2);
    int size3 = sizeDequeList(bm3);
    if (size1 != size2 || size1 != size3)
        PrintErrExit("please ensure Bitmaps have the same size");
    for (int i = 0; i < size1; i++) {
        long long int* ll1 = getDequeList(bm1, i);
        long long int* ll2 = getDequeList(bm2, i);
        long long int* ll3 = getDequeList(bm3, i);
        *ll3 = SingleBitMapUnion(*ll1, *ll2);
    }
}

void BitMapIntersect(struct DequeList* bm1, struct DequeList* bm2, struct DequeList* bm3) {
    int size1 = sizeDequeList(bm1);
    int size2 = sizeDequeList(bm2);
    int size3 = sizeDequeList(bm3);
    if (size1 != size2 || size1 != size3)
        PrintErrExit("please ensure Bitmaps have the same size");
    for (int i = 0; i < size1; i++) {
        long long int* ll1 = getDequeList(bm1, i);
        long long int* ll2 = getDequeList(bm2, i);
        long long int* ll3 = getDequeList(bm3, i);
        *ll3 = SingleBitMapIntersect(*ll1, *ll2);
    }
}

void BitMapSub(struct DequeList* bm1, struct DequeList* bm2, struct DequeList* bm3) {
    int size1 = sizeDequeList(bm1);
    int size2 = sizeDequeList(bm2);
    int size3 = sizeDequeList(bm3);
    if (size1 != size2 || size1 != size3)
        PrintErrExit("please ensure Bitmaps have the same size");
    for (int i = 0; i < size1; i++) {
        long long int* ll1 = getDequeList(bm1, i);
        long long int* ll2 = getDequeList(bm2, i);
        long long int* ll3 = getDequeList(bm3, i);
        *ll3 = SingleBitMapSub(*ll1, *ll2);
    }
}

void __CLS_read_op(struct Register* op, struct DequeList* b_gen, struct DequeList* b_kill) {
    if (op->type != REGISTER) return;

    int reg = op->reg;
    int index1 = reg / 64;
    int index2 = reg - 64 * index1;
    unsigned long long* ll = getDequeList(b_kill, index1);
    if (!is_One(*ll, index2)) {
        ll = getDequeList(b_gen, index1);
        set_One(*ll, index2);
    }
}

void __CLS_write_op(struct Register* op, struct DequeList* b_kill) {
    if (op->type != REGISTER) return;

    int reg = op->reg;
    int index1 = reg / 64;
    int index2 = reg - 64 * index1;
    unsigned long long* ll = getDequeList(b_kill, index1);
    set_One(*ll, index2);
}

void __CLS_read_op2(struct Operand2* op, struct DequeList* b_gen, struct DequeList* b_kill) {
    if (op->type != REGISTER) return;
    __CLS_read_op(op->Rm.reg, b_gen, b_kill);
    if (op->shift_op != REGISTER) return;
    __CLS_read_op(op->shift.reg, b_gen, b_kill);
}

void compute_local_live_set_block(BlockBegin* block, void* args) {
    list_entry_t* ir_list = getIrListFromBlock(block);
    list_entry_t* ir_elem = list_next(ir_list);

    struct DequeList* b_gen = getBlock_gen(block);
    struct DequeList* b_kill = getBlock_kill(block);

    while (ir_elem != ir_list) {
        struct ArmIr* ir_value = le2struct(ir_elem, struct ArmIr, ir_link);
        ir_elem = list_next(ir_elem);

#define READ_OP(op) __CLS_read_op(op, b_gen, b_kill)
#define WRITE_OP(op) __CLS_write_op(op, b_kill)
#define READ_OP2(op) __CLS_read_op2(op, b_gen, b_kill)
        ARM_IR_OP_READ_WRITE(ir_value, READ_OP, READ_OP2, WRITE_OP, PrintErrExit(" "););
#undef READ_OP
#undef WRITE_OP
#undef READ_OP2
    }
}

void compute_local_live_set(struct DequeList* start) {
    gothrough_BlockBeginNode_list(start, compute_local_live_set_block, NULL);
}

void compute_global_live_set_block(BlockBegin* block, void* args) {
    struct DequeList* sux = getSuccessors(block);
    struct DequeList* b_live_out = getBlock_out(block);
    struct DequeList* b_live_in = getBlock_in(block);
    struct DequeList* b_live_kill = getBlock_kill(block);
    struct DequeList* b_live_gen = getBlock_gen(block);
    int size = sizeDequeList(b_live_out);
    struct DequeList* tmp_live_out = newBitMap(size);
    struct DequeList* tmp_live_in = newBitMap(size);
    struct DequeList* tmp = newBitMap(size);

    while (!isEmptyDequeList(sux)) {
        BlockBegin* suc = popBackDequeList(sux);
        struct DequeList* suc_live_in = getBlock_in(suc);
        BitMapUnion(tmp_live_out, suc_live_in, tmp_live_out);
    }
    BitMapSub(tmp_live_out, b_live_kill, tmp);
    BitMapUnion(tmp, b_live_gen, tmp_live_in);

    if (!isBitMapEqual(tmp_live_in, b_live_in)) {
        change = 1;
        BitMapCopy(b_live_in, tmp_live_in);
    }

    if (!isBitMapEqual(tmp_live_out, b_live_out)) {
        change = 1;
        BitMapCopy(b_live_out, tmp_live_out);
    }

    freeBitMap(tmp_live_out);
    freeBitMap(tmp_live_in);
    freeBitMap(tmp);
}

void compute_global_live_set(struct DequeList* block_list) {
    change = 1;
    while (change) {
        change = 0;
        gothrough_BlockBeginNode_list_reverse(block_list, compute_global_live_set_block, NULL);
    }
}

RangeList* getFirstRange(Interval* interval) {
    return le2RangeList(list_next(&(interval->range_list->link)));
}

RangeList* getLastRange(Interval* interval) {
    return le2RangeList(list_prev(&(interval->range_list->link)));
}

int isIntervalFixed(Interval* interval) {
    return interval->is_fixed;
}
