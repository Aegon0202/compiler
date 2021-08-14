#include "LRA.h"

Interval* child_at(int reg_num, int op_id);

int alloc_register();
void __compute_loop_info_func(struct FuncTabElem* func);
void __convert_to_file(struct FuncTabElem* func, struct DequeList* block_list, FILE* out_file);
void __add_bit_map_global(struct DequeList* block_list);
void assign_phisical_reg_num(struct DequeList* block_list);

int MaxbitMapSize;        //当前dequelist中最多能够容纳多少元素个数（64的整数倍）
int current_bitMap_size;  //当前位图元素个数
struct LinearList* reg2Intival;
extern struct DequeList* allBlock;
int spill_current;

int allocate_spill_slot() {
    spill_current -= INT_SIZE;
    return spill_current;
}

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

        MALLOC_WITHOUT_DECLARE(j, long long int, 1);
        pushFrontDequeList(block->block_live_in, j);

        MALLOC_WITHOUT_DECLARE(j, long long int, 1);
        pushFrontDequeList(block->block_live_out, j);

        MALLOC_WITHOUT_DECLARE(j, long long int, 1);
        pushFrontDequeList(block->block_live_kill, j);
    }
}

void __add_bit_map_global(struct DequeList* block_list) {
    gothrough_BlockBeginNode_list(block_list, __add_bit_map, NULL);
}

void __init_bit_map_global() {
    current_bitMap_size = alloc_register();
    MaxbitMapSize = current_bitMap_size + BEGIN_REG_NUM + 5 * 64;
    __add_bit_map_global(allBlock);
}

//对每个函数
void LinearScanRegAllocation(struct FuncTabElem* elem, FILE* out_file) {
    if (elem->blocks == NULL) {
        return;
    }
    spill_current = elem->var_offset_end;
    __compute_loop_info_func(elem);

    struct DequeList* block_seq = computeBlockOrder(((BASIC_BLOCK_TYPE*)elem->blocks)->block_LRA);
    numberLirOp(block_seq);
    compute_local_live_set(block_seq);
    compute_global_live_set(block_seq);

    gothrough_BlockBeginNode_list(block_seq, __print_arm_ir_block, NULL);

    build_interval(block_seq);
    walkIntervals(block_seq);
    resolve_data_flow(block_seq);
    assign_phisical_reg_num(block_seq);

    elem->var_offset_end = spill_current;
    __convert_to_file(elem, block_seq, out_file);

    Interval* it = NULL;
    while ((it = popLinearList(reg2Intival)) != NULL) {
        free_interval(it);
    }
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
    Interval* it = getLinearList(reg2Intival, reg_num);
    if (it == NULL) {
        it = create_new_interval(reg_num, NULL);
        setLinearList(reg2Intival, reg_num, it);
        if (reg_num < BEGIN_REG_NUM) {
            it->is_fixed = 1;
            it->phisical_reg = reg_num;
        }
    }
    return it;
}

int getInterval_assigned_reg(Interval* interval) {
    return interval->phisical_reg;
}
int isCoverd(Interval* it, int position) {
    int flag = 0;
    int begin_pos = position + (position & 0x1);
    list_entry_t* range_list_head = &it->range_list->link;
    list_entry_t* range_list_tmp = list_next(range_list_head);
    while (range_list_tmp != range_list_head) {
        int tmp_begin = le2struct(range_list_tmp, RangeList, link)->begin;
        int tmp_end = le2struct(range_list_tmp, RangeList, link)->end;
        if (tmp_begin <= begin_pos && tmp_end > position) {
            flag = 1;
        }
        range_list_tmp = list_next(range_list_tmp);
    }
    return flag;
}

