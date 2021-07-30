#include "../ssa/ssa.h"
#include "../ssa/traverse.h"
#include "./SysY.target.arm.h"

void __convert_ir_ssa_to_arm(IR_TYPE* ir) {
    switch (ir->type) {
        case NOP:
            break;
        case PARAM:
            READ_OP(ir->op3);
            break;
        case CALL:
            READ_OP(ir->op1);
            READ_OP(ir->op2);
            WRITE_OP(ir->op3);
            break;
        case JUMP:
            READ_OP(ir->op3);
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

void __convert_from_ssa_to_arm(BASIC_BLOCK_TYPE* block) {
}