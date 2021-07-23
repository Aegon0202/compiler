#include "../ssa/traverse.h"
#include "./SysY.target.def.h"

static int __reg_id_equal(void* k1, void* k2) {
    return k1 == k2;
}

struct FuncActive* newFuncActive(struct FuncTabElem* elem) {
    MALLOC(f_active, struct FuncActive, 1);
    f_active->funcelem = elem;
    f_active->all_block_active = newLinearList();
    f_active->block_next_index = 0;
    f_active->reg_active = newLinearList();
    f_active->basic_block_active_table = newLinearList();
    return f_active;
}

struct BlockLevelActive* newBlockLevelActive(BASIC_BLOCK_TYPE* basic_block) {
    MALLOC(b_active, struct BlockLevelActive, 1);
    b_active->basic_block = basic_block;
    b_active->ir_total_num = 0;
    b_active->in_block_reg_active = newLinearList();
    b_active->out_block_reg_active = newLinearList();
    b_active->def_in_block = newLinearList();
    b_active->total_reg = newLinearList();
    b_active->total_reg_num = 0;
    return b_active;
}

struct IrLevelActive* newIrLevelActive(REG_ID_TYPE* reg) {
    MALLOC(r_active, struct IrLevelActive, 1);
    r_active->block_line_active = newLinkedTable(__reg_id_equal);
    r_active->reg = reg;
    return r_active;
}

struct ActiveStatus* newActiveStatus(int line_num, BASIC_BLOCK_TYPE* basic_block) {
    MALLOC(as, struct ActiveStatus, 1);
    as->status = NOT_ON_STACK;
    as->action = NOP;
    as->line_num = line_num;
    as->basic_block = basic_block;
    return as;
}

struct LinearList* newActiveStatusBlock(struct IrLevelActive* ir_active, struct BlockLevelActive* b_active, int is_write) {
    struct LinearList* ir_line_status = newLinearList();
    int reg_idx = ir_active->reg;
    setLinearList(ir_active->block_line_active, b_active->basic_block, ir_line_status);
    for (int i = 1; i <= b_active->ir_total_num; i++) {
        struct ActiveStatus* as = newActiveStatus(i, b_active->basic_block);
        as->status = ON_STACK;
        setLinearList(ir_line_status, i, as);
    }
    setLinearList(b_active->out_block_reg_active, reg_idx, ir_active);
    if (is_write) {
        setLinearList(b_active->def_in_block, reg_idx, ir_active);
    } else {
        setLinearList(b_active->in_block_reg_active, reg_idx, ir_active);
    }
    setLinearList(b_active->total_reg, b_active->total_reg_num, ir_active);
    b_active->total_reg_num++;
    return ir_line_status;
}

void __read_reg_value_block(OPERAND_TYPE* op, int line_num, struct BlockLevelActive* b_active, struct FuncActive* f_active) {
    if (op->type != REGISTER) {  // other is immi
        return;
    }
    int reg_idx = op->operand.reg_idx;
    void* elem = getLinearList(reg_id_vartabelem, reg_idx);
    if (elem != NULL) {  // 有名字的变量不参与活性分析。
        return;
    }

    struct IrLevelActive* ir_active = getLinearList(f_active->reg_active, reg_idx);
    if (ir_active == NULL) {
        ir_active = newIrLevelActive(reg_idx);
        setLinearList(f_active->reg_active, reg_idx, ir_active);
    }

    struct LinearList* ir_line_status = getLinearList(ir_active->block_line_active, b_active->basic_block);
    if (ir_line_status == NULL) {
        ir_line_status = newActiveStatusBlock(ir_active, b_active, 0);
    }

    struct ActiveStatus* as = getLinearList(ir_line_status, line_num);
    as->action = READ_REG;
    as->status = MUST_ON_STACK;

    for (int i = line_num - 1; i; i--) {
        as = getLinearList(ir_line_status, i);
        as->status = MUST_ON_STACK;
        if (as->action == WRITE_REG || as->action == READ_REG) {
            break;
        }
    }
}

void __write_reg_value_block(OPERAND_TYPE* op, int line_num, struct BlockLevelActive* b_active, struct FuncActive* f_active) {
    if (op->type != REGISTER) {  // other is immi
        return;
    }
    int reg_idx = op->operand.reg_idx;
    void* elem = getLinearList(reg_id_vartabelem, reg_idx);
    if (elem != NULL) {  // 有名字的变量不参与活性分析。
        return;
    }

    struct IrLevelActive* ir_active = getLinearList(f_active->reg_active, reg_idx);
    if (ir_active == NULL) {
        ir_active = newIrLevelActive(reg_idx);
        setLinearList(f_active->reg_active, reg_idx, ir_active);
    }

    struct LinearList* ir_line_status = getLinearList(ir_active->block_line_active, b_active->basic_block);
    if (ir_line_status == NULL) {
        ir_line_status = ir_line_status = newActiveStatusBlock(ir_active, b_active, 1);
    }

    struct ActiveStatus* as = getLinearList(ir_line_status, line_num);
    as->action = WRITE_REG;
    as->status = ON_STACK;

    for (int i = line_num - 1; i; i--) {
        as = getLinearList(ir_line_status, i);
        if (as->action == READ_REG) {
            break;
        }
        as->status = NOT_ON_STACK;
    }
}

