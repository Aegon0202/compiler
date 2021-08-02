#include "LRA.h"

int alloc_register();
int MaxbitMapSize;
int current_bitMap_size;

int get_new_reg_num() {
    if (current_bitMap_size == MaxbitMapSize) {
        MaxbitMapSize += 64;
    }
    return current_bitMap_size++;
}

void __init_bit_map(BlockBegin* block, void* args) {
    current_bitMap_size = alloc_register();
    int deq_list_length = current_bitMap_size / 64 + 11;
    MaxbitMapSize = deq_list_length * 64;

    for (int i = 0; i < deq_list_length; i++) {
        MALLOC(j, long long int, 1);
        *j = 0;
        pushFrontDequeList(block->block_live_gen, j);
        pushFrontDequeList(block->block_live_in, j);
        pushFrontDequeList(block->block_live_out, j);
        pushFrontDequeList(block->block_live_kill, j);
    }
}

void __init_bit_map_global() {
}