//如果有交集，返回的点是 it的某个range的起点
int getNextIntersect(Interval* current, Interval* it) {
    list_entry_t* current_range_list = &current->range_list->link;
    list_entry_t* current_range_elem = list_next(current_range_list);

    list_entry_t* it_range_list = &it->range_list->link;
    list_entry_t* it_range_elem = list_next(it_range_list);

    RangeList* current_range_value = le2RangeList(current_range_elem);
    RangeList* it_range_value = le2RangeList(it_range_elem);

    while (current_range_list != current_range_elem && it_range_list != it_range_elem) {
        while (it_range_value->begin >= current_range_value->end && current_range_list != current_range_elem) {
            current_range_elem = list_next(current_range_elem);
            current_range_value = le2RangeList(current_range_elem);
        }
        while (it_range_value->end <= current_range_value->begin && it_range_list != it_range_elem) {
            it_range_elem = list_next(it_range_elem);
            it_range_value = le2RangeList(it_range_elem);
        }

        if (current_range_list != current_range_elem && it_range_list != it_range_elem && it_range_value->begin < current_range_value->end && it_range_value->end > current_range_value->begin) {
            return it_range_value->begin;
        }
    }

    return -1;
}

int isIntervalsect(Interval* it1, Interval* it2) {
    if (getNextIntersect(it1, it2) == -1)
        return 0;
    else
        return 1;
}

int getNextUsage(Interval* interval, int pos) {
    list_entry_t* usepoint_list_head = &interval->usepostion->link;
    list_entry_t* usepoint_list_elem = list_next(usepoint_list_head);
    while (usepoint_list_head != usepoint_list_elem) {
        usepositionList* usepoint_list_value = le2UsePositionList(usepoint_list_elem);
        usepoint_list_elem = list_next(usepoint_list_elem);
        if (pos < usepoint_list_value->position)
            return usepoint_list_value->position;
    }
    return 0x3f3f3f3f;
}

void assign_reg2interval(Interval* current, int reg) {
    current->phisical_reg = reg;
}

int getFirstUsePos(Interval* interval) {
    list_entry_t* usepos_head = &interval->usepostion->link;
    list_entry_t* usepos_elem = list_next(usepos_head);
    int ans = 0x3f3f3f3f;
    while (usepos_head != usepos_elem) {
        int usepos_value = le2UsePositionList(usepos_elem)->position;
        usepos_elem = list_next(usepos_elem);

        ans = ans > usepos_value ? usepos_value : ans;
    }
    return ans;
}

int getOptimalPos(int n) {
    return n - 1;
}

void __rename_reg_num(struct Register* reg, struct ArmIr* arm_ir) {
    if (reg->type == PHISICAL) {
        return;
    }

    Interval* it = child_at(reg->reg, arm_ir->id);
    reg->type = PHISICAL;
    reg->reg = it->phisical_reg;
}

void __rename_op2_num(struct Operand2* op2, struct ArmIr* arm_ir) {
    if (op2->type != REGISTER) {
        return;
    }
    __rename_reg_num(op2->Rm.reg, arm_ir);
    if (op2->shift_op != REGISTER) {
        return;
    }
    __rename_reg_num(op2->shift.reg, arm_ir);
}

void __assign_reg_num_block(BlockBegin* block, void* args) {
    list_entry_t* arm_head = &block->block->arm_ir_list->ir_link;
    list_entry_t* arm_elem = list_next(arm_head);
    while (arm_head != arm_elem) {
        struct ArmIr* arm_ir = le2struct(arm_elem, struct ArmIr, ir_link);
        arm_elem = list_next(arm_elem);
#define RENAME_REG(op) __rename_reg_num(op, arm_ir)
#define RENAME_OP2(op) __rename_op2_num(op, arm_ir)
        ARM_IR_OP_READ_WRITE(arm_ir, RENAME_REG, RENAME_OP2, RENAME_REG, PrintErrExit("not support arm ir type %s", EnumTypeToString(arm_ir->type)););
#undef RENAME_REG
#undef RENAME_OP2
    }
}

void assign_phisical_reg_num(struct DequeList* block_list) {
    gothrough_BlockBeginNode_list_reverse(block_list, __assign_reg_num_block, NULL);
}

