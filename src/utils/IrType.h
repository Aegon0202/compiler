#ifndef UTILS_IR_TYPE_H
#define UTILS_IR_TYPE_H
#include "../ENUM.h"
#include "../ssa/ssa.h"

#define IR_OP_READ_WRITE(ir_type, READ_OP, WRITE_OP, DEFAULT) \
    switch (ir_type) {                                        \
        case NOP:                                             \
            break;                                            \
        case PARAM:                                           \
            READ_OP(3);                                       \
            break;                                            \
        case CALL:                                            \
            READ_OP(1);                                       \
            READ_OP(2);                                       \
            WRITE_OP(3);                                      \
            break;                                            \
        case JUMP:                                            \
            READ_OP(3);                                       \
            break;                                            \
        case BRANCH:                                          \
            READ_OP(1);                                       \
            READ_OP(2);                                       \
            READ_OP(3);                                       \
            break;                                            \
        case RETURNSTMT:                                      \
            READ_OP(1);                                       \
            break;                                            \
        case LOAD:                                            \
            READ_OP(1);                                       \
            READ_OP(2);                                       \
            WRITE_OP(3);                                      \
            break;                                            \
        case STORE:                                           \
            READ_OP(1);                                       \
            READ_OP(2);                                       \
            READ_OP(3);                                       \
            break;                                            \
        case ASSIGN:                                          \
            READ_OP(1);                                       \
            WRITE_OP(3);                                      \
            break;                                            \
        case K_NOT:                                           \
            READ_OP(1);                                       \
            WRITE_OP(3);                                      \
            break;                                            \
        case K_ADD:                                           \
        case K_SUB:                                           \
        case K_MUL:                                           \
        case K_DIV:                                           \
        case K_MOD:                                           \
        case K_AND:                                           \
        case K_OR:                                            \
        case K_EQ:                                            \
        case K_NEQ:                                           \
        case K_LT:                                            \
        case K_LTE:                                           \
        case K_GT:                                            \
        case K_GTE:                                           \
            READ_OP(1);                                       \
            READ_OP(2);                                       \
            WRITE_OP(3);                                      \
            break;                                            \
        case PHI:                                             \
            WRITE_OP(3);                                      \
            break;                                            \
        default:                                              \
            DEFAULT;                                          \
    }

int isWriteOp(IR_TYPE* ir, int op_index);
int isReadOp(IR_TYPE* ir, int op_index);

#endif