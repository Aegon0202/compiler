#include "../ssa/ssa.h"
#include "../ssa/traverse.h"
#include "./SysY.target.arm.h"

struct DequeList* string_queue;

const char* block_label(const char* func_name, BASIC_BLOCK_TYPE* basic_block) {
    static char buffer[100];
    snprintf(buffer, 100, "%s_block%p", func_name, basic_block);
    return buffer;
}

struct Operand2* __convert_op_to_op2(OPERAND_TYPE* op, struct DequeList* queue) {
    struct Operand2* op2;
    struct ArmIr* arm_ir;
    if (op->type == INT) {
        long long int num = op->operand.v.intValue;
        if (num > INT8_MAX || num < INT8_MIN) {
            int new_reg_num = alloc_register() + BEGIN_REG_NUM;
            arm_ir = newArmIr(ARM_MOVW, NULL, newRegister(REGISTER, new_reg_num), newImmi_16(num), NULL, NULL);
            pushFrontDequeList(queue, arm_ir);
            if (num > INT16_MAX || num < INT16_MIN) {
                arm_ir = newArmIr(ARM_MOVT, NULL, newRegister(REGISTER, new_reg_num), newImmi_16(num >> 16), NULL, NULL);
                pushFrontDequeList(queue, arm_ir);
            }
            return newOperand(REGISTER, newRegister(REGISTER, new_reg_num));
        }
        return newOperand(IMMI_8, newImmi_8(num));
    } else if (op->type == REGISTER) {
        return newOperand(REGISTER, newRegister(REGISTER, op->operand.reg_idx + BEGIN_REG_NUM));
    } else if (op->type == FRAMEPOINT) {
        int new_reg_num = alloc_register() + BEGIN_REG_NUM;
        void* arm_op1 = newRegister(REGISTER, new_reg_num);
        void* arm_op2 = newRegister(PHISICAL, FP);
        OPERAND_TYPE tmp_op;
        tmp_op.type = INT;
        tmp_op.operand.v.intValue = op->operand.v.intValue;
        void* arm_op3 = __convert_op_to_op2(&tmp_op, queue);
        arm_ir = newArmIr(ARM_ADD, NULL, arm_op1, arm_op2, arm_op3, NULL);
        pushFrontDequeList(queue, arm_ir);
        return newOperand(REGISTER, newRegister(REGISTER, new_reg_num));
    } else if (op->type == STACKPOINT) {
        int new_reg_num = alloc_register() + BEGIN_REG_NUM;
        void* arm_op1 = newRegister(REGISTER, new_reg_num);
        void* arm_op2 = newRegister(PHISICAL, SP);
        OPERAND_TYPE tmp_op;
        tmp_op.type = INT;
        tmp_op.operand.v.intValue = op->operand.v.intValue;
        void* arm_op3 = __convert_op_to_op2(&tmp_op, queue);
        arm_ir = newArmIr(ARM_ADD, NULL, arm_op1, arm_op2, arm_op3, NULL);
        pushFrontDequeList(queue, arm_ir);
        return newOperand(REGISTER, newRegister(REGISTER, new_reg_num));
    } else if (op->type == GLOBALDATA) {
        int new_reg_num = alloc_register() + BEGIN_REG_NUM;
        void* arm_op1 = newRegister(REGISTER, new_reg_num);
        void* arm_op2 = newLabel(((struct VarTabElem*)op->operand.v.intValue)->name);
        arm_ir = newArmIr(ARM_MOVW_L, NULL, arm_op1, arm_op2, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);

        arm_op1 = newRegister(REGISTER, new_reg_num);
        arm_op2 = newLabel(((struct VarTabElem*)op->operand.v.intValue)->name);
        arm_ir = newArmIr(ARM_MOVT_L, NULL, arm_op1, arm_op2, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);

        return newOperand(REGISTER, newRegister(REGISTER, new_reg_num));
    } else if (op->type == ConstSTRING) {
    }
}

#define func_head(type) void __convert_##type##_to_arm(IR_TYPE* ir, struct DequeList* queue, BASIC_BLOCK_TYPE* block, struct FuncTabElem* func)

#define operands                 \
    struct ArmTr* arm_ir = NULL; \
    struct CondOp* cond = NULL;  \
    void* arm_op1 = NULL;        \
    void* arm_op2 = NULL;        \
    void* arm_op3 = NULL;        \
    void* arm_op4 = NULL;

