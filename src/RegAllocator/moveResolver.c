#include "./moveResolver.h"

#include "./LRA.h"

struct MoveSolver* newMoveSolver() {
    MALLOC(mr, struct MoveSolver, 1);
    mr->from_to_queue = newDequeList();
    mr->add_before_entry = NULL;
    mr->block_register = newLinearList();
    for (int i = 0; i < BEGIN_REG_NUM; i++) {
        MALLOC(block_num, int, 1);
        setLinearList(mr->block_register, i, block_num);
        *block_num = 0;
    }
    return mr;
}

void freeMoveSolver(struct MoveSolver* ms) {
    freeFuncDequeList(ms->from_to_queue, only_free, NULL);
    freeFuncLinearList(ms->block_register, only_free, NULL);
    free(ms);
}

void setAddBeforeEntry(list_entry_t* before, struct MoveSolver* mr) {
    EnsureNotNull(before);
    mr->add_before_entry = before;
}

void addIntervalMap(Interval* from_interval, Interval* to_interval, struct MoveSolver* mr) {
    EnsureNotNull(from_interval);
    EnsureNotNull(to_interval);
    MALLOC(item, struct Item, 1);
    item->key = from_interval;
    item->value = to_interval;
    pushFrontDequeList(mr->from_to_queue, item);
}

void solveMove(struct MoveSolver* mr) {
    struct LinearList* block_register = mr->block_register;
    struct DequeList* from_to_queue = mr->from_to_queue;
    list_entry_t* add_before_entry = mr->add_before_entry;

    int size = sizeDequeList(from_to_queue);
    for (int i = 0; i < size; i++) {
        Interval* f_inter = ((struct Item*)getDequeList(from_to_queue, i))->key;
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
                store_reg_to_stack(f_inter, add_before_entry);

                int* block_num = getLinearList(block_register, f_inter->phisical_reg);
                *block_num -= 1;
                processed_interval = 1;
            } else if (*block_num == 0) {
                if (f_inter->phisical_reg == -1) {
                    load_reg_from_stack(t_inter, add_before_entry);
                } else {
                    struct ArmIr* arm_ir = NULL;
                    void* arm_op1 = newRegister(PHISICAL, t_inter->phisical_reg);
                    void* arm_op2 = newOperand2(REGISTER, newRegister(PHISICAL, f_inter->phisical_reg));
                    arm_ir = newArmIr(ARM_MOV, NULL, arm_op1, arm_op2, NULL, NULL);
                    list_add_before(add_before_entry, &arm_ir->ir_link);

                    int* block_num = getLinearList(block_register, f_inter->phisical_reg);
                    *block_num -= 1;
                }
                processed_interval = 1;
            } else {
                pushFrontDequeList(not_processed_interval, item);
            }
        }

        if (!processed_interval) {
            struct Item* spill_from_to = getFrontDequeList(not_processed_interval);
            Interval* f_inter = spill_from_to->key;
            Interval* spill_it = create_new_interval(f_inter->reg_num, f_inter);

            spill_from_to->key = spill_it;
            spill_it->phisical_reg = -1;

            store_reg_to_stack(f_inter, add_before_entry);
            int* block_num = getLinearList(block_register, f_inter->phisical_reg);
            *block_num -= 1;
        }
        freeDequeList(&from_to_queue);
        from_to_queue = not_processed_interval;
    }

    mr->from_to_queue = from_to_queue;
}