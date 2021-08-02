#include "LRA.h"

void resolve_data_flow_block(BlockBegin* block) {
    int pre_last_opid = getLastOpId(block);
    struct DequeList* sux = getSuccessors(block);
    while (!isEmptyDequeList(sux)) {
        BlockBegin* suc = popBackDequeList(sux);
        int suc_first_opid = getFirstOpId(block);
        struct DequeList* suc_live_in = getBlock_in(suc);
        int size = sizeDequeList(suc_live_in);
        for (int i = 0; i < size; i++) {
            unsigned long long int tmp = *((long long int*)getDequeList(suc_live_in, i));
            while (tmp) {
                int bit = lowBit(tmp);
                int index = bit + 64 * i;
                tmp -= bit;

                Interval* p_inter = getIntervalByVal(index);
                Interval* f_inter = getintervalByChildAt(p_inter, pre_last_opid);
                Interval* t_inter = getintervalByChildAt(p_inter, suc_first_opid);
                if (f_inter != t_inter) {
                    ;
                }
            }
        }
    }
}

void resolve_data_flow() {
}