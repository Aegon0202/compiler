#include "./SysY.target.offset.h"

#include "../ssa/ssa.h"
#include "../ssa/traverse.h"
#include "../utils/IrType.h"
#include "../utils/Malloc.h"

struct BlockRegOffset* newBlockRegOffset(BASIC_BLOCK_TYPE* basic_block, int now_offset) {
    MALLOC(b_offset, struct BlockRegOffset, 1);
    b_offset->basic_block = basic_block;
    b_offset->now_offset = now_offset;
    b_offset->reg_offset = newLinearList();
    return b_offset;
}

struct FuncRegOffset* newFuncRegOffset(struct FuncTabElem* elem) {
    MALLOC(f_offset, struct FuncRegOffset, 1);
    f_offset->funcelem = elem;
    f_offset->now_offset = elem->var_offset_end;
    f_offset->max_offset = elem->var_offset_end;
    f_offset->block_offsets = newLinearList();
    f_offset->reg_offset = newLinearList();
    f_offset->reg_2_users = newLinearList();
    f_offset->reg_2_defs = newLinearList();
    return f_offset;
}

void __calc_in_block_reg_offset(OPERAND_TYPE* op, struct BlockRegOffset* b_offset, struct FuncRegOffset* f_offset) {
    if (op == NULL) {
        return;
    }
    if (op->type != REGISTER) {  // other is immi
        return;
    }
    int reg_idx = op->operand.reg_idx;

    int* offset_p = getLinearList(f_offset->reg_offset, reg_idx);
    IfNull(offset_p, offset_p = getLinearList(b_offset->reg_offset, reg_idx););
    if (offset_p == NULL) {
        MALLOC_WITHOUT_DECLARE(offset_p, int, 1)
        *offset_p = b_offset->now_offset - INT_SIZE;
        b_offset->now_offset -= INT_SIZE;
        setLinearList(b_offset->reg_offset, reg_idx, offset_p);
    }
}

void __calc_in_block_offset(BASIC_BLOCK_TYPE* basic_block, void* args) {
    struct FuncRegOffset* f_offset = (struct FuncRegOffset*)args;
    list_entry_t* head = &(basic_block->ir_list->ir_link);
    list_entry_t* next = head->next;
    struct BlockRegOffset* b_offset = getLinearList(f_offset->block_offsets, (size_t)basic_block);
    b_offset->now_offset = f_offset->now_offset;

#define READ_OP(op) __calc_in_block_reg_offset(op, b_offset, f_offset);
#define WRITE_OP(op) __calc_in_block_reg_offset(op, b_offset, f_offset);

    while (head != next) {
        IR_TYPE* ir = le2struct(next, IR_TYPE, ir_link);

        IR_OP_READ_WRITE(ir, READ_OP, WRITE_OP, PrintErrExit("Not support ir type %s", EnumTypeToString(ir->type)););

        next = list_next(next);
    }

    f_offset->max_offset = f_offset->max_offset < b_offset->now_offset ? f_offset->max_offset : b_offset->now_offset;

#undef READ_OP
#undef WRITE_OP
}

void __calc_between_block_reg_offset(OPERAND_TYPE* op, struct BlockRegOffset* b_offset, struct FuncRegOffset* f_offset) {
    if (op == NULL) {
        return;
    }
    if (op->type != REGISTER) {  // other is immi
        return;
    }
    int reg_idx = op->operand.reg_idx;

    int* offset_p = getLinearList(f_offset->reg_offset, reg_idx);
    if (offset_p == NULL) {
        struct VarTabElem* vte = getLinearList(reg_id_vartabelem, reg_idx);
        if (vte != NULL && vte->offset != INT32_MAX) {
            MALLOC(offset_p, int, 1)
            *offset_p = vte->offset;
            setLinearList(f_offset->reg_offset, reg_idx, offset_p);
            return;
        }

        int all_in_block = 1;

        struct Item* user = getLinearList(f_offset->reg_2_users, reg_idx);
        struct Item* def = getLinearList(f_offset->reg_2_defs, reg_idx);
        EnsureNotNull(def);
        if (user != NULL) {
            if (*(int*)(user->key) == -1) {
                user->value = convertToLinearList(user->value, user->key);
            }
            if (*(int*)(def->key) == -1) {
                def->value = convertToLinearList(def->value, def->key);
            }
            int user_blocks_num = *(int*)user->key;
            int def_blocks_num = *(int*)def->key;
            for (int i = 0; i < user_blocks_num; i++) {
                void* u_block = getLinearList(user->value, i);
                for (int j = 0; j < def_blocks_num; j++) {
                    void* d_block = getLinearList(def->value, j);
                    if (d_block != u_block) {
                        all_in_block = 0;
                        break;
                    }
                }
                if (!all_in_block) {
                    break;
                }
            }
        }
        if (!all_in_block) {
            MALLOC_WITHOUT_DECLARE(offset_p, int, 1)
            *offset_p = b_offset->now_offset - INT_SIZE;
            b_offset->now_offset -= INT_SIZE;
            setLinearList(f_offset->reg_offset, reg_idx, offset_p);
        }
    }
}

