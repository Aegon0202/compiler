#include <assert.h>

#include "../SysY.target/SysY.target.offset.h"
#include "LRA.h"
#include "lifeinterval.h"
struct FuncRegOffset* f_offset;
Interval* create_new_interval(int reg_num, Interval* parent);
int allocate_spill_slot();

int get_reg_offset(int reg_num) {
    Interval* interval = getIntervalByVal(reg_num);
    if (interval->spill_slot == 0)
        interval->spill_slot = allocate_spill_slot();
    return interval->spill_slot;
}

void resolve_data_flow_block(BlockBegin* block) {
    int pre_last_opid = getLastOpId(block);
    struct DequeList* sux = getSuccessors(block);
    struct DequeList* from_to_queue = newDequeList();
    struct LinearList* block_register = newLinearList();
    while (!isEmptyDequeList(sux)) {
        BlockBegin* suc = popBackDequeList(sux);
        int suc_first_opid = getFirstOpId(block);
        struct DequeList* suc_live_in = getBlock_in(suc);
        int size = sizeDequeList(suc_live_in);

        list_entry_t* add_before_entry = NULL;
        if (block->block->successor_num > 1) {
            assert(suc->block->predecessor_num == 1);
            add_before_entry = suc->block->arm_ir_list->ir_link.next->next;
        } else {
            add_before_entry = block->block->arm_ir_list->ir_link.prev->prev;
        }

        for (int i = 0; i < size; i++) {
            unsigned long long int tmp = *((long long int*)getDequeList(suc_live_in, i));
            while (tmp) {
                int bit = lowBit(tmp);
                int index = bit + 64 * i;
                tmp -= bit;

                Interval* p_inter = getIntervalByVal(index);
                Interval* f_inter = getintervalByChildAt(p_inter, pre_last_opid);
                Interval* t_inter = getintervalByChildAt(p_inter, suc_first_opid);
                if (f_inter != t_inter && f_inter->phisical_reg != t_inter->phisical_reg) {
                    MALLOC(item, struct Item, 1);
                    item->key = f_inter;
                    item->value = t_inter;
                    pushFrontDequeList(from_to_queue, item);
                }
            }
        }
        int size = sizeDequeList(from_to_queue);
        for (int i = 0; i < size; i++) {
            Interval* f_inter = ((struct Item*)getDequeList(from_to_queue, size))->key;
            int* block_num = getLinearList(block_register, f_inter->phisical_reg);
            if (block_num == NULL) {
                MALLOC_WITHOUT_DECLARE(block_num, int, 1);
                setLinearList(block_register, f_inter->phisical_reg, block_num);
            }
            *block_num += 1;
        }
        while (!isEmptyDequeList(from_to_queue)) {
            struct DequeList* not_processed_interval = newDequeList();
            int processed_interval = 0;

            while (!isEmptyDequeList(from_to_queue)) {
                struct Item* item = popFrontDequeList(from_to_queue);
                Interval* f_inter = item->key;
                Interval* t_inter = item->value;
                int* block_num = getLinearList(block_register, t_inter->phisical_reg);
                if (t_inter->phisical_reg == -1) {
                    int off = __get_reg_offset(t_inter->reg_num, suc->block);
                    struct ArmIr* arm_ir = NULL;
                    void* arm_op3 = NULL;
                    void* arm_op1 = newRegister(PHISICAL, f_inter->phisical_reg);
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

                    int* block_num = getLinearList(block_register, f_inter->phisical_reg);
                    *block_num -= 1;
                    processed_interval = 1;
                } else if (*block_num == 0) {
                    if (f_inter->phisical_reg == -1) {
                        int off = __get_reg_offset(f_inter->reg_num, block->block);
                        struct ArmIr* arm_ir = NULL;
                        void* arm_op3 = NULL;
                        void* arm_op1 = newRegister(PHISICAL, t_inter->phisical_reg);
                        void* arm_op2 = newRegister(PHISICAL, FP);
                        if (off > -4095 && off < 4095) {
                            arm_op3 = newImmi_12(off);
                            arm_ir = newArmIr(ARM_LDR_I, NULL, arm_op1, arm_op2, arm_op3, NULL);
                            list_add_before(add_before_entry, &arm_ir->ir_link);
                        } else {
                            arm_ir = newArmIr(ARM_MOVW, NULL, newRegister(PHISICAL, IP), newImmi_16(off), NULL, NULL);
                            list_add_before(add_before_entry, &arm_ir->ir_link);
                            if (off > INT16_MAX || off < INT16_MIN) {
                                arm_ir = newArmIr(ARM_MOVT, NULL, newRegister(PHISICAL, off), newImmi_16(off >> 16), NULL, NULL);
                                list_add_before(add_before_entry, &arm_ir->ir_link);
                            }
                            arm_op3 = newRegister(PHISICAL, IP);
                            arm_ir = newArmIr(ARM_LDR_R, NULL, arm_op1, arm_op2, arm_op3, NULL);
                            list_add_before(add_before_entry, &arm_ir->ir_link);
                        }
                    } else {
                        struct ArmIr* arm_ir = NULL;
                        void* arm_op1 = newRegister(PHISICAL, t_inter->phisical_reg);
                        void* arm_op2 = newOperand(REGISTER, newRegister(PHISICAL, f_inter->phisical_reg));
                        arm_ir = newArmIr(ARM_MOV, NULL, arm_op1, arm_op2, NULL, NULL);
                        list_add_before(add_before_entry, &arm_ir->ir_link);
                    }
                    int* block_num = getLinearList(block_register, f_inter->phisical_reg);
                    *block_num -= 1;
                    processed_interval = 1;
                } else {
                    pushFrontDequeList(not_processed_interval, item);
                }
            }

            if (!processed_interval) {
                struct Item* spill_from_to = getFrontDequeList(not_processed_interval);
                Interval* f_inter = spill_from_to->key;
                Interval* t_inter = spill_from_to->value;
                Interval* spill_it = create_new_interval(f_inter->reg_num, f_inter);

                spill_from_to->key = spill_it;
                spill_it->phisical_reg = -1;

                int off = __get_reg_offset(f_inter->reg_num, block->block);
                struct ArmIr* arm_ir = NULL;
                void* arm_op3 = NULL;
                void* arm_op1 = newRegister(PHISICAL, f_inter->phisical_reg);
                void* arm_op2 = newRegister(PHISICAL, FP);
                if (off > -4095 && off < 4095) {
                    arm_op3 = newImmi_12(off);
                    arm_ir = newArmIr(ARM_STR_I, NULL, arm_op1, arm_op2, arm_op3, NULL);
                    list_add_before(add_before_entry, &arm_ir->ir_link);
                } else {
                    arm_ir = newArmIr(ARM_MOVW, NULL, newRegister(PHISICAL, IP), newImmi_16(off), NULL, NULL);
                    list_add_before(add_before_entry, &arm_ir->ir_link);
                    if (off > INT16_MAX || off < INT16_MIN) {
                        arm_ir = newArmIr(ARM_MOVT, NULL, newRegister(PHISICAL, IP), newImmi_16(off >> 16), NULL, NULL);
                        list_add_before(add_before_entry, &arm_ir->ir_link);
                    }
                    arm_op3 = newRegister(PHISICAL, IP);
                    arm_ir = newArmIr(ARM_STR_R, NULL, arm_op1, arm_op2, arm_op3, NULL);
                    list_add_before(add_before_entry, &arm_ir->ir_link);
                }

                int* block_num = getLinearList(block_register, f_inter->phisical_reg);
                *block_num -= 1;
            }
            freeDequeList(from_to_queue);
            from_to_queue = not_processed_interval;
        }
    }
}

void resolve_data_flow(struct DequeList* block_list) {
    gothrough_BlockBeginNode_list_reverse(block_list, resolve_data_flow_block, NULL);
}