Interval* __find_child(Interval* p_it, int op_id) {
    if (getFirstRange(p_it)->begin <= op_id && getLastRange(p_it)->end >= op_id) {
        return p_it;
    }

    list_entry_t* head = &p_it->split_childer->link;
    list_entry_t* elem = list_next(head);
    while (head != elem) {
        Interval* c_it = le2IntervalList(elem)->value;
        elem = list_next(elem);
        Interval* r_it = __find_child(c_it, op_id);
        if (r_it != NULL) {
            return r_it;
        }
    }
    return NULL;
}

Interval* child_at(int reg_num, int op_id) {
    Interval* p_it = getIntervalByVal(reg_num);
    return __find_child(p_it, op_id);
}

list_entry_t* getArmIrByOpid(struct DequeList* block_list, int op_id) {
    op_id += op_id & 0x1;
    for (int i = 0; i < sizeDequeList(block_list); i++) {
        BlockBegin* block = getDequeList(block_list, i);
        if (block->first_op_id <= op_id && block->last_op_id >= op_id) {
            list_entry_t* ir_list = getIrListFromBlock(block);
            list_entry_t* ir_elem = list_next(ir_list);
            while (ir_list != ir_elem) {
                struct ArmIr* ir_value = le2struct(ir_elem, struct ArmIr, ir_link);

                if (ir_value->id == op_id)
                    return ir_elem;
                ir_elem = list_next(ir_elem);
            }
        }
    }
    PrintErrExit("not found arm op id %d", op_id);
    return NULL;
}

void store_reg_to_stack(Interval* it, list_entry_t* add_before_entry) {
    int off = get_reg_offset(it->reg_num);
    struct ArmIr* arm_ir = NULL;
    void* arm_op3 = NULL;
    void* arm_op1 = newRegister(PHISICAL, it->phisical_reg);
    void* arm_op2 = newRegister(PHISICAL, FP);

    if (off > -4095 && off < 4095) {
        arm_op3 = newImmi_12(off);
        arm_ir = newArmIr(ARM_STR_I, NULL, arm_op1, arm_op2, arm_op3, NULL);
        list_add_before(add_before_entry, &arm_ir->ir_link);
    } else {
        arm_ir = newArmIr(ARM_MOVW, NULL, newRegister(PHISICAL, IP), newImmi_16(off), NULL, NULL);
        list_add_before(add_before_entry, &arm_ir->ir_link);

        arm_ir = newArmIr(ARM_MOVT, NULL, newRegister(PHISICAL, off), newImmi_16(off >> 16), NULL, NULL);
        list_add_before(add_before_entry, &arm_ir->ir_link);

        arm_op3 = newRegister(PHISICAL, IP);
        arm_ir = newArmIr(ARM_STR_R, NULL, arm_op1, arm_op2, arm_op3, NULL);
        list_add_before(add_before_entry, &arm_ir->ir_link);
    }
}

void load_reg_from_stack(Interval* it, list_entry_t* add_before_entry) {
    int off = get_reg_offset(it->reg_num);
    struct ArmIr* arm_ir = NULL;
    void* arm_op3 = NULL;
    void* arm_op1 = newRegister(PHISICAL, it->phisical_reg);
    void* arm_op2 = newRegister(PHISICAL, FP);

    if (off > -4095 && off < 4095) {
        arm_op3 = newImmi_12(off);
        arm_ir = newArmIr(ARM_LDR_I, NULL, arm_op1, arm_op2, arm_op3, NULL);
        list_add_before(add_before_entry, &arm_ir->ir_link);
    } else {
        arm_ir = newArmIr(ARM_MOVW, NULL, newRegister(PHISICAL, IP), newImmi_16(off), NULL, NULL);
        list_add_before(add_before_entry, &arm_ir->ir_link);

        arm_ir = newArmIr(ARM_MOVT, NULL, newRegister(PHISICAL, off), newImmi_16(off >> 16), NULL, NULL);
        list_add_before(add_before_entry, &arm_ir->ir_link);

        arm_op3 = newRegister(PHISICAL, IP);
        arm_ir = newArmIr(ARM_LDR_R, NULL, arm_op1, arm_op2, arm_op3, NULL);
        list_add_before(add_before_entry, &arm_ir->ir_link);
    }
}