func_head(jump) {
    operands;
    arm_op1 = newLabel(block_label(func->name, ir->op3->operand.v.b));
    arm_ir = newArmIr(ARM_B, cond, arm_op1, arm_op2, arm_op3, arm_op4);
    pushFrontDequeList(queue, arm_ir);
}

func_head(branch) {
    operands;
    if (ir->op1->type == INT) {
        if (ir->op1->operand.v.intValue) {
            arm_op1 = newLabel(block_label(func->name, ir->op2->operand.v.b));
        } else {
            arm_op1 = newLabel(block_label(func->name, ir->op3->operand.v.b));
        }
        arm_ir = newArmIr(ARM_B, cond, arm_op1, arm_op2, arm_op3, arm_op4);
        pushFrontDequeList(queue, arm_ir);
    } else if (ir->op1->type == REGISTER) {
        arm_op1 = newRegister(REGISTER, ir->op1->operand.reg_idx);
        arm_op2 = newOperand(IMMI_8, newImmi_8(0));
        arm_ir = newArmIr(ARM_CMP, cond, arm_op1, arm_op2, arm_op3, arm_op4);
        pushFrontDequeList(queue, arm_ir);

        cond = newCondOp(EQ);
        arm_op1 = newLabel(block_label(func->name, ir->op3->operand.v.b));
        arm_ir = newArmIr(ARM_B, cond, arm_op1, arm_op2, arm_op3, arm_op4);
        pushFrontDequeList(queue, arm_ir);

        cond = NULL;
        arm_op1 = newLabel(block_label(func->name, ir->op2->operand.v.b));
        arm_ir = newArmIr(ARM_B, cond, arm_op1, arm_op2, arm_op3, arm_op4);
        pushFrontDequeList(queue, arm_ir);
    } else {
        arm_op1 = newLabel(block_label(func->name, ir->op2->operand.v.b));
        arm_ir = newArmIr(ARM_B, cond, arm_op1, arm_op2, arm_op3, arm_op4);
        pushFrontDequeList(queue, arm_ir);
    }
}

func_head(return_stmt) {
    operands;
    arm_op1 = newRegister(PHISICAL, A1);
}

#undef func_head
#undef opreands
void __convert_ir_ssa_to_arm(IR_TYPE* ir, struct DequeList* queue, BASIC_BLOCK_TYPE* block, struct FuncTabElem* func) {
    struct ArmTr* arm_ir = NULL;
    switch (ir->type) {
        case NOP:
        case PARAM:
        case CALL:
            break;
            break;
            break;
        case JUMP:
            break;
        case BRANCH:
            READ_OP(ir->op1);
            READ_OP(ir->op2);
            READ_OP(ir->op3);
            break;
        case RETURNSTMT:
            READ_OP(ir->op1);
            break;
        case LOAD:
            READ_OP(ir->op1);
            READ_OP(ir->op2);
            WRITE_OP(ir->op3);
            break;
        case STORE:
            READ_OP(ir->op1);
            READ_OP(ir->op2);
            READ_OP(ir->op3);
            break;
        case ASSIGN:
            READ_OP(ir->op1);
            WRITE_OP(ir->op3);
            break;
        case K_NOT:
            READ_OP(ir->op1);
            WRITE_OP(ir->op3);
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
            READ_OP(ir->op1);
            READ_OP(ir->op2);
            WRITE_OP(ir->op3);
            break;
        default:
    }
}

void __convert_from_ssa_to_arm(BASIC_BLOCK_TYPE* block, void* args) {
    struct FuncTabElem* func = (struct FuncTabElem*)args;

    list_entry_t* arm_ir_head = &block->arm_ir_list->ir_link;
    list_entry_t* ir_head = &block->ir_list->ir_link;
    list_entry_t* ir_elem = list_next(ir_head);

    struct ArmIr* arm_ir = NULL;
    arm_ir = newArmIr(ARM_LABEL, newCondOp(AL), newLabel(block_label(func->name, block)), NULL, NULL, NULL);
    list_add_before(arm_ir_head, &arm_ir->ir_link);
    struct DequeList* arm_ir_queue = newDequeList();

    while (ir_head != ir_elem) {
        ir_elem = list_next(ir_elem);
    }
}