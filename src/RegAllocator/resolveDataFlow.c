#include <assert.h>

#include "./moveResolver.h"
#include "LRA.h"
#include "lifeinterval.h"

struct FuncRegOffset* f_offset;
Interval* create_new_interval(int reg_num, Interval* parent);
int allocate_spill_slot();
int __lowBitToNum(unsigned long long int n);

int get_reg_offset(int reg_num) {
    Interval* interval = getIntervalByVal(reg_num);
    if (interval->spill_slot == 0)
        interval->spill_slot = allocate_spill_slot();
    return interval->spill_slot;
}

void resolve_data_flow_block(BlockBegin* block, void* args) {
    int pre_last_opid = getLastOpId(block);
    struct DequeList* sux = getSuccessors(block);
    struct MoveSolver* ms = newMoveSolver();

    while (!isEmptyDequeList(sux)) {
        BlockBegin* suc = popBackDequeList(sux);
        int suc_first_opid = getFirstOpId(suc);
        struct DequeList* suc_live_in = getBlock_in(suc);
        int size = sizeDequeList(suc_live_in);

        list_entry_t* add_before_entry = NULL;
        if (block->block->successor_num > 1) {
            assert(suc->block->predecessor_num == 1);
            add_before_entry = suc->block->arm_ir_list->ir_link.next->next;
        } else {
            add_before_entry = block->block->arm_ir_list->ir_link.prev;
        }
        setAddBeforeEntry(add_before_entry, ms);

        for (int i = 0; i < size; i++) {
            unsigned long long int tmp = *((long long int*)getDequeList(suc_live_in, i));
            while (tmp) {
                long long int bit = lowBit(tmp);
                int index = __lowBitToNum(bit) + 64 * i;
                tmp -= bit;

                Interval* p_inter = getIntervalByVal(index);
                Interval* f_inter = child_at(p_inter->reg_num, pre_last_opid);
                Interval* t_inter = child_at(p_inter->reg_num, suc_first_opid);
                if (f_inter != t_inter && f_inter->phisical_reg != t_inter->phisical_reg) {
                    addIntervalMap(f_inter, t_inter, ms);
                }
            }
        }

        solveMove(ms);
    }

    freeDequeList(&sux);
    freeMoveSolver(ms);
}

void resolve_data_flow(struct DequeList* block_list) {
    gothrough_BlockBeginNode_list_reverse(block_list, resolve_data_flow_block, NULL);
}