#ifndef UTILS_IR_TYPE_H
#define UTILS_IR_TYPE_H
#include "../ENUM.h"
#include "../ssa/ssa.h"

#define IR_OP_READ_WRITE(IR_instance, READ_OP, WRITE_OP, DEFAULT)            \
    switch (IR_instance->type) {                                             \
        case NOP:                                                            \
            break;                                                           \
        case PARAM:                                                          \
            READ_OP(IR_instance->op3);                                       \
            break;                                                           \
        case CALL:                                                           \
            READ_OP(IR_instance->op1);                                       \
            READ_OP(IR_instance->op2);                                       \
            WRITE_OP(IR_instance->op3);                                      \
            break;                                                           \
        case JUMP:                                                           \
            READ_OP(IR_instance->op3);                                       \
            break;                                                           \
        case BRANCH:                                                         \
            READ_OP(IR_instance->op1);                                       \
            READ_OP(IR_instance->op2);                                       \
            READ_OP(IR_instance->op3);                                       \
            break;                                                           \
        case RETURNSTMT:                                                     \
            READ_OP(IR_instance->op1);                                       \
            break;                                                           \
        case LOAD:                                                           \
            READ_OP(IR_instance->op1);                                       \
            READ_OP(IR_instance->op2);                                       \
            WRITE_OP(IR_instance->op3);                                      \
            break;                                                           \
        case STORE:                                                          \
            READ_OP(IR_instance->op1);                                       \
            READ_OP(IR_instance->op2);                                       \
            READ_OP(IR_instance->op3);                                       \
            break;                                                           \
        case ASSIGN:                                                         \
            READ_OP(IR_instance->op1);                                       \
            WRITE_OP(IR_instance->op3);                                      \
            break;                                                           \
        case K_NOT:                                                          \
            READ_OP(IR_instance->op1);                                       \
            WRITE_OP(IR_instance->op3);                                      \
            break;                                                           \
        case K_ADD:                                                          \
        case K_SUB:                                                          \
        case K_MUL:                                                          \
        case K_DIV:                                                          \
        case K_MOD:                                                          \
        case K_AND:                                                          \
        case K_OR:                                                           \
        case K_EQ:                                                           \
        case K_NEQ:                                                          \
        case K_LT:                                                           \
        case K_LTE:                                                          \
        case K_GT:                                                           \
        case K_GTE:                                                          \
            READ_OP(IR_instance->op1);                                       \
            READ_OP(IR_instance->op2);                                       \
            WRITE_OP(IR_instance->op3);                                      \
            break;                                                           \
        case PHI:                                                            \
            break;                                                           \
        default:                                                             \
            DEFAULT;                                                         \
    }                                                                        \
    if (IR_instance->type == PHI) {                                          \
        list_entry_t* phi_op_head = IR_instance->op1->operand.v.phi_op_list; \
        list_entry_t* elem = list_next(phi_op_head);                         \
        while (elem != phi_op_head) {                                        \
            Operand* op = le2struct(elem, Phi, op_link)->value;              \
            READ_OP(op);                                                     \
            elem = list_next(elem);                                          \
        }                                                                    \
        WRITE_OP(IR_instance->op3);                                          \
    }

int isWriteOp(IR_TYPE* IR_instance, int op_index);
int isReadOp(IR_TYPE* IR_instance, int op_index);

#endif