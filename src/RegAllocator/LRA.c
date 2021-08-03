#include "LRA.h"

int alloc_register();
int MaxbitMapSize;        //当前dequelist中最多能够容纳多少元素个数（64的整数倍）
int current_bitMap_size;  //当前位图元素个数
struct LinearList* reg2Intival;
extern struct DequeList* allBlock;

int get_new_reg_num() {
    if (current_bitMap_size == MaxbitMapSize) {
        MaxbitMapSize += (64 * 3);
        __add_bit_map_global(allBlock);
    }
    return current_bitMap_size++;
}

void gothrough_BlockBeginNode_list_reverse(struct DequeList* block_list_head, void (*func)(BlockBegin*, void*), void* args) {
    for (int i = sizeDequeList(block_list_head) - 1; i >= 0; i--) {
        BlockBegin* block = getDequeList(block_list_head, i);
        func(block, args);
    }
}
void gothrough_BlockBeginNode_list(struct DequeList* block_list_head, void (*func)(BlockBegin*, void*), void* args) {
    for (int i = 0; i < sizeDequeList(block_list_head); i++) {
        BlockBegin* block = getDequeList(block_list_head, i);
        func(block, args);
    };
}

void __add_bit_map(BlockBegin* block, void* args) {
    int increase = MaxbitMapSize / 64 - sizeDequeList(block->block_live_gen);
    if (increase < 0)
        PrintErrExit("increasement cannot be minus");

    for (int i = 0; i < increase; i++) {
        MALLOC(j, long long int, 1);
        *j = 0;
        pushFrontDequeList(block->block_live_gen, j);
        pushFrontDequeList(block->block_live_in, j);
        pushFrontDequeList(block->block_live_out, j);
        pushFrontDequeList(block->block_live_kill, j);
    }
}

void __add_bit_map_global(struct DequeList* block_list) {
    gothrough_BlockBeginNode_list(block_list, __add_bit_map, NULL);
}

void __init_bit_map_global() {
    current_bitMap_size = alloc_register();
    MaxbitMapSize = current_bitMap_size / 64 + 64 * 5;
    __add_bit_map_global(allBlock);
}

void assign_phisical_reg_num_block(BlockBegin* block, void* args) {
    list_entry_t* ir_list = getIrListFromBlock(block);
    list_entry_t* ir_elem = list_next(ir_list);
    while (ir_elem != ir_list) {
    }
}

//对每个函数
void LinearScanRegAllocation() {
    struct DequeList* block_seq = computeBlockOrder(NULL);
    numberLirOp(block_seq);
    compute_local_live_set(block_seq);
    compute_global_live_set(block_seq);

    build_interval(block_seq);
    IntervalList* unhandled_list = __init_unhandled_list();
    walkIntervals(unhandled_list);
    resolve_data_flow(block_seq);
    assign_phisical_reg_num(block_seq);
}

//----------------------

list_entry_t* getIrListFromBlock(BlockBegin* block) {
    return &block->block->arm_ir_list->ir_link;
}

int getFirstOpId(BlockBegin* block) {
    return le2struct(list_next(&block->block->arm_ir_list->ir_link), struct ArmIr, ir_link)->id;
}
int getLastOpId(BlockBegin* block) {
    return le2struct(list_prev(&block->block->arm_ir_list->ir_link), struct ArmIr, ir_link)->id;
}
struct DequeList* getBlock_kill(BlockBegin* block) {
    return block->block_live_kill;
}
struct DequeList* getBlock_gen(BlockBegin* block) {
    return block->block_live_gen;
}
struct DequeList* getBlock_in(BlockBegin* block) {
    return block->block_live_in;
}
struct DequeList* getBlock_out(BlockBegin* block) {
    return block->block_live_out;
}

Interval* getIntervalByVal(int reg_num) {
    return getLinearList(reg2Intival, reg_num);
}

Interval* getFixIntervalByReg(int reg_num) {
    return NULL;
}