void __generator_block_active(BASIC_BLOCK_TYPE* basic_block, void* func_active) {
    struct FuncActive* f_active = (struct FuncActive*)func_active;
    list_entry_t* head = &(basic_block->ir_list->ir_link);
    list_entry_t* next = head->next;
    int line_num;

    struct BlockLevelActive* b_active = newBlockLevelActive(basic_block);
    setLinearList(f_active->all_block_active, f_active->block_next_index, b_active);
    setLinearList(f_active->basic_block_active_table, (size_t)basic_block, b_active);
    f_active->block_next_index++;

    while (head != next) {
        b_active->ir_total_num++;
        next = list_next(next);
    }
    next = list_next(next);

#define READ_OP(num) __read_reg_value_block(op##num, line_num, b_active, f_active);
#define WRITE_OP(num) __write_reg_value_block(op##num, line_num, b_active, f_active);

    line_num = 0;
    while (head != next) {
        line_num++;
        IR_TYPE* ir = le2struct(next, IR_TYPE, ir_link);
        OPERAND_TYPE* op1 = ir->op1;
        OPERAND_TYPE* op2 = ir->op2;
        OPERAND_TYPE* op3 = ir->op3;
        switch (ir->type) {
            case NOP:
                break;
            case PARAM:
                READ_OP(3);
                break;
            case CALL:
                READ_OP(1);
                READ_OP(2);
                WRITE_OP(3);
                break;
            case JUMP:
                READ_OP(3);
                break;
            case BRANCH:
                READ_OP(1);
                READ_OP(2);
                READ_OP(3);
                break;
            case RETURNSTMT:
                READ_OP(1);
                break;
            case LOAD:
                READ_OP(1);
                READ_OP(2);
                WRITE_OP(3);
                break;
            case STORE:
                READ_OP(1);
                READ_OP(2);
                READ_OP(3);
                break;
            case ASSIGN:
                READ_OP(1);
                WRITE_OP(3);
                break;
            case K_NOT:
                READ_OP(1);
                WRITE_OP(3);
                break;
            case K_ADD:
            case K_SUB:
            case K_MUL:
            case K_DIV:
            case K_MOD:
            case K_AND:
            case K_OR:
            case K_EQ:
            case K_NEQ:
            case K_LT:
            case K_LTE:
            case K_GT:
            case K_GTE:
                READ_OP(1);
                READ_OP(2);
                WRITE_OP(3);
                break;
            default:
                PrintErrExit("Not support ir type %s", EnumTypeToString(ir->type));
        }
        next = list_next(next);
    }

#undef READ_OP
#undef WRITE_OP
}

int __active_from_block(struct BlockLevelActive* b_active, struct FuncActive* f_active) {
    struct LinearList* total = b_active->total_reg;
    int change_other = 0;
    for (int i = 0; i < b_active->total_reg_num; i++) {
        struct IrLevelActive* ir_active = getLinearList(total, i);
        if (getLinearList(b_active->in_block_reg_active, ir_active->reg) == NULL) {
            continue;  // 此寄存器不需要从in 中获取数据。
        }
        BASIC_BLOCK_TYPE* block = b_active->basic_block;
        list_entry_t* head = &(block->predecessors->block_link);
        list_entry_t* next = list_next(head);
        while (next != head) {
            BasicBlockNode* t_node = le2struct(next, BasicBlockNode, block_link);
            BasicBlock* t_block = t_node->value;
            struct BlockLevelActive* t_b_active = getLinearList(f_active->basic_block_active_table, (size_t)t_block);
            struct LinearList* ir_status_list = getLinearList(ir_active->block_line_active, t_block);

            if (ir_status_list == NULL) {
                ir_status_list = newActiveStatusBlock(ir_active, t_b_active, 0);
            }
            for (int j = t_b_active->ir_total_num; j; j--) {
                struct ActiveStatus* as = getLinearList(ir_status_list, j);
                as->status = MUST_ON_STACK;
                if (as->action == WRITE_REG || as->action == READ_REG) {
                    break;
                }
            }
        }
    }
}

struct FuncActive* analyzeFuncLevelActive(struct FuncTabElem* fte) {
    struct FuncActive* f_active = newFuncActive(fte);
    deepTraverseSuccessorsBasicBlock(fte->blocks, __generator_block_active, f_active);
    // mapLinearList(f_active, 0, f_active->block_next_index, __broadcast_from_out_to_in_map, 0, fte);
    return f_active;
}