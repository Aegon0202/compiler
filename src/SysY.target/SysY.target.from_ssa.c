#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include "../ssa/ssa.h"
#include "../ssa/traverse.h"
#include "./SysY.target.arm.h"

struct DequeList* string_queue;
struct Operand2* __convert_op_to_op2(OPERAND_TYPE* op, struct DequeList* queue, struct FuncTabElem* func);

const char* block_label(const char* func_name, BASIC_BLOCK_TYPE* basic_block) {
    static char buffer[100];
    snprintf(buffer, 100, "%s_block%p", func_name, basic_block);
    return buffer;
}

const char* string_label(const char* func_name, const char* str) {
    static char buffer[100];
    snprintf(buffer, 100, "%s_str%p", func_name, str);
    return buffer;
}

int __can_jump_to_return(IR_TYPE* ir) {
    if (ir->type != JUMP) {
        return 0;
    }
    if (ir->op3->type != BASIC_BLOCK) {
        return 0;
    }
    BASIC_BLOCK_TYPE* basic_block = ir->op3->operand.v.b;
    IR_TYPE* first_ir = le2struct(list_next(&basic_block->ir_list->ir_link), IR_TYPE, ir_link);
    if (first_ir->type != RETURNSTMT && first_ir->type != JUMP) {
        return 0;
    }
    if (first_ir->type == RETURNSTMT) {
        if (first_ir->op1 == NULL) {
            return 1;
        }
        return 0;
    } else {
        return __can_jump_to_return(first_ir);
    }
    return 0;
}

int __is_tail_call(IR_TYPE* ir, BASIC_BLOCK_TYPE* bsaic_bock) {
    IR_TYPE* next_ir = le2struct(list_next(&ir->ir_link), IR_TYPE, ir_link);
    if (ir->type != CALL) {
        return 0;
    }
    if (next_ir->type != RETURNSTMT && next_ir->type != JUMP) {
        return 0;
    }
    if (next_ir->type == RETURNSTMT) {
        if (next_ir->op1 == NULL) {
            return 1;
        }
        if (next_ir->op1->type != REGISTER) {
            return 0;
        }
        if (next_ir->op1->operand.reg_idx != ir->op3->operand.reg_idx) {
            return 0;
        }
        return 1;
    } else {
        return __can_jump_to_return(next_ir);
    }
    return 0;
}

struct Label* __convert_op_to_label(OPERAND_TYPE* op, struct FuncTabElem* func) {
    switch (op->type) {
        case GLOBALDATA:
            return newLabel(((struct VarTabElem*)op->operand.v.intValue)->name);
        case ConstSTRING:
            return newLabel(string_label(func->name, op->operand.v.str));
        case FUNCID:
            return newLabel(((struct FuncTabElem*)op->operand.v.funcID)->name);
        case BASIC_BLOCK:
            return newLabel(block_label(func->name, op->operand.v.b));
        default:
            PrintErrExit("label not support op type %s\n", EnumTypeToString(op->type));
    }
    return NULL;
}

