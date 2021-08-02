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

RangeList* create_new_interval(int from, int to) {
}
void add_range(Interval* inter, RangeList* range) {
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
            __lowBitToNum(bit);
            tmp -= bit;
        }
    }
}

void build_interval(BlockBegin* start) {
    gothrough_BlockBeginNode_list_reverse(start, build_interval_block, NULL);
}