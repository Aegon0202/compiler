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
    return f_offset;
}

void __read_reg_value_block(OPERAND_TYPE* op, struct BlockRegOffset* b_offset) {
    if (op == NULL) {
        return;
    }
    if (op->type != REGISTER) {  // other is immi
        return;
    }
    int reg_idx = op->operand.reg_idx;
    struct VarTabElem* elem = getLinearList(reg_id_vartabelem, reg_idx);
    if (elem != NULL) {  // 有名字的变量不参与活性分析。
        MALLOC(offset_p, int, 1);
        *offset_p = elem->offset;
        setLinearList(b_offset->reg_offset, reg_idx, offset_p);
        return;
    }

    int* offset_p = getLinearList(b_offset->reg_offset, reg_idx);
    if (offset_p == NULL) {
        MALLOC_WITHOUT_DECLARE(offset_p, int, 1)
        *offset_p = b_offset->now_offset - INT_SIZE;
        b_offset->now_offset -= INT_SIZE;
        setLinearList(b_offset->reg_offset, reg_idx, offset_p);
    }
}

void __write_reg_value_block(OPERAND_TYPE* op, struct BlockRegOffset* b_offset) {
    if (op == NULL) {
        return;
    }
    if (op->type != REGISTER) {  // other is immi
        return;
    }
    int reg_idx = op->operand.reg_idx;
    struct VarTabElem* elem = getLinearList(reg_id_vartabelem, reg_idx);
    if (elem != NULL) {  // 有名字的变量不参与活性分析。
        MALLOC(offset_p, int, 1);
        *offset_p = elem->offset;
        setLinearList(b_offset->reg_offset, reg_idx, offset_p);
        return;
    }

    int* offset_p = getLinearList(b_offset->reg_offset, reg_idx);
    if (offset_p == NULL) {
        MALLOC_WITHOUT_DECLARE(offset_p, int, 1)
        *offset_p = b_offset->now_offset - INT_SIZE;
        b_offset->now_offset -= INT_SIZE;
        setLinearList(b_offset->reg_offset, reg_idx, offset_p);
    }
}

void __generator_block_active(BASIC_BLOCK_TYPE* basic_block, void* args) {
    struct FuncRegOffset* f_offset = (struct FuncRegOffset*)args;
    list_entry_t* head = &(basic_block->ir_list->ir_link);
    list_entry_t* next = head->next;
    struct BlockRegOffset* b_offset = newBlockRegOffset(basic_block, f_offset->now_offset);
    setLinearList(f_offset->block_offsets, (size_t)basic_block, b_offset);

#define READ_OP(num) __read_reg_value_block(op##num, b_offset)
#define WRITE_OP(num) __write_reg_value_block(op##num, b_offset)

    while (head != next) {
        IR_TYPE* ir = le2struct(next, IR_TYPE, ir_link);
        OPERAND_TYPE* op1 = ir->op1;
        OPERAND_TYPE* op2 = ir->op2;
        OPERAND_TYPE* op3 = ir->op3;

        IR_OP_READ_WRITE(ir->type, READ_OP, WRITE_OP, PrintErrExit("Not support ir type %s", EnumTypeToString(ir->type)););

        next = list_next(next);
    }

    f_offset->max_offset = f_offset->max_offset < b_offset->now_offset ? f_offset->max_offset : b_offset->now_offset;

#undef READ_OP
#undef WRITE_OP
}

struct FuncRegOffset* generatorRegOffset(struct FuncTabElem* elem) {
    struct FuncRegOffset* f_offset = newFuncRegOffset(elem);
    deepTraverseSuccessorsBasicBlock(elem->blocks, __generator_block_active, f_offset);
    return f_offset;
}