void __calc_between_block_offset(BASIC_BLOCK_TYPE* basic_block, void* args) {
    struct FuncRegOffset* f_offset = (struct FuncRegOffset*)args;
    list_entry_t* head = &(basic_block->ir_list->ir_link);
    list_entry_t* next = head->next;
    struct BlockRegOffset* b_offset = newBlockRegOffset(basic_block, f_offset->max_offset);
    setLinearList(f_offset->block_offsets, (size_t)basic_block, b_offset);

#define READ_OP(op) __calc_between_block_reg_offset(op, b_offset, f_offset);
#define WRITE_OP(op) __calc_between_block_reg_offset(op, b_offset, f_offset);

    while (head != next) {
        IR_TYPE* ir = le2struct(next, IR_TYPE, ir_link);

        IR_OP_READ_WRITE(ir, READ_OP, WRITE_OP, PrintErrExit("Not support ir type %s", EnumTypeToString(ir->type)););

        next = list_next(next);
    }

    f_offset->max_offset = f_offset->max_offset < b_offset->now_offset ? f_offset->max_offset : b_offset->now_offset;

#undef READ_OP
#undef WRITE_OP
}

void __add_op_to_user(OPERAND_TYPE* op, BASIC_BLOCK_TYPE* block, struct LinearList* reg_2_user) {
    IfNull(op, return;);
    if (op->type != REGISTER) {
        return;
    }
    int reg_idx = op->operand.reg_idx;
    struct Item* item = getLinearList(reg_2_user, reg_idx);
    if (item == NULL) {
        MALLOC_WITHOUT_DECLARE(item, struct Item, 1);
        MALLOC_WITHOUT_DECLARE(item->key, int, 1);
        *(int*)(item->key) = -1;
        item->value = newDequeList();
        setLinearList(reg_2_user, reg_idx, item);
    }
    pushFrontDequeList(item->value, block);
}

void __add_op_to_def(OPERAND_TYPE* op, BASIC_BLOCK_TYPE* block, struct LinearList* reg_2_def) {
    IfNull(op, return;);
    if (op->type != REGISTER) {
        return;
    }
    int reg_idx = op->operand.reg_idx;
    struct Item* item = getLinearList(reg_2_def, reg_idx);
    if (item == NULL) {
        MALLOC_WITHOUT_DECLARE(item, struct Item, 1);
        MALLOC_WITHOUT_DECLARE(item->key, int, 1);
        *(int*)(item->key) = -1;
        item->value = newDequeList();
        setLinearList(reg_2_def, reg_idx, item);
    }
    pushFrontDequeList(item->value, block);
}

void __add_user_block_to_def(BASIC_BLOCK_TYPE* basic_block, void* args) {
    struct FuncRegOffset* f_offset = (struct FuncRegOffset*)args;
    struct LinearList* reg_2_user = f_offset->reg_2_users;
    struct LinearList* reg_2_def = f_offset->reg_2_defs;
    list_entry_t* ir_head = &basic_block->ir_list->ir_link;
    list_entry_t* ir_next = list_next(ir_head);
    while (ir_head != ir_next) {
        IR_TYPE* ir = le2struct(ir_next, IR_TYPE, ir_link);
#define READ_OP(op) __add_op_to_user(op, basic_block, reg_2_user)
#define WRITE_OP(op) __add_op_to_def(op, basic_block, reg_2_def)
        IR_OP_READ_WRITE(ir, READ_OP, WRITE_OP, break;);
#undef READ_OP
#undef WRITE_OP
        ir_next = list_next(ir_next);
    }
}

struct FuncRegOffset* generatorRegOffset(struct FuncTabElem* elem) {
    struct FuncRegOffset* f_offset = newFuncRegOffset(elem);
    deepTraverseSuccessorsBasicBlock(elem->blocks, __add_user_block_to_def, f_offset);
    deepTraverseSuccessorsBasicBlock(elem->blocks, __calc_between_block_offset, f_offset);
    f_offset->now_offset = f_offset->max_offset;
    deepTraverseSuccessorsBasicBlock(elem->blocks, __calc_in_block_offset, f_offset);
    return f_offset;
}