struct Register* __convert_op_to_reg(OPERAND_TYPE* op, struct DequeList* queue, struct FuncTabElem* func) {
    struct ArmIr* arm_ir;
    if (op->type == INT) {
        long long int num = op->operand.v.intValue;
        int new_reg_num = alloc_register() + BEGIN_REG_NUM;
        arm_ir = newArmIr(ARM_MOVW, NULL, newRegister(REGISTER, new_reg_num), newImmi_16(num), NULL, NULL);
        pushFrontDequeList(queue, arm_ir);
        if (num > INT16_MAX || num < INT16_MIN) {
            arm_ir = newArmIr(ARM_MOVT, NULL, newRegister(REGISTER, new_reg_num), newImmi_16(num >> 16), NULL, NULL);
            pushFrontDequeList(queue, arm_ir);
        }
        return newRegister(REGISTER, new_reg_num);
    } else if (op->type == REGISTER) {
        return newRegister(REGISTER, op->operand.reg_idx + BEGIN_REG_NUM);
    } else if (op->type == FRAMEPOINT) {
        int new_reg_num = alloc_register() + BEGIN_REG_NUM;
        void* arm_op1 = newRegister(REGISTER, new_reg_num);
        void* arm_op2 = newRegister(PHISICAL, FP);
        OPERAND_TYPE tmp_op;
        tmp_op.type = INT;
        tmp_op.operand.v.intValue = op->operand.v.intValue;
        void* arm_op3 = __convert_op_to_op2(&tmp_op, queue, func);
        arm_ir = newArmIr(ARM_ADD, NULL, arm_op1, arm_op2, arm_op3, NULL);
        pushFrontDequeList(queue, arm_ir);
        return newRegister(REGISTER, new_reg_num);
    } else if (op->type == STACKPOINT) {
        int new_reg_num = alloc_register() + BEGIN_REG_NUM;
        void* arm_op1 = newRegister(REGISTER, new_reg_num);
        void* arm_op2 = newRegister(PHISICAL, SP);
        OPERAND_TYPE tmp_op;
        tmp_op.type = INT;
        tmp_op.operand.v.intValue = op->operand.v.intValue;
        void* arm_op3 = __convert_op_to_op2(&tmp_op, queue, func);
        arm_ir = newArmIr(ARM_ADD, NULL, arm_op1, arm_op2, arm_op3, NULL);
        pushFrontDequeList(queue, arm_ir);
        return newRegister(REGISTER, new_reg_num);
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

        return newRegister(REGISTER, new_reg_num);
    } else if (op->type == ConstSTRING) {
        int new_reg_num = alloc_register() + BEGIN_REG_NUM;
        MALLOC(item, struct Item, 1);
        item->key = strdup(string_label(func->name, op->operand.v.str));
        item->value = strdup(op->operand.v.str);
        pushFrontDequeList(string_queue, item);

        void* arm_op1 = newRegister(REGISTER, new_reg_num);
        void* arm_op2 = newLabel(item->key);
        arm_ir = newArmIr(ARM_MOVW_L, NULL, arm_op1, arm_op2, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);

        arm_op1 = newRegister(REGISTER, new_reg_num);
        arm_op2 = newLabel(item->key);
        arm_ir = newArmIr(ARM_MOVT_L, NULL, arm_op1, arm_op2, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);

        return newRegister(REGISTER, new_reg_num);
    } else if (op->type == FUNCID) {
        int new_reg_num = alloc_register() + BEGIN_REG_NUM;
        void* arm_op1 = newRegister(REGISTER, new_reg_num);
        void* arm_op2 = newLabel(((struct FuncTabElem*)op->operand.v.funcID)->name);
        arm_ir = newArmIr(ARM_MOVW_L, NULL, arm_op1, arm_op2, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);

        arm_op1 = newRegister(REGISTER, new_reg_num);
        arm_op2 = newLabel(((struct FuncTabElem*)op->operand.v.funcID)->name);
        arm_ir = newArmIr(ARM_MOVT_L, NULL, arm_op1, arm_op2, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);

        return newRegister(REGISTER, new_reg_num);
    } else if (op->type == BASIC_BLOCK) {
        int new_reg_num = alloc_register() + BEGIN_REG_NUM;
        void* arm_op1 = newRegister(REGISTER, new_reg_num);
        void* arm_op2 = newLabel(block_label(func->name, op->operand.v.b));
        arm_ir = newArmIr(ARM_MOVW_L, NULL, arm_op1, arm_op2, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);

        arm_op1 = newRegister(REGISTER, new_reg_num);
        arm_op2 = newLabel(block_label(func->name, op->operand.v.b));
        arm_ir = newArmIr(ARM_MOVT_L, NULL, arm_op1, arm_op2, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);

        return newRegister(REGISTER, new_reg_num);
    }

    PrintErrExit("not support op type %s", EnumTypeToString(op->type));
    return NULL;
}

struct Operand2* __convert_op_to_op2(OPERAND_TYPE* op, struct DequeList* queue, struct FuncTabElem* func) {
    if (op->type == INT) {
        long long int num = op->operand.v.intValue;
        if (num >= INT8_MIN && num <= INT8_MAX) {
            return newOperand2(IMMI_8, newImmi_8(num));
        }
    }
    return newOperand2(REGISTER, __convert_op_to_reg(op, queue, func));
}

void __generator_func_return(struct DequeList* queue) {
    OPERAND_TYPE tmp_op;
    struct ArmIr* arm_ir;
    void* arm_op1;
    void* arm_op2;
    void* arm_op3;
    tmp_op.type = INT;
    tmp_op.operand.v.intValue = -32;
    arm_op1 = newRegister(PHISICAL, SP);
    arm_op2 = newRegister(PHISICAL, FP);
    arm_op3 = __convert_op_to_op2(&tmp_op, queue, NULL);
    arm_ir = newArmIr(ARM_ADD, NULL, arm_op1, arm_op2, arm_op3, NULL);
    pushFrontDequeList(queue, arm_ir);

    arm_op1 = newDequeList();
    pushFrontDequeList(arm_op1, newRegister(PHISICAL, V1));
    pushFrontDequeList(arm_op1, newRegister(PHISICAL, V2));
    pushFrontDequeList(arm_op1, newRegister(PHISICAL, V3));
    pushFrontDequeList(arm_op1, newRegister(PHISICAL, V4));
    pushFrontDequeList(arm_op1, newRegister(PHISICAL, V5));
    pushFrontDequeList(arm_op1, newRegister(PHISICAL, V6));
    pushFrontDequeList(arm_op1, newRegister(PHISICAL, V7));
    arm_ir = newArmIr(ARM_POP, NULL, arm_op1, NULL, NULL, NULL);
    pushFrontDequeList(queue, arm_ir);

    arm_op1 = newDequeList();
    pushFrontDequeList(arm_op1, newRegister(PHISICAL, FP));
    pushFrontDequeList(arm_op1, newRegister(PHISICAL, LR));
    arm_ir = newArmIr(ARM_POP, NULL, arm_op1, NULL, NULL, NULL);
    pushFrontDequeList(queue, arm_ir);

    tmp_op.operand.v.intValue = 16;
    arm_op1 = newRegister(PHISICAL, SP);
    arm_op2 = newRegister(PHISICAL, SP);
    arm_op3 = __convert_op_to_op2(&tmp_op, queue, NULL);
    arm_ir = newArmIr(ARM_ADD, NULL, arm_op1, arm_op2, arm_op3, NULL);
    pushFrontDequeList(queue, arm_ir);
}

#define func_head(type) void __convert_##type##_to_arm(IR_TYPE* ir, struct DequeList* queue, BASIC_BLOCK_TYPE* block, struct FuncTabElem* func)

#define operands                 \
    struct ArmIr* arm_ir = NULL; \
    struct CondOp* cond = NULL;  \
    void* arm_op1 = NULL;        \
    void* arm_op2 = NULL;        \
    void* arm_op3 = NULL;        \
    void* arm_op4 = NULL;

func_head(jump) {
    struct ArmIr* arm_ir = NULL;
    void* arm_op1 = NULL;

    arm_op1 = __convert_op_to_label(ir->op3, func);
    arm_ir = newArmIr(ARM_B, NULL, arm_op1, NULL, NULL, NULL);
    pushFrontDequeList(queue, arm_ir);
}

func_head(branch) {
    struct ArmIr* arm_ir = NULL;
    struct CondOp* cond = NULL;
    void* arm_op1 = NULL;
    void* arm_op2 = NULL;

    arm_op1 = __convert_op_to_reg(ir->op1, queue, func);
    arm_op2 = newOperand2(IMMI_8, newImmi_8(0));
    arm_ir = newArmIr(ARM_CMP, NULL, arm_op1, arm_op2, NULL, NULL);
    pushFrontDequeList(queue, arm_ir);
    arm_op2 = NULL;

    cond = newCondOp(EQ);
    arm_op1 = __convert_op_to_label(ir->op3, func);
    arm_ir = newArmIr(ARM_B, cond, arm_op1, NULL, NULL, NULL);
    pushFrontDequeList(queue, arm_ir);

    arm_op1 = __convert_op_to_label(ir->op2, func);
    arm_ir = newArmIr(ARM_B, NULL, arm_op1, NULL, NULL, NULL);
    pushFrontDequeList(queue, arm_ir);
}

func_head(return_stmt) {
    struct ArmIr* arm_ir = NULL;
    void* arm_op1 = NULL;
    void* arm_op2 = NULL;

    if (ir->op1 == NULL) {
        ir->op1 = create_new_operand(INT, -1, 0);
    }

    arm_op1 = newRegister(PHISICAL, A1);
    arm_op2 = __convert_op_to_op2(ir->op1, queue, func);
    arm_ir = newArmIr(ARM_MOV, NULL, arm_op1, arm_op2, NULL, NULL);
    pushFrontDequeList(queue, arm_ir);

    __generator_func_return(queue);
    arm_op1 = newRegister(PHISICAL, LR);
    arm_ir = newArmIr(ARM_BX, NULL, arm_op1, NULL, NULL, NULL);
    pushFrontDequeList(queue, arm_ir);
}

func_head(load) {
    struct ArmIr* arm_ir = NULL;
    void* arm_op1 = NULL;
    void* arm_op2 = NULL;
    void* arm_op3 = NULL;

    arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
    arm_op2 = __convert_op_to_reg(ir->op1, queue, func);
    if (ir->op2->type == INT &&
        ir->op2->operand.v.intValue < 4095 &&
        ir->op2->operand.v.intValue > -4095) {
        arm_op3 = newImmi_12(ir->op2->operand.v.intValue);
        arm_ir = newArmIr(ARM_LDR_I, NULL, arm_op1, arm_op2, arm_op3, NULL);
        pushFrontDequeList(queue, arm_ir);
    } else {
        arm_op3 = __convert_op_to_op2(ir->op2, queue, func);
        arm_ir = newArmIr(ARM_LDR_R, NULL, arm_op1, arm_op2, arm_op3, NULL);
        pushFrontDequeList(queue, arm_ir);
    }
}

func_head(store) {
    struct ArmIr* arm_ir = NULL;
    void* arm_op1 = NULL;
    void* arm_op2 = NULL;
    void* arm_op3 = NULL;

    arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
    arm_op2 = __convert_op_to_reg(ir->op1, queue, func);
    if (ir->op2->type == INT &&
        ir->op2->operand.v.intValue < 4095 &&
        ir->op2->operand.v.intValue > -4095) {
        arm_op3 = newImmi_12(ir->op2->operand.v.intValue);
        arm_ir = newArmIr(ARM_STR_I, NULL, arm_op1, arm_op2, arm_op3, NULL);
        pushFrontDequeList(queue, arm_ir);
    } else {
        arm_op3 = __convert_op_to_op2(ir->op2, queue, func);
        arm_ir = newArmIr(ARM_STR_R, NULL, arm_op1, arm_op2, arm_op3, NULL);
        pushFrontDequeList(queue, arm_ir);
    }
}

func_head(assign) {
    struct ArmIr* arm_ir = NULL;
    void* arm_op1 = NULL;
    void* arm_op2 = NULL;
    void* arm_op3 = NULL;

    if (ir->op1->type == INT) {
        long long int num = ir->op1->operand.v.intValue;
        arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
        arm_ir = newArmIr(ARM_MOVW, NULL, arm_op1, newImmi_16(num), NULL, NULL);
        pushFrontDequeList(queue, arm_ir);
        if (num > INT16_MAX || num < INT16_MIN) {
            arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
            arm_ir = newArmIr(ARM_MOVT, NULL, arm_op1, newImmi_16(num >> 16), NULL, NULL);
            pushFrontDequeList(queue, arm_ir);
        }
    } else if (ir->op1->type == REGISTER) {
        arm_op2 = __convert_op_to_op2(ir->op1, queue, func);
        arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
        arm_ir = newArmIr(ARM_MOV, NULL, arm_op1, arm_op2, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);
    } else if (ir->op1->type == FRAMEPOINT) {
        arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
        arm_op2 = newRegister(PHISICAL, FP);
        OPERAND_TYPE tmp_op;
        tmp_op.type = INT;
        tmp_op.operand.v.intValue = ir->op1->operand.v.intValue;
        arm_op3 = __convert_op_to_op2(&tmp_op, queue, func);
        arm_ir = newArmIr(ARM_ADD, NULL, arm_op1, arm_op2, arm_op3, NULL);
        pushFrontDequeList(queue, arm_ir);
    } else if (ir->op1->type == STACKPOINT) {
        arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
        arm_op2 = newRegister(PHISICAL, SP);
        OPERAND_TYPE tmp_op;
        tmp_op.type = INT;
        tmp_op.operand.v.intValue = ir->op1->operand.v.intValue;
        arm_op3 = __convert_op_to_op2(&tmp_op, queue, func);
        arm_ir = newArmIr(ARM_ADD, NULL, arm_op1, arm_op2, arm_op3, NULL);
        pushFrontDequeList(queue, arm_ir);
    } else if (ir->op1->type == GLOBALDATA) {
        arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
        arm_op2 = newLabel(((struct VarTabElem*)ir->op1->operand.v.intValue)->name);
        arm_ir = newArmIr(ARM_MOVW_L, NULL, arm_op1, arm_op2, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);

        arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
        arm_op2 = newLabel(((struct VarTabElem*)ir->op1->operand.v.intValue)->name);
        arm_ir = newArmIr(ARM_MOVT_L, NULL, arm_op1, arm_op2, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);

    } else if (ir->op1->type == ConstSTRING) {
        MALLOC(item, struct Item, 1);
        item->key = strdup(string_label(func->name, ir->op1->operand.v.str));
        item->value = strdup(ir->op1->operand.v.str);
        pushFrontDequeList(string_queue, item);

        arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
        arm_op2 = newLabel(item->key);
        arm_ir = newArmIr(ARM_MOVW_L, NULL, arm_op1, arm_op2, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);

        arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
        arm_op2 = newLabel(item->key);
        arm_ir = newArmIr(ARM_MOVT_L, NULL, arm_op1, arm_op2, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);

    } else if (ir->op1->type == FUNCID) {
        arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
        arm_op2 = newLabel(((struct FuncTabElem*)ir->op1->operand.v.funcID)->name);
        arm_ir = newArmIr(ARM_MOVW_L, NULL, arm_op1, arm_op2, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);

        arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
        arm_op2 = newLabel(((struct FuncTabElem*)ir->op1->operand.v.funcID)->name);
        arm_ir = newArmIr(ARM_MOVT_L, NULL, arm_op1, arm_op2, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);

    } else if (ir->op1->type == BASIC_BLOCK) {
        arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
        arm_op2 = newLabel(block_label(func->name, ir->op1->operand.v.b));
        arm_ir = newArmIr(ARM_MOVW_L, NULL, arm_op1, arm_op2, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);

        arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
        arm_op2 = newLabel(block_label(func->name, ir->op1->operand.v.b));
        arm_ir = newArmIr(ARM_MOVT_L, NULL, arm_op1, arm_op2, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);

    } else {
        PrintErrExit("not support op type %s", EnumTypeToString(ir->op1->type));
    }
}

func_head(k_not) {
    struct ArmIr* arm_ir = NULL;
    struct CondOp* cond = NULL;
    void* arm_op1 = NULL;
    void* arm_op2 = NULL;

    OPERAND_TYPE tmp_op;
    tmp_op.type = INT;
    tmp_op.operand.v.intValue = 0;
    arm_op1 = __convert_op_to_reg(ir->op1, queue, func);
    arm_op2 = __convert_op_to_op2(&tmp_op, queue, func);
    arm_ir = newArmIr(ARM_CMP, NULL, arm_op1, arm_op2, NULL, NULL);
    pushFrontDequeList(queue, arm_ir);

    cond = newCondOp(EQ);
    arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
    tmp_op.operand.v.intValue = 1;
    arm_op2 = __convert_op_to_op2(&tmp_op, queue, func);
    arm_ir = newArmIr(ARM_MOV, cond, arm_op1, arm_op2, NULL, NULL);
    pushFrontDequeList(queue, arm_ir);

    cond = newCondOp(NE);
    arm_op1 = __convert_op_to_reg(ir->op2, queue, func);
    tmp_op.operand.v.intValue = 0;
    arm_op2 = __convert_op_to_op2(&tmp_op, queue, func);
    arm_ir = newArmIr(ARM_MOV, cond, arm_op1, arm_op2, NULL, NULL);
    pushFrontDequeList(queue, arm_ir);
}

