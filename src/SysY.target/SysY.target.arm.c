#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include "./SysY.target.arm.h"

#include <stdio.h>

#include "../ssa/ssa.h"

struct Immi_8* newImmi_8(int num) {
    MALLOC(imm, struct Immi_8, 1);
    imm->num = num & 0xff;
    return imm;
}

struct Immi_12* newImmi_12(int num) {
    MALLOC(imm, struct Immi_12, 1);
    imm->num = num & 0xfff;
    return imm;
}

struct Immi_16* newImmi_16(int num) {
    MALLOC(imm, struct Immi_16, 1);
    imm->num = num & 0xffff;
    return imm;
}

struct Register* newRegister(int type, int reg_num) {
    MALLOC(reg, struct Register, 1);
    reg->type = type;
    reg->reg = reg_num;
    return reg;
}

struct Label* newLabel(const char* label_name) {
    MALLOC(label, struct Label, 1);
    label->label = strdup(label_name);
    return label;
}

struct Operand2* newOperand2(int type, void* Rm) {
    MALLOC(opr, struct Operand2, 1);
    opr->type = type;
    opr->Rm.immi_8 = Rm;
    opr->shift_type = NOP;
    opr->shift_op = IMMI_8;
    opr->shift.immi_8 = NULL;
    return opr;
}

struct CondOp* newCondOp(int type) {
    MALLOC(cond, struct CondOp, 1);
    cond->type = type;
    return cond;
}

struct ArmIr* newArmIr(int type, struct CondOp* cond, void* op1, void* op2, void* op3, void* op4) {
    MALLOC(ir, struct ArmIr, 1);
    ir->type = type;
    if (cond != NULL) {
        ir->cond = cond;
    } else {
        ir->cond = newCondOp(AL);
    }
    ir->op1 = op1;
    ir->op2 = op2;
    ir->op3 = op3;
    ir->op4 = op4;
    list_init(&ir->ir_link);
    return ir;
}