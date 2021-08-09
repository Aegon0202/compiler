#ifndef UTILS_IR_TYPE_H
#define UTILS_IR_TYPE_H
#include "../ENUM.h"
#include "../ssa/ssa.h"

#define IR_OP_READ_WRITE(IR_instance, READ_OP, WRITE_OP, DEFAULT)                  \
    switch (IR_instance->type) {                                                   \
        case NOP:                                                                  \
            break;                                                                 \
        case PARAM:                                                                \
            READ_OP(IR_instance->op3);                                             \
            break;                                                                 \
        case CALL:                                                                 \
            READ_OP(IR_instance->op1);                                             \
            READ_OP(IR_instance->op2);                                             \
            WRITE_OP(IR_instance->op3);                                            \
            break;                                                                 \
        case JUMP:                                                                 \
            READ_OP(IR_instance->op3);                                             \
            break;                                                                 \
        case BRANCH:                                                               \
            READ_OP(IR_instance->op1);                                             \
            READ_OP(IR_instance->op2);                                             \
            READ_OP(IR_instance->op3);                                             \
            break;                                                                 \
        case RETURNSTMT:                                                           \
            READ_OP(IR_instance->op1);                                             \
            break;                                                                 \
        case LOAD:                                                                 \
            READ_OP(IR_instance->op1);                                             \
            READ_OP(IR_instance->op2);                                             \
            WRITE_OP(IR_instance->op3);                                            \
            break;                                                                 \
        case STORE:                                                                \
            READ_OP(IR_instance->op1);                                             \
            READ_OP(IR_instance->op2);                                             \
            READ_OP(IR_instance->op3);                                             \
            break;                                                                 \
        case ASSIGN:                                                               \
            READ_OP(IR_instance->op1);                                             \
            WRITE_OP(IR_instance->op3);                                            \
            break;                                                                 \
        case K_NOT:                                                                \
            READ_OP(IR_instance->op1);                                             \
            WRITE_OP(IR_instance->op3);                                            \
            break;                                                                 \
        case K_ADD:                                                                \
        case K_SUB:                                                                \
        case K_MUL:                                                                \
        case K_DIV:                                                                \
        case K_MOD:                                                                \
        case K_AND:                                                                \
        case K_OR:                                                                 \
        case K_EQ:                                                                 \
        case K_NEQ:                                                                \
        case K_LT:                                                                 \
        case K_LTE:                                                                \
        case K_GT:                                                                 \
        case K_GTE:                                                                \
            READ_OP(IR_instance->op1);                                             \
            READ_OP(IR_instance->op2);                                             \
            WRITE_OP(IR_instance->op3);                                            \
            break;                                                                 \
        case PHI:                                                                  \
            break;                                                                 \
        default:                                                                   \
            DEFAULT;                                                               \
    }                                                                              \
    if (IR_instance->type == PHI) {                                                \
        list_entry_t* macro_phi_op_head = IR_instance->op1->operand.v.phi_op_list; \
        list_entry_t* macro_phi_op_elem = list_next(macro_phi_op_head);            \
        while (macro_phi_op_elem != macro_phi_op_head) {                           \
            READ_OP((le2struct(macro_phi_op_elem, Phi, op_link)->value));          \
            macro_phi_op_elem = list_next(macro_phi_op_elem);                      \
        }                                                                          \
        WRITE_OP(IR_instance->op3);                                                \
    }