#define convert_ir_bi_op(type, op)                                          \
    func_head(type) {                                                       \
        struct ArmIr* arm_ir = NULL;                                        \
        void* arm_op1 = NULL;                                               \
        void* arm_op2 = NULL;                                               \
        void* arm_op3 = NULL;                                               \
        arm_op1 = __convert_op_to_reg(ir->op3, queue, func);                \
        arm_op2 = __convert_op_to_reg(ir->op1, queue, func);                \
        arm_op3 = __convert_op_to_op2(ir->op2, queue, func);                \
        arm_ir = newArmIr(ARM_##op, NULL, arm_op1, arm_op2, arm_op3, NULL); \
        pushFrontDequeList(queue, arm_ir);                                  \
    }

convert_ir_bi_op(k_add, ADD);
convert_ir_bi_op(k_sub, SUB);
#undef convert_ir_bi_op

#define convert_ir_bi_op(type, op)                                          \
    func_head(type) {                                                       \
        struct ArmIr* arm_ir = NULL;                                        \
        void* arm_op1 = NULL;                                               \
        void* arm_op2 = NULL;                                               \
        void* arm_op3 = NULL;                                               \
                                                                            \
        arm_op1 = __convert_op_to_reg(ir->op3, queue, func);                \
        arm_op2 = __convert_op_to_reg(ir->op1, queue, func);                \
        arm_op3 = __convert_op_to_reg(ir->op2, queue, func);                \
        arm_ir = newArmIr(ARM_##op, NULL, arm_op1, arm_op2, arm_op3, NULL); \
        pushFrontDequeList(queue, arm_ir);                                  \
    }
convert_ir_bi_op(k_mul, MUL);
convert_ir_bi_op(k_div, SDIV);
#undef convert_ir_bi_op
func_head(k_mod) {
    struct ArmIr* arm_ir = NULL;
    void* arm_op1 = NULL;
    void* arm_op2 = NULL;
    void* arm_op3 = NULL;

    void* numerator = __convert_op_to_reg(ir->op1, queue, func);    // 分子
    void* denominator = __convert_op_to_reg(ir->op2, queue, func);  //分母
    int new_reg_num = alloc_register() + BEGIN_REG_NUM;
    arm_op1 = newRegister(REGISTER, new_reg_num);
    arm_op2 = numerator;
    arm_op3 = denominator;
    arm_ir = newArmIr(ARM_SDIV, NULL, arm_op1, arm_op2, arm_op3, NULL);
    pushFrontDequeList(queue, arm_ir);

    arm_op2 = newRegister(REGISTER, new_reg_num);
    new_reg_num = alloc_register() + BEGIN_REG_NUM;
    arm_op1 = newRegister(REGISTER, new_reg_num);
    arm_op3 = newRegister(((struct Register*)denominator)->type, ((struct Register*)denominator)->reg);
    arm_ir = newArmIr(ARM_MUL, NULL, arm_op1, arm_op2, arm_op3, NULL);
    pushFrontDequeList(queue, arm_ir);

    arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
    arm_op2 = newRegister(((struct Register*)numerator)->type, ((struct Register*)numerator)->reg);
    arm_op3 = newRegister(REGISTER, new_reg_num);
    arm_ir = newArmIr(ARM_SUB, NULL, arm_op1, arm_op2, arm_op3, NULL);
    pushFrontDequeList(queue, arm_ir);
}

#define convert_ir_bi_op(c_type, true_cond, false_cond)                 \
    func_head(c_type) {                                                 \
        struct ArmIr* arm_ir = NULL;                                    \
        struct CondOp* cond = NULL;                                     \
        void* arm_op1 = NULL;                                           \
        void* arm_op2 = NULL;                                           \
                                                                        \
        OPERAND_TYPE tmp_op;                                            \
        tmp_op.type = INT;                                              \
        tmp_op.operand.v.intValue = 0;                                  \
        arm_op1 = __convert_op_to_reg(ir->op1, queue, func);            \
        arm_op2 = __convert_op_to_op2(ir->op2, queue, func);            \
        arm_ir = newArmIr(ARM_CMP, NULL, arm_op1, arm_op2, NULL, NULL); \
        pushFrontDequeList(queue, arm_ir);                              \
                                                                        \
        cond = newCondOp(true_cond);                                    \
        arm_op1 = __convert_op_to_reg(ir->op2, queue, func);            \
        tmp_op.operand.v.intValue = 1;                                  \
        arm_op2 = __convert_op_to_op2(&tmp_op, queue, func);            \
        arm_ir = newArmIr(ARM_MOV, cond, arm_op1, arm_op2, NULL, NULL); \
        pushFrontDequeList(queue, arm_ir);                              \
                                                                        \
        cond = newCondOp(false_cond);                                   \
        arm_op1 = __convert_op_to_reg(ir->op2, queue, func);            \
        tmp_op.operand.v.intValue = 0;                                  \
        arm_op2 = __convert_op_to_op2(&tmp_op, queue, func);            \
        arm_ir = newArmIr(ARM_MOV, cond, arm_op1, arm_op2, NULL, NULL); \
        pushFrontDequeList(queue, arm_ir);                              \
    }
convert_ir_bi_op(k_eq, EQ, NE);
convert_ir_bi_op(k_neq, NE, EQ);
convert_ir_bi_op(k_lt, LT, GE);
convert_ir_bi_op(k_lte, LE, GT);
convert_ir_bi_op(k_gt, GT, LE);
convert_ir_bi_op(k_gte, GE, LT);
#undef convert_ir_bi_op

func_head(normal_call) {
    struct ArmIr* arm_ir = NULL;
    void* arm_op1 = NULL;
    void* arm_op2 = NULL;
    void* arm_op3 = NULL;

    int param_num = ir->op2->operand.v.intValue;
    list_entry_t* param_elem = list_prev(&ir->ir_link);
    for (int i = 0; i < param_num; i++) {
        IR_LIST_TYPE* param_ir = le2struct(param_elem, IR_TYPE, ir_link);
        param_elem = list_prev(param_elem);
        if (param_ir->op2->operand.v.intValue == 1) {
            arm_op1 = newRegister(PHISICAL, A1);
            arm_op2 = __convert_op_to_op2(param_ir->op3, queue, func);
            arm_ir = newArmIr(ARM_MOV, NULL, arm_op1, arm_op2, NULL, NULL);
            pushFrontDequeList(queue, arm_ir);
        } else if (param_ir->op2->operand.v.intValue == 2) {
            arm_op1 = newRegister(PHISICAL, A2);
            arm_op2 = __convert_op_to_op2(param_ir->op3, queue, func);
            arm_ir = newArmIr(ARM_MOV, NULL, arm_op1, arm_op2, NULL, NULL);
            pushFrontDequeList(queue, arm_ir);
        } else if (param_ir->op2->operand.v.intValue == 3) {
            arm_op1 = newRegister(PHISICAL, A3);
            arm_op2 = __convert_op_to_op2(param_ir->op3, queue, func);
            arm_ir = newArmIr(ARM_MOV, NULL, arm_op1, arm_op2, NULL, NULL);
            pushFrontDequeList(queue, arm_ir);
        } else if (param_ir->op2->operand.v.intValue == 4) {
            arm_op1 = newRegister(PHISICAL, A4);
            arm_op2 = __convert_op_to_op2(param_ir->op3, queue, func);
            arm_ir = newArmIr(ARM_MOV, NULL, arm_op1, arm_op2, NULL, NULL);
            pushFrontDequeList(queue, arm_ir);
        } else {
            arm_op1 = newDequeList();
            pushFrontDequeList(arm_op1, __convert_op_to_reg(param_ir->op3, queue, func));
            arm_op2 = NULL;
            arm_ir = newArmIr(ARM_PUSH, NULL, arm_op1, NULL, NULL, NULL);
            pushFrontDequeList(queue, arm_ir);
        }
    }

    arm_op1 = __convert_op_to_label(ir->op1, func);
    MALLOC_WITHOUT_DECLARE(arm_op2, int, 1);
    *(int*)arm_op2 = param_num;
    arm_ir = newArmIr(ARM_BL, NULL, arm_op1, arm_op2, NULL, NULL);
    pushFrontDequeList(queue, arm_ir);

    arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
    arm_op2 = newRegister(PHISICAL, A1);
    arm_ir = newArmIr(ARM_MOV, NULL, arm_op1, arm_op2, NULL, NULL);
    pushFrontDequeList(queue, arm_ir);

    if (ir->op2->operand.v.intValue > 4) {
        OPERAND_TYPE tmp_op;
        tmp_op.type = INT;
        tmp_op.operand.v.intValue = ((int)((ir->op2->operand.v.intValue - 4) * INT_SIZE));
        arm_op1 = newRegister(PHISICAL, SP);
        arm_op2 = newRegister(PHISICAL, SP);
        arm_op3 = __convert_op_to_op2(&tmp_op, queue, func);
        arm_ir = newArmIr(ARM_ADD, NULL, arm_op1, arm_op2, arm_op3, NULL);
        pushFrontDequeList(queue, arm_ir);
    }
}

func_head(tail_recusive) {
    struct ArmIr* arm_ir = NULL;
    void* arm_op1 = NULL;
    void* arm_op2 = NULL;
    void* arm_op3 = NULL;

    list_entry_t* param_elem = list_prev(&ir->ir_link);
    int param_num = ir->op2->operand.v.intValue;
    int fp_offset = 4;
    OPERAND_TYPE tmp_op;
    tmp_op.type = INT;
    tmp_op.operand.v.intValue = 0;

    for (int i = 0; i < param_num; i++) {
        IR_TYPE* param_ir = le2struct(param_elem, IR_TYPE, ir_link);
        param_elem = list_prev(param_elem);
        arm_op1 = newDequeList();
        pushFrontDequeList(arm_op1, __convert_op_to_reg(param_ir->op3, queue, func));
        arm_ir = newArmIr(ARM_PUSH, NULL, arm_op1, NULL, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);
    }

    param_elem = list_next(param_elem);
    for (int i = 0; i < param_num; i++) {
        int new_reg = alloc_register() + BEGIN_REG_NUM;
        param_elem = list_next(param_elem);
        arm_op1 = newDequeList();
        pushFrontDequeList(arm_op1, newRegister(REGISTER, new_reg));
        arm_ir = newArmIr(ARM_POP, NULL, arm_op1, NULL, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);

        arm_op2 = newRegister(PHISICAL, FP);
        arm_op1 = newRegister(REGISTER, new_reg);
        if (fp_offset < 4095 && fp_offset > -4095) {
            arm_op3 = newImmi_12(fp_offset);
            arm_ir = newArmIr(ARM_STR_I, NULL, arm_op1, arm_op2, arm_op3, NULL);
            pushFrontDequeList(queue, arm_ir);
        } else {
            tmp_op.operand.v.intValue = fp_offset;
            arm_op3 = __convert_op_to_op2(&tmp_op, queue, func);
            arm_ir = newArmIr(ARM_STR_R, NULL, arm_op1, arm_op2, arm_op3, NULL);
            pushFrontDequeList(queue, arm_ir);
        }
        fp_offset += INT_SIZE;
    }
    tmp_op.type = BASIC_BLOCK;
    tmp_op.operand.v.b = func->blocks;
    arm_op1 = __convert_op_to_label(&tmp_op, func);
    arm_ir = newArmIr(ARM_B, NULL, arm_op1, NULL, NULL, NULL);
    pushFrontDequeList(queue, arm_ir);
}

func_head(tail_call) {
    struct ArmIr* arm_ir = NULL;
    void* arm_op1 = NULL;
    void* arm_op2 = NULL;
    void* arm_op3 = NULL;

    int param_num = ir->op2->operand.v.intValue;
    int fp_offset = 4;
    OPERAND_TYPE tmp_op;
    tmp_op.type = INT;
    tmp_op.operand.v.intValue = 0;

    list_entry_t* param_elem = list_prev(&ir->ir_link);
    for (int i = 0; i < param_num; i++) {
        param_elem = list_prev(param_elem);
    }

    param_elem = list_next(param_elem);
    for (int i = 0; i < param_num; i++) {
        IR_TYPE* param_ir = le2struct(param_elem, IR_TYPE, ir_link);
        param_elem = list_next(param_elem);
        arm_op1 = newDequeList();
        pushFrontDequeList(arm_op1, __convert_op_to_reg(param_ir->op3, queue, func));
        arm_ir = newArmIr(ARM_PUSH, NULL, arm_op1, NULL, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);
    }

    param_elem = list_prev(param_elem);
    fp_offset = -32;
    for (int i = 0; i < param_num; i++) {
        IR_TYPE* param_ir = le2struct(param_elem, IR_TYPE, ir_link);
        param_elem = list_prev(param_elem);

        int new_reg = alloc_register() + BEGIN_REG_NUM;
        arm_op1 = newDequeList();
        pushFrontDequeList(arm_op1, newRegister(REGISTER, new_reg));
        arm_ir = newArmIr(ARM_POP, NULL, arm_op1, NULL, NULL, NULL);
        pushFrontDequeList(queue, arm_ir);

        if (param_ir->op2->operand.v.intValue == 1) {
            arm_op1 = newRegister(PHISICAL, A1);
            arm_op2 = __convert_op_to_op2(param_ir->op3, queue, func);
            arm_ir = newArmIr(ARM_MOV, NULL, arm_op1, arm_op2, NULL, NULL);
            pushFrontDequeList(queue, arm_ir);
        } else if (param_ir->op2->operand.v.intValue == 2) {
            arm_op1 = newRegister(PHISICAL, A2);
            arm_op2 = __convert_op_to_op2(param_ir->op3, queue, func);
            arm_ir = newArmIr(ARM_MOV, NULL, arm_op1, arm_op2, NULL, NULL);
            pushFrontDequeList(queue, arm_ir);
        } else if (param_ir->op2->operand.v.intValue == 3) {
            arm_op1 = newRegister(PHISICAL, A3);
            arm_op2 = __convert_op_to_op2(param_ir->op3, queue, func);
            arm_ir = newArmIr(ARM_MOV, NULL, arm_op1, arm_op2, NULL, NULL);
            pushFrontDequeList(queue, arm_ir);
        } else if (param_ir->op2->operand.v.intValue == 4) {
            arm_op1 = newRegister(PHISICAL, A4);
            arm_op2 = __convert_op_to_op2(param_ir->op3, queue, func);
            arm_ir = newArmIr(ARM_MOV, NULL, arm_op1, arm_op2, NULL, NULL);
            pushFrontDequeList(queue, arm_ir);
        } else {
            fp_offset -= INT_SIZE;
            arm_op2 = newRegister(PHISICAL, FP);
            arm_op1 = newRegister(REGISTER, new_reg);
            if (fp_offset < 4095 && fp_offset > -4095) {
                arm_op3 = newImmi_12(fp_offset);
                arm_ir = newArmIr(ARM_STR_I, NULL, arm_op1, arm_op2, arm_op3, NULL);
                pushFrontDequeList(queue, arm_ir);
            } else {
                tmp_op.operand.v.intValue = fp_offset;
                arm_op3 = __convert_op_to_op2(&tmp_op, queue, func);
                arm_ir = newArmIr(ARM_STR_R, NULL, arm_op1, arm_op2, arm_op3, NULL);
                pushFrontDequeList(queue, arm_ir);
            }
        }
    }
    tmp_op.operand.v.intValue = fp_offset;
    arm_op1 = newRegister(PHISICAL, SP);
    arm_op2 = newRegister(PHISICAL, FP);
    arm_op3 = __convert_op_to_op2(&tmp_op, queue, func);
    arm_ir = newArmIr(ARM_ADD, NULL, arm_op1, arm_op2, arm_op3, NULL);
    pushFrontDequeList(queue, arm_ir);

    arm_op1 = __convert_op_to_label(ir->op1, func);
    MALLOC_WITHOUT_DECLARE(arm_op2, int, 1);
    *(int*)arm_op2 = param_num;
    arm_ir = newArmIr(ARM_BL, NULL, arm_op1, arm_op2, NULL, NULL);
    pushFrontDequeList(queue, arm_ir);

    arm_op1 = __convert_op_to_reg(ir->op3, queue, func);
    arm_op2 = newRegister(PHISICAL, A1);
    arm_ir = newArmIr(ARM_MOV, NULL, arm_op1, arm_op2, NULL, NULL);
    pushFrontDequeList(queue, arm_ir);

    __generator_func_return(queue);
    arm_op1 = newRegister(PHISICAL, LR);
    arm_ir = newArmIr(ARM_BX, NULL, arm_op1, NULL, NULL, NULL);
    pushFrontDequeList(queue, arm_ir);
}

func_head(call) {
    if (__is_tail_call(ir, block)) {
        if (strcmp(func->name, ((struct FuncTabElem*)(ir->op1->operand.v.funcID))->name) == 0) {
            return __convert_tail_recusive_to_arm(ir, queue, block, func);
        } else {
            return __convert_tail_call_to_arm(ir, queue, block, func);
        }
        return __convert_normal_call_to_arm(ir, queue, block, func);
    }
}

#undef func_head
#undef opreands
void __convert_ir_ssa_to_arm(IR_TYPE* ir, struct DequeList* queue, BASIC_BLOCK_TYPE* block, struct FuncTabElem* func) {
    switch (ir->type) {
        case NOP:
        case PARAM:
            break;
        case CALL:
            __convert_call_to_arm(ir, queue, block, func);
            break;
        case JUMP:
            __convert_jump_to_arm(ir, queue, block, func);
            break;
        case BRANCH:
            __convert_branch_to_arm(ir, queue, block, func);
            break;
        case RETURNSTMT:
            __convert_return_stmt_to_arm(ir, queue, block, func);
            break;
        case LOAD:
            __convert_load_to_arm(ir, queue, block, func);
            break;
        case STORE:
            __convert_store_to_arm(ir, queue, block, func);
            break;
        case ASSIGN:
            __convert_assign_to_arm(ir, queue, block, func);
            break;
        case K_NOT:
            __convert_k_not_to_arm(ir, queue, block, func);
            break;
        case K_ADD:
            __convert_k_add_to_arm(ir, queue, block, func);
            break;
        case K_SUB:
            __convert_k_sub_to_arm(ir, queue, block, func);
            break;
        case K_MUL:
            __convert_k_mul_to_arm(ir, queue, block, func);
            break;
        case K_DIV:
            __convert_k_div_to_arm(ir, queue, block, func);
            break;
        case K_MOD:
            __convert_k_mod_to_arm(ir, queue, block, func);
            break;
        case K_EQ:
            __convert_k_eq_to_arm(ir, queue, block, func);
            break;
        case K_NEQ:
            __convert_k_neq_to_arm(ir, queue, block, func);
            break;
        case K_LT:
            __convert_k_lt_to_arm(ir, queue, block, func);
            break;
        case K_LTE:
            __convert_k_lte_to_arm(ir, queue, block, func);
            break;
        case K_GT:
            __convert_k_gt_to_arm(ir, queue, block, func);
            break;
        case K_GTE:
            __convert_k_gte_to_arm(ir, queue, block, func);
            break;

        default:
            PrintErrExit("not support ssa ir type %d, %s", ir->type, EnumTypeToString(ir->type));
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
        IR_TYPE* ir = le2struct(ir_elem, IR_TYPE, ir_link);
        ir_elem = list_next(ir_elem);
        __convert_ir_ssa_to_arm(ir, arm_ir_queue, block, func);
        while (!isEmptyDequeList(arm_ir_queue)) {
            arm_ir = popBackDequeList(arm_ir_queue);
            list_add_before(arm_ir_head, &arm_ir->ir_link);
        }
    }
}

void convertSSAToArmFunc(struct FuncTabElem* func) {
    if (func->blocks != NULL) {
        deepTraverseSuccessorsBasicBlock(func->blocks, __convert_from_ssa_to_arm, func);
    }
}