#define ARM_IR_OP_READ_WRITE(ARM_IR_INSTANCE, READ_REG, READ_OPERAND2, WRITE_REG, DEFAULT) \
    switch (ARM_IR_INSTANCE->type) {                                                       \
        case ARM_ADD:                                                                      \
        case ARM_SUB:                                                                      \
            READ_REG(ARM_IR_INSTANCE->op2);                                                \
            READ_OPERAND2(ARM_IR_INSTANCE->op3);                                           \
            WRITE_REG(ARM_IR_INSTANCE->op1);                                               \
            break;                                                                         \
        case ARM_MUL:                                                                      \
        case ARM_SDIV:                                                                     \
        case ARM_SMMUL:                                                                    \
            READ_REG(ARM_IR_INSTANCE->op2);                                                \
            READ_REG(ARM_IR_INSTANCE->op3);                                                \
            WRITE_REG(ARM_IR_INSTANCE->op1);                                               \
            break;                                                                         \
        case ARM_MOV:                                                                      \
        case ARM_MVN:                                                                      \
            READ_OPERAND2(ARM_IR_INSTANCE->op2);                                           \
            WRITE_REG(ARM_IR_INSTANCE->op1);                                               \
            break;                                                                         \
        case ARM_CLZ:                                                                      \
            READ_REG(ARM_IR_INSTANCE->op2);                                                \
            WRITE_REG(ARM_IR_INSTANCE->op1);                                               \
            break;                                                                         \
        case ARM_MOVW:                                                                     \
        case ARM_MOVT:                                                                     \
        case ARM_MOVW_L:                                                                   \
        case ARM_MOVT_L:                                                                   \
            WRITE_REG(ARM_IR_INSTANCE->op1);                                               \
            break;                                                                         \
        case ARM_CMP:                                                                      \
        case ARM_CMN:                                                                      \
            READ_REG(ARM_IR_INSTANCE->op1);                                                \
            READ_OPERAND2(ARM_IR_INSTANCE->op2);                                           \
            break;                                                                         \
        case ARM_B:                                                                        \
        case ARM_BL:                                                                       \
            break;                                                                         \
        case ARM_BX:                                                                       \
        case ARM_BLX:                                                                      \
            READ_REG(ARM_IR_INSTANCE->op1);                                                \
            break;                                                                         \
        case ARM_LDR_I:                                                                    \
            READ_REG(ARM_IR_INSTANCE->op2);                                                \
            WRITE_REG(ARM_IR_INSTANCE->op1);                                               \
            break;                                                                         \
        case ARM_LDR_I_PRE:                                                                \
        case ARM_LDR_I_POST:                                                               \
            READ_REG(ARM_IR_INSTANCE->op2);                                                \
            WRITE_REG(ARM_IR_INSTANCE->op1);                                               \
            WRITE_REG(ARM_IR_INSTANCE->op2);                                               \
            break;                                                                         \
        case ARM_LDR_R:                                                                    \
            READ_REG(ARM_IR_INSTANCE->op2);                                                \
            READ_OPERAND2(ARM_IR_INSTANCE->op3);                                           \
            WRITE_REG(ARM_IR_INSTANCE->op1);                                               \
            break;                                                                         \
        case ARM_LDR_R_PRE:                                                                \
        case ARM_LDR_R_POST:                                                               \
            READ_REG(ARM_IR_INSTANCE->op2);                                                \
            READ_OPERAND2(ARM_IR_INSTANCE->op3);                                           \
            WRITE_REG(ARM_IR_INSTANCE->op1);                                               \
            WRITE_REG(ARM_IR_INSTANCE->op2);                                               \
            break;                                                                         \
        case ARM_STR_I:                                                                    \
            READ_REG(ARM_IR_INSTANCE->op2);                                                \
            READ_REG(ARM_IR_INSTANCE->op1);                                                \
            break;                                                                         \
        case ARM_STR_I_PRE:                                                                \
        case ARM_STR_I_POST:                                                               \
            READ_REG(ARM_IR_INSTANCE->op2);                                                \
            READ_REG(ARM_IR_INSTANCE->op1);                                                \
            WRITE_REG(ARM_IR_INSTANCE->op2);                                               \
            break;                                                                         \
        case ARM_STR_R:                                                                    \
            READ_REG(ARM_IR_INSTANCE->op2);                                                \
            READ_OPERAND2(ARM_IR_INSTANCE->op3);                                           \
            READ_REG(ARM_IR_INSTANCE->op1);                                                \
            break;                                                                         \
        case ARM_STR_R_PRE:                                                                \
        case ARM_STR_R_POST:                                                               \
            READ_REG(ARM_IR_INSTANCE->op2);                                                \
            READ_OPERAND2(ARM_IR_INSTANCE->op3);                                           \
            READ_REG(ARM_IR_INSTANCE->op1);                                                \
            WRITE_REG(ARM_IR_INSTANCE->op2);                                               \
            break;                                                                         \
        case ARM_PUSH:                                                                     \
        case ARM_POP:                                                                      \
        case ARM_LABEL:                                                                    \
            break;                                                                         \
        default:                                                                           \
            DEFAULT;                                                                       \
    }                                                                                      \
    if (ARM_IR_INSTANCE->type == ARM_PUSH) {                                               \
        int PUSH_DEQUE_NUM = sizeDequeList(ARM_IR_INSTANCE->op1);                          \
        for (int INDEX = 0; INDEX < PUSH_DEQUE_NUM; INDEX++) {                             \
            READ_REG(getDequeList(ARM_IR_INSTANCE->op1, INDEX));                           \
        }                                                                                  \
    }                                                                                      \
    if (ARM_IR_INSTANCE->type == ARM_POP) {                                                \
        int PUSH_DEQUE_NUM = sizeDequeList(ARM_IR_INSTANCE->op1);                          \
        for (int INDEX = 0; INDEX < PUSH_DEQUE_NUM; INDEX++) {                             \
            WRITE_REG(getDequeList(ARM_IR_INSTANCE->op1, INDEX));                          \
        }                                                                                  \
    }

int isWriteOp(IR_TYPE* IR_instance, int op_index);
int isReadOp(IR_TYPE* IR_instance, int op